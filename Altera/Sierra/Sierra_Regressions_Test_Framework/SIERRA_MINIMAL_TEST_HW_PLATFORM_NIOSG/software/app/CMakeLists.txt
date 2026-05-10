cmake_minimum_required(VERSION 3.14)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../hal_bsp/toolchain.cmake"
        CACHE FILEPATH "Sierra BSP/app CMake toolchain file.")
endif()

project(app C ASM)

add_subdirectory(../hal_bsp hal_bsp)

find_program(NIOSV_STACK_REPORT_TOOL niosv-stack-report
    HINTS
        "/home/ludvig/altera_lite/25.1std/niosv/bin"
        "$ENV{SIERRA_NIOSV_BIN}"
        "${SIERRA_INTELFPGA_ROOT}/niosv/bin"
        "C:/intelFPGA_lite/25.1std/niosv/bin"
        "C:/intelFPGA/25.1std/niosv/bin"
)

include_directories(Testbench_code)
file(GLOB SIERRA_TEST_SRCS CONFIGURE_DEPENDS "Testbench_code/*.c")

add_executable(app.elf
               main.c
               ${SIERRA_TEST_SRCS}
               )

target_include_directories(app.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(app.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lgcc -lm -Wl,--end-group
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
if(NIOSV_STACK_REPORT_TOOL)
    add_custom_command(
        OUTPUT "${stack_report_file}"
        DEPENDS app.elf
        COMMAND "${NIOSV_STACK_REPORT_TOOL}" -p "${ToolchainPrefix}" app.elf
        COMMENT "Reporting memory available for stack + heap in app.elf."
        VERBATIM
    )
    add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")
else()
    message(WARNING "niosv-stack-report was not found. app.elf will still be built, but no stack report target will be generated.")
endif()
