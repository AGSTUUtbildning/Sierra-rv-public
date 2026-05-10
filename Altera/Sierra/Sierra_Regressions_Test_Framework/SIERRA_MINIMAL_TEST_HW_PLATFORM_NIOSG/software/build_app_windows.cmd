@echo off
setlocal

rem Adjust this if Quartus/Nios V is installed somewhere else.
if "%SIERRA_INTELFPGA_ROOT%"=="" if exist "C:\altera_lite\25.1std\riscfree\toolchain\riscv32-unknown-elf\bin\riscv32-unknown-elf-gcc.exe" set "SIERRA_INTELFPGA_ROOT=C:\altera_lite\25.1std"
if "%SIERRA_INTELFPGA_ROOT%"=="" if exist "C:\intelFPGA_lite\25.1std\riscfree\toolchain\riscv32-unknown-elf\bin\riscv32-unknown-elf-gcc.exe" set "SIERRA_INTELFPGA_ROOT=C:\intelFPGA_lite\25.1std"
if "%SIERRA_INTELFPGA_ROOT%"=="" if exist "C:\intelFPGA\25.1std\riscfree\toolchain\riscv32-unknown-elf\bin\riscv32-unknown-elf-gcc.exe" set "SIERRA_INTELFPGA_ROOT=C:\intelFPGA\25.1std"
if "%SIERRA_INTELFPGA_ROOT%"=="" set "SIERRA_INTELFPGA_ROOT=C:\intelFPGA_lite\25.1std"

set "SIERRA_NIOSV_TOOLCHAIN_BIN=%SIERRA_INTELFPGA_ROOT%\riscfree\toolchain\riscv32-unknown-elf\bin"
set "SIERRA_NIOSV_BIN=%SIERRA_INTELFPGA_ROOT%\niosv\bin"
set "SIERRA_CMAKE_BIN=%SIERRA_INTELFPGA_ROOT%\riscfree\build_tools\cmake\bin"
set "SIERRA_BUILD_TOOLS_BIN=%SIERRA_INTELFPGA_ROOT%\riscfree\build_tools\bin"

set "PATH=%SIERRA_NIOSV_TOOLCHAIN_BIN%;%SIERRA_NIOSV_BIN%;%SIERRA_CMAKE_BIN%;%SIERRA_BUILD_TOOLS_BIN%;%PATH%"

cd /d "%~dp0app"

cmake --fresh --preset windows-release-ninja
if errorlevel 1 goto :fallback

cmake --build --preset windows-release-ninja
exit /b %errorlevel%

:fallback
echo Ninja configure failed. Trying MinGW Makefiles preset.
cmake --fresh --preset windows-release-mingw
if errorlevel 1 goto :make_fallback

cmake --build --preset windows-release-mingw
exit /b %errorlevel%

:make_fallback
echo MinGW Makefiles configure failed. Trying Intel bundled make.exe preset.
cmake --fresh --preset windows-release-make
if errorlevel 1 exit /b %errorlevel%

cmake --build --preset windows-release-make
exit /b %errorlevel%
