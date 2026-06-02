    
     #include "system.h"

    #define MSIP_ADDR      (ALT_CPU_MTIME_OFFSET + 0x10)
    #define SIERRA_BASE    (SIERRA_0_BASE)
    #define SIERRA_STATUSB_REG (SIERRA_0_BASE + 0x30)



    .section .exceptions
    .align  2

    .globl  sierra_trap_vector
    .globl  __alt_exception_stack_pointer
    .globl  __alt_exception_stack_limit
    .type   sierra_trap_vector, @function

# Register      ABI Name    Description                       Saver
 # x0            zero        Hard-wired zero                   -
 # x1            ra          Return address                    Caller
 # x2            sp          Stack pointer                     Callee
 # x3            gp          Global pointer                    -
 # x4            tp          Thread pointer                    -
 # x5-7          t0-2        Temporaries                       Caller
 # x8            s0/fp       Saved register/Frame pointer      Callee
 # x9            s1          Saved register                    Callee
 # x10-11        a0-1        Function Arguments/return values  Caller
 # x12-17        a2-7        Function arguments                Caller
 # x18-27        s2-11       Saved registers                   Callee
 # x28-31        t3-6        Temporaries                       Caller

sierra_trap_vector:
# Stäng av globala interrupts (Machine)
csrc  mstatus, 0x8    # clear MIE (bit 3)

    addi    sp, sp, -30*4

    sw      ra,  0*4(sp)
    sw      gp,  1*4(sp)
    sw      tp,  2*4(sp)
    sw      t0,  3*4(sp)
    sw      t1,  4*4(sp)
    sw      t2,  5*4(sp)
    sw      s0,  6*4(sp)
    sw      s1,  7*4(sp)
    sw      a0,  8*4(sp)
    sw      a1,  9*4(sp)
    sw      a2, 10*4(sp)
    sw      a3, 11*4(sp)
    sw      a4, 12*4(sp)
    sw      a5, 13*4(sp)
    sw      a6, 14*4(sp)
    sw      a7, 15*4(sp)
    sw      s2, 16*4(sp)
    sw      s3, 17*4(sp)
    sw      s4, 18*4(sp)
    sw      s5, 19*4(sp)
    sw      s6, 20*4(sp)
    sw      s7, 21*4(sp)
    sw      s8, 22*4(sp)
    sw      s9, 23*4(sp)
    sw      s10,24*4(sp)
    sw      s11,25*4(sp)
    sw      t3, 26*4(sp)
    sw      t4, 27*4(sp)
    sw      t5, 28*4(sp)
    sw      t6, 29*4(sp)

# Läs trap-information
    csrr    a0, mcause          # a0 = mcause
    csrr    a1, mepc            # a1 = trappad PC
    csrr    a2, mtval           # a2 = extra trapinfo

    # ---------------------------------------------------------
    # 1) Är det Machine Software Interrupt? (0x80000003)
    #    → rensa MSIP och gå direkt till Kontext_switch
    # ---------------------------------------------------------
    li      t0, 0x80000003      # M Software Interrupt
    bne     a0, t0, check_external_irq

    # Här: mcause == M Software Interrupt , MSIP_ADDR
    li      t1, MSIP_ADDR      # MSIP-adress (plattformsspecifik)
    sw      zero, 0(t1)         # rensa MSIP
    j       Kontext_switch      # context switch för SW interrupt


check_external_irq:
    # ---------------------------------------------------------
    # 2) Är det Machine External Interrupt? (0x80000010)
    #    Om NEJ → normal_trap_handler
    # ---------------------------------------------------------
    li      t0, 0x80000010      # M External Interrupt
    bne     a0, t0, normal_trap_handler   # ej extern IRQ → vanlig trap

    # ---------------------------------------------------------
    # 3) Extern IRQ + extra test för om det har blivit en glitch
    #    Läs byte från CPU SIERRA_STATUSB_REG
    # 0 = context switch request is off => (normal_trap_handler)
    # 1 = context switch request → Kontext_switch
    # ---------------------------------------------------------
    la      t0, SIERRA_STATUSB_REG   # t0 = adress till statusregister
    lbu     t1, 0(t0)                # t1 = läs byte
    andi    t1, t1, 0x01             # maska ut bit0
    beqz    t1, normal_trap_handler  # om bit0 == 0 → normal trap
 #   j       Kontext_switch               # annars -> Kontext_switch (onödig)
 
Kontext_switch:
    la      t1, current_tcb
    lw      t2, 0(t1)
    beqz    t2, first_switch

    lw      t1, 0(t1)             # t1 = current_tcb
    sw      sp, 0(t1)             # spara sp
    csrr    t2, mepc
    sw      t2, 4(t1)             # spara pc

    li      t0, 0x80000010
    beq     a0, t0, irq

    call    get_new_task
    j       sch

irq:
    call    get_next_task

sch:
    la      t1, current_tcb
    lw      t1, 0(t1)             # t1 = new_tcb

    lw      sp, 0(t1)
    lw      t2, 4(t1)
    csrw    mepc, t2

    lw      t3, 8(t1)             # first_run
    beqz    t3, fresh_task

    lw      ra,  0*4(sp)
    lw      gp,  1*4(sp)
    lw      tp,  2*4(sp)
    lw      t0,  3*4(sp)
    lw      t1,  4*4(sp)
    lw      t2,  5*4(sp)
    lw      s0,  6*4(sp)
    lw      s1,  7*4(sp)
    lw      a0,  8*4(sp)
    lw      a1,  9*4(sp)
    lw      a2, 10*4(sp)
    lw      a3, 11*4(sp)
    lw      a4, 12*4(sp)
    lw      a5, 13*4(sp)
    lw      a6, 14*4(sp)
    lw      a7, 15*4(sp)
    lw      s2, 16*4(sp)
    lw      s3, 17*4(sp)
    lw      s4, 18*4(sp)
    lw      s5, 19*4(sp)
    lw      s6, 20*4(sp)
    lw      s7, 21*4(sp)
    lw      s8, 22*4(sp)
    lw      s9, 23*4(sp)
    lw      s10,24*4(sp)
    lw      s11,25*4(sp)
    lw      t3, 26*4(sp)
    lw      t4, 27*4(sp)
    lw      t5, 28*4(sp)
    lw      t6, 29*4(sp)

    addi    sp, sp, 30*4
    mret

fresh_task:
    li      t3, 1
    la      t0, current_tcb
    lw      t0, 0(t0)
    sw      t3, 8(t0)             # markera first_run = 1
    mret

first_switch:
    call    get_next_task

    la      t1, current_tcb
    lw      t1, 0(t1)

    lw      sp, 0(t1)
    lw      t2, 4(t1)
    csrw    mepc, t2

    li      t3, 1
    sw      t3, 8(t1)

    mret

normal_trap_handler:
 #   jal     handle_trap
    call handle_trap #temporär lösning
    csrw    mepc, a0

    lw      ra,  0*4(sp)
    lw      gp,  1*4(sp)
    lw      tp,  2*4(sp)
    lw      t0,  3*4(sp)
    lw      t1,  4*4(sp)
    lw      t2,  5*4(sp)
    lw      s0,  6*4(sp)
    lw      s1,  7*4(sp)
    lw      a0,  8*4(sp)
    lw      a1,  9*4(sp)
    lw      a2, 10*4(sp)
    lw      a3, 11*4(sp)
    lw      a4, 12*4(sp)
    lw      a5, 13*4(sp)
    lw      a6, 14*4(sp)
    lw      a7, 15*4(sp)
    lw      s2, 16*4(sp)
    lw      s3, 17*4(sp)
    lw      s4, 18*4(sp)
    lw      s5, 19*4(sp)
    lw      s6, 20*4(sp)
    lw      s7, 21*4(sp)
    lw      s8, 22*4(sp)
    lw      s9, 23*4(sp)
    lw      s10,24*4(sp)
    lw      s11,25*4(sp)
    lw      t3, 26*4(sp)
    lw      t4, 27*4(sp)
    lw      t5, 28*4(sp)
    lw      t6, 29*4(sp)

    addi    sp, sp, 30*4
    mret
