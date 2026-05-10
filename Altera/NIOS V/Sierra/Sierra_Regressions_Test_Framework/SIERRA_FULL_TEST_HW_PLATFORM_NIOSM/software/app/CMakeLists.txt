cmake_minimum_required(VERSION 3.14)

add_subdirectory(../hal_bsp hal_bsp)

include(../hal_bsp/toolchain.cmake)

project(app)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

include_directories(Testbench_code)
file(GLOB SIERRA_TEST_SRCS "Testbench_code/*.c")

add_executable(app.elf
               main.c
               ${SIERRA_TEST_SRCS}
               )
 

target_sources(app.elf
    PRIVATE
        main.c
)

target_include_directories(app.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(app.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lstdc++ -lgcc -lm -Wl,--end-group
)

# Create objdump from ELF.
set(objdump app.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS app.elf
    COMMAND "${ToolchainObjdump}" "${ToolchainObjdumpFlags}" app.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")

# Report space free for stack + heap. Note that the file below is never created
# so the report is always output on build.
set(stack_report_file app.elf.stack_report)
add_custom_command(
    OUTPUT "${stack_report_file}"
    DEPENDS app.elf
    COMMAND niosv-stack-report -p "${ToolchainPrefix}" app.elf
    COMMENT "Reporting memory available for stack + heap in app.elf."
    VERBATIM
)
add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")

# Generate HEX file(s) from app.elf using elf2hex tool.
# Note : If ECC Full is enabled, width of 39 is set for NiosV TCM. Otherwise, 32.
add_custom_command(
    OUTPUT "onchip_memoy.hex"
    DEPENDS app.elf
    COMMAND elf2hex app.elf -o onchip_memoy.hex -b 0x08020000 -w 32 -e 0x0802FFFF -r 4
    COMMENT "Creating onchip_memoy.hex."
    VERBATIM
)
add_custom_command(
    OUTPUT "sdram_controller.hex"
    DEPENDS app.elf
    COMMAND elf2hex app.elf -o sdram_controller.hex -b 0x04000000 -w 32 -e 0x07FFFFFF -r 4
    COMMENT "Creating sdram_controller.hex."
    VERBATIM
)
add_custom_target(create-hex ALL DEPENDS "onchip_memoy.hex" "sdram_controller.hex")
