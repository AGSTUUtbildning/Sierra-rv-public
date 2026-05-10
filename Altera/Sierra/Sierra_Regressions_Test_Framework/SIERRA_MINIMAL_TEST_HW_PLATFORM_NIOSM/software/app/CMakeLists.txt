cmake_minimum_required(VERSION 3.14)

set(BSP_DIR "${CMAKE_CURRENT_LIST_DIR}/../hal_bsp")

include("${BSP_DIR}/toolchain.cmake")

project(app LANGUAGES C ASM)

add_subdirectory("${BSP_DIR}" hal_bsp)

file(GLOB SIERRA_TEST_SRCS CONFIGURE_DEPENDS "Testbench_code/*.c")

add_executable(app.elf
    main.c
    ${SIERRA_TEST_SRCS}
)

target_include_directories(app.elf PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/Testbench_code"
)

target_link_libraries(app.elf
    PRIVATE
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lgcc -Wl,--end-group
)

target_compile_options(app.elf PRIVATE
    -Os
    -fno-stack-protector
    -ffunction-sections
    -fdata-sections
)

target_link_options(app.elf PRIVATE
    -T "${BspLinkerScript}"
    -nostdlib
    -Wl,--gc-sections
)

get_filename_component(TOOLCHAIN_BIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component(QUARTUS_STD_ROOT "${TOOLCHAIN_BIN_DIR}/../../../.." ABSOLUTE)
set(NIOSV_TOOLS_BIN "${QUARTUS_STD_ROOT}/niosv/bin")
set(TOOLCHAIN_EXECUTABLE_PREFIX "${TOOLCHAIN_BIN_DIR}/${ToolchainPrefix}")

find_program(ELF2HEX_EXECUTABLE elf2hex HINTS "${NIOSV_TOOLS_BIN}" NO_DEFAULT_PATH)
find_program(NIOSV_STACK_REPORT_EXECUTABLE niosv-stack-report HINTS "${NIOSV_TOOLS_BIN}" NO_DEFAULT_PATH)

if(NOT ELF2HEX_EXECUTABLE)
    message(FATAL_ERROR "elf2hex not found in ${NIOSV_TOOLS_BIN}")
endif()

if(NOT NIOSV_STACK_REPORT_EXECUTABLE)
    message(FATAL_ERROR "niosv-stack-report not found in ${NIOSV_TOOLS_BIN}")
endif()

set(objdump app.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS app.elf
    COMMAND "${TOOLCHAIN_EXECUTABLE_PREFIX}objdump" "${ToolchainObjdumpFlags}" app.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")

set(stack_report_file app.elf.stack_report)
add_custom_command(
    OUTPUT "${stack_report_file}"
    DEPENDS app.elf
    COMMAND "${NIOSV_STACK_REPORT_EXECUTABLE}" -p "${TOOLCHAIN_EXECUTABLE_PREFIX}" app.elf
    COMMENT "Reporting memory available for stack + heap in app.elf."
    VERBATIM
)
add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")

set(SYSTEM_H "${BSP_DIR}/system.h")
function(read_system_define define_name output_var)
    file(STRINGS "${SYSTEM_H}" define_line REGEX "^#define ${define_name} ")
    if(define_line)
        string(REGEX REPLACE "^#define ${define_name}[ \t]+([^ \t]+).*$" "\\1" define_value "${define_line}")
        set("${output_var}" "${define_value}" PARENT_SCOPE)
    else()
        set("${output_var}" "" PARENT_SCOPE)
    endif()
endfunction()

read_system_define(ONCHIP_MEMOY_BASE ONCHIP_MEMORY_BASE)
read_system_define(ONCHIP_MEMOY_SPAN ONCHIP_MEMORY_SPAN)
math(EXPR ONCHIP_MEMORY_END "${ONCHIP_MEMORY_BASE} + ${ONCHIP_MEMORY_SPAN} - 1" OUTPUT_FORMAT HEXADECIMAL)
add_custom_command(
    OUTPUT "onchip_memoy.hex"
    DEPENDS app.elf
    COMMAND "${CMAKE_COMMAND}" -E env "QUARTUS_ROOTDIR=${QUARTUS_STD_ROOT}/quartus"
            "${ELF2HEX_EXECUTABLE}" app.elf -o onchip_memoy.hex -b "${ONCHIP_MEMORY_BASE}" -w 32 -e "${ONCHIP_MEMORY_END}" -r 4
    COMMENT "Creating onchip_memoy.hex."
    VERBATIM
)
add_custom_target(create-hex ALL DEPENDS "onchip_memoy.hex")
