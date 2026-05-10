set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "Nios V/m")
set(CMAKE_CROSSCOMPILING True)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_AR riscv32-unknown-elf-ar)
set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)

set(RISCV_TOOLCHAIN_BIN_HINT
    "C:/altera_lite/25.1std/riscfree/toolchain/riscv32-unknown-elf/bin"
    CACHE PATH "Directory containing the Nios V RISC-V cross compiler tools.")
set(RISCV_TOOLCHAIN_BIN_HINTS
    "${RISCV_TOOLCHAIN_BIN_HINT}"
    "$ENV{QUARTUS_ROOTDIR}/../riscfree/toolchain/riscv32-unknown-elf/bin"
    "$ENV{QUARTUS_ROOTDIR}/../riscfree/toolchain/riscv32-unknown-elf/bin64"
    "/home/ludvig/altera_lite/25.1std/riscfree/toolchain/riscv32-unknown-elf/bin")

foreach(tool_var CMAKE_AR CMAKE_ASM_COMPILER CMAKE_C_COMPILER)
    if(DEFINED ${tool_var} AND NOT IS_ABSOLUTE "${${tool_var}}")
        find_program(resolved_${tool_var}
            NAMES "${${tool_var}}" "${${tool_var}}.exe"
            HINTS ${RISCV_TOOLCHAIN_BIN_HINTS})
        if(resolved_${tool_var})
            set(${tool_var} "${resolved_${tool_var}}" CACHE FILEPATH "" FORCE)
        endif()
    endif()
endforeach()

set(ToolchainPrefix riscv32-unknown-elf- CACHE STRING "Toolchain prefix." FORCE)

set(ToolchainObjdump riscv32-unknown-elf-objdump CACHE STRING "Objdump executable." FORCE)
set(ToolchainObjdumpFlags -Sdtx CACHE STRING "Objdump flags." FORCE)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Defaulting build type to Debug.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g")

set(CMAKE_C_FLAGS_RELEASE "-O2")

add_compile_options(
    $<$<COMPILE_LANGUAGE:ASM>:-Wa,-gdwarf2>
    -DALT_SINGLE_THREADED
    -Wall -Wformat-security
    -Wformat
    -Wformat-security
    -fno-stack-protector
    -march=rv32i -mabi=ilp32
)

add_link_options(
    -march=rv32i -mabi=ilp32
    -nostdlib
)

add_compile_definitions(
    ALTERA_AVALON_JTAG_UART_IGNORE_FIFO_FULL_ERROR
    ALTERA_AVALON_JTAG_UART_SMALL
    ALT_LOG_ENABLE
    ALT_LOG_FLAGS=0
    ALT_NO_C_PLUS_PLUS
    ALT_NO_EXIT
    ALT_USE_DIRECT_DRIVERS
    ALT_USE_SMALL_DRIVERS
    __hal__
)

remove_definitions(
)
