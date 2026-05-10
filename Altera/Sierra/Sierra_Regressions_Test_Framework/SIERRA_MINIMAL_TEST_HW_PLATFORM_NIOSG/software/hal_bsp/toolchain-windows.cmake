set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "Nios V/g")
set(CMAKE_CROSSCOMPILING True)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(SIERRA_INTELFPGA_ROOT "" CACHE PATH
    "Intel Quartus/Nios V installation root, for example C:/intelFPGA_lite/25.1std.")
set(SIERRA_NIOSV_TOOLCHAIN_BIN "" CACHE PATH
    "Directory containing riscv32-unknown-elf-gcc.exe.")
set(SIERRA_NIOSV_BIN "" CACHE PATH
    "Directory containing niosv-stack-report.exe.")

if(NOT SIERRA_INTELFPGA_ROOT AND DEFINED ENV{SIERRA_INTELFPGA_ROOT})
    file(TO_CMAKE_PATH "$ENV{SIERRA_INTELFPGA_ROOT}" SIERRA_INTELFPGA_ROOT)
endif()

if(NOT SIERRA_NIOSV_TOOLCHAIN_BIN AND DEFINED ENV{SIERRA_NIOSV_TOOLCHAIN_BIN})
    file(TO_CMAKE_PATH "$ENV{SIERRA_NIOSV_TOOLCHAIN_BIN}" SIERRA_NIOSV_TOOLCHAIN_BIN)
endif()

if(NOT SIERRA_NIOSV_BIN AND DEFINED ENV{SIERRA_NIOSV_BIN})
    file(TO_CMAKE_PATH "$ENV{SIERRA_NIOSV_BIN}" SIERRA_NIOSV_BIN)
endif()

if(NOT SIERRA_INTELFPGA_ROOT)
    foreach(_sierra_intelfpga_root
            "C:/altera_lite/25.1std"
            "C:/intelFPGA_lite/25.1std"
            "C:/intelFPGA/25.1std")
        if(EXISTS "${_sierra_intelfpga_root}/riscfree/toolchain/riscv32-unknown-elf/bin")
            set(SIERRA_INTELFPGA_ROOT "${_sierra_intelfpga_root}" CACHE PATH
                "Intel Quartus/Nios V installation root." FORCE)
            break()
        endif()
    endforeach()
endif()

if(NOT SIERRA_NIOSV_TOOLCHAIN_BIN AND SIERRA_INTELFPGA_ROOT)
    set(SIERRA_NIOSV_TOOLCHAIN_BIN "${SIERRA_INTELFPGA_ROOT}/riscfree/toolchain/riscv32-unknown-elf/bin"
        CACHE PATH "Directory containing riscv32-unknown-elf-gcc.exe." FORCE)
endif()

if(NOT SIERRA_NIOSV_BIN AND SIERRA_INTELFPGA_ROOT)
    set(SIERRA_NIOSV_BIN "${SIERRA_INTELFPGA_ROOT}/niosv/bin"
        CACHE PATH "Directory containing niosv-stack-report.exe." FORCE)
endif()

find_program(CMAKE_AR
    NAMES riscv32-unknown-elf-ar.exe riscv32-unknown-elf-ar
    HINTS "${SIERRA_NIOSV_TOOLCHAIN_BIN}"
    REQUIRED
)
find_program(CMAKE_ASM_COMPILER
    NAMES riscv32-unknown-elf-gcc.exe riscv32-unknown-elf-gcc
    HINTS "${SIERRA_NIOSV_TOOLCHAIN_BIN}"
    REQUIRED
)
find_program(CMAKE_C_COMPILER
    NAMES riscv32-unknown-elf-gcc.exe riscv32-unknown-elf-gcc
    HINTS "${SIERRA_NIOSV_TOOLCHAIN_BIN}"
    REQUIRED
)
find_program(ToolchainObjdump
    NAMES riscv32-unknown-elf-objdump.exe riscv32-unknown-elf-objdump
    HINTS "${SIERRA_NIOSV_TOOLCHAIN_BIN}"
    REQUIRED
)

set(CMAKE_AR "${CMAKE_AR}" CACHE FILEPATH "Archiver executable." FORCE)
set(CMAKE_ASM_COMPILER "${CMAKE_ASM_COMPILER}" CACHE FILEPATH "ASM compiler executable." FORCE)
set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "C compiler executable." FORCE)

set(ToolchainPrefix riscv32-unknown-elf- CACHE STRING "Toolchain prefix." FORCE)
set(ToolchainObjdump "${ToolchainObjdump}" CACHE FILEPATH "Objdump executable." FORCE)
set(ToolchainObjdumpFlags -Sdtx CACHE STRING "Objdump flags." FORCE)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Defaulting build type to Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")

add_compile_options(
    $<$<COMPILE_LANGUAGE:ASM>:-Wa,-gdwarf2>
    -DALT_SINGLE_THREADED
    -Wall -Wformat-security
    -Wformat
    -Wformat-security
    -ffunction-sections
    -fdata-sections
    -march=rv32imf_zicbom -mabi=ilp32f
)

add_link_options(
    -march=rv32imf_zicbom -mabi=ilp32f
    -nostdlib
    -Wl,--gc-sections
)

add_compile_definitions(
    ALTERA_AVALON_JTAG_UART_IGNORE_FIFO_FULL_ERROR
    ALTERA_AVALON_JTAG_UART_SMALL
    ALT_LOG_ENABLE
    ALT_LOG_FLAGS=0
    ALT_NO_C_PLUS_PLUS
    ALT_USE_DIRECT_DRIVERS
    ALT_USE_SMALL_DRIVERS
    __hal__
)

remove_definitions(
)
