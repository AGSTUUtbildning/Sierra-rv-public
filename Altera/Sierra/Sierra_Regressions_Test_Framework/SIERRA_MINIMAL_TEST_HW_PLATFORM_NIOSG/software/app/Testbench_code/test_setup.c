/* -------------------------------------------------------
   Simple delay helper (busy-wait)
------------------------------------------------------- */

#include "test_setup.h"

void time_delay(volatile int loops)
{
    for (volatile int i = 0; i < loops; i++)
        asm volatile ("nop");
}

    #include <altera_avalon_jtag_uart_regs.h>

    // konverterar en integer till en 0 - 9 ascii av en char datatyp. 
    #define int_to_char(i)    (unsigned int)(i >= 0 && i < 10 ? 48 + i : 48)

    // En hjälp macro för snabb "lookup" tabell för hur många siffror det är i en int.
    #define GET_DIGITS(x) (          \
        (x) < 0 ? (                  \
            (x) <= -1000000000 ? 10 : \
            (x) <= -100000000 ? 9 :  \
            (x) <= -10000000 ? 8 :   \
            (x) <= -1000000 ? 7 :    \
            (x) <= -100000 ? 6 :     \
            (x) <= -10000 ? 5 :      \
            (x) <= -1000 ? 4 :       \
            (x) <= -100 ? 3 :        \
            (x) <= -10 ? 2 : 1       \
        ) : (                        \
            (x) >= 1000000000 ? 10 : \
            (x) >= 100000000 ? 9 :   \
            (x) >= 10000000 ? 8 :    \
            (x) >= 1000000 ? 7 :     \
            (x) >= 100000 ? 6 :      \
            (x) >= 10000 ? 5 :       \
            (x) >= 1000 ? 4 :        \
            (x) >= 100 ? 3 :         \
            (x) >= 10 ? 2 : 1        \
        )                            \
    )

    // Hämtar en siffra från intergern n vid element pos.
    #define GET_DIGIT_AT(n, pos) (((n) < 0 ? -(n) : (n)) / POWER_OF_10(pos) % 10)

    // Hjälp macro för potensen av 10. 10 element är max för en int med 32 bitar. 
    #define POWER_OF_10(p) ( \
        (p) == 0 ? 1 : \
        (p) == 1 ? 10 : \
        (p) == 2 ? 100 : \
        (p) == 3 ? 1000 : \
        (p) == 4 ? 10000 : \
        (p) == 5 ? 100000 : \
        (p) == 6 ? 1000000 : \
        (p) == 7 ? 10000000 : \
        (p) == 8 ? 100000000 : 1000000000)

    // Maximum 10 siffror i en int på 32 bitar. 
    #define std_max_size 10
    // Buffert som används till att skriva ut string's.
    char agstu_string_buffert[std_max_size];

    static void agstu_write_uart_char(char c) {
        /*
         * Wait until the JTAG UART reports write space so characters are not
         * dropped when several test banners are printed back-to-back.
         */
        while ((IORD_ALTERA_AVALON_JTAG_UART_CONTROL(JTAG_UART_BASE) &
                ALTERA_AVALON_JTAG_UART_CONTROL_WSPACE_MSK) == 0) {
        }

        IOWR_ALTERA_AVALON_JTAG_UART_DATA(JTAG_UART_BASE, (unsigned int)(unsigned char)c);
    }

    // Funktion att konvertera en int till en string. Returnerar en pekare till string bufferten.
    const char* agstu_int_to_string(int nr) {
        char* string_pointer = agstu_string_buffert;
        int digits = GET_DIGITS(nr);
        if (digits > std_max_size) digits = std_max_size;
        while (digits > 0){
            *string_pointer = int_to_char( GET_DIGIT_AT(nr, digits - 1) );
            string_pointer++;
            digits--;
        }
        *string_pointer = '\0';
        return agstu_string_buffert;
    }

    // Funktion att skriva ut en char direkt till jtag UART.
    void agstu_print_char(char c) {
        agstu_write_uart_char(c);
    }

    // Funktion att skriva ut en hel string. Som en normal string måste array sluta med null (\0).
    void agstu_print_string(const char* str) {
        const char* string = str;
        while (*string != '\0') {
            agstu_write_uart_char(*string);
            string++;
        }    
    }
