# Windows 11 Build Notes for the Sierra Nios V/g App

This project builds `software/app/app.elf` plus the BSP with the Intel Nios V RISC-V GCC toolchain. The Linux build remains available through `software/hal_bsp/toolchain.cmake`; Windows should use `software/hal_bsp/toolchain-windows.cmake`.

## Goal

Rebuild the current app and BSP on Windows 11 with:

- Nios V/g target flags: `-march=rv32imf_zicbom -mabi=ilp32f`
- `Release` / size optimized build: `-Os -DNDEBUG`
- section garbage collection: `-ffunction-sections -fdata-sections -Wl,--gc-sections`
- `SIERRA_LOGGING=2`, using the separate `log_memory` region as the buffered logging area
- no hex generation step; the project only needs `app.elf`, `app.elf.objdump`, and the optional stack report

The Linux reference build currently fits the 65 KB on-chip memory:

```text
app.elf
 * 37.37 KB - Program size (code + initialized data).
 * 34.14 KB - Free for stack + heap.
```

## Required Local Windows Setup

Install Intel Quartus/Nios V tools for Windows. The helper files auto-detect these common install roots:

```text
C:\altera_lite\25.1std
C:\intelFPGA_lite\25.1std
C:\intelFPGA\25.1std
```

If your install path is different, set this environment variable before configuring:

```cmd
set SIERRA_INTELFPGA_ROOT=C:\path\to\25.1std
```

The Windows toolchain file derives these paths from `SIERRA_INTELFPGA_ROOT`:

```text
%SIERRA_INTELFPGA_ROOT%\riscfree\toolchain\riscv32-unknown-elf\bin
%SIERRA_INTELFPGA_ROOT%\riscfree\build_tools\cmake\bin
%SIERRA_INTELFPGA_ROOT%\riscfree\build_tools\bin
%SIERRA_INTELFPGA_ROOT%\niosv\bin
```

If those paths differ on the Windows machine, set these variables explicitly:

```cmd
set SIERRA_NIOSV_TOOLCHAIN_BIN=C:\path\to\riscv32-unknown-elf\bin
set SIERRA_NIOSV_BIN=C:\path\to\niosv\bin
```

Use Ninja if available. If not, the included command file falls back to `MinGW Makefiles`, then to Intel's bundled `make.exe` with the `Unix Makefiles` generator.

## Easy Build Command

From a Windows command prompt:

```cmd
cd software
build_app_windows.cmd
```

This script:

1. Adds the Nios V GCC, CMake, and Nios V utility folders to `PATH`.
2. Configures `software/app` with the Windows CMake preset using `--fresh`, so stale Linux or incorrectly configured Windows caches are not reused.
3. Builds `app.elf`.
4. Falls back from Ninja to MinGW Makefiles, then to Intel's bundled make if needed.

## Manual CMake Build

From `software/app`:

```cmd
cmake --preset windows-release-ninja
cmake --build --preset windows-release-ninja
```

If Ninja is not installed:

```cmd
cmake --preset windows-release-mingw
cmake --build --preset windows-release-mingw
```

If neither Ninja nor MinGW Make is installed, use the Intel bundled make preset:

```cmd
cmake --preset windows-release-make
cmake --build --preset windows-release-make
```

If `C:/intelFPGA_lite/25.1std` is not correct, pass the install root at configure time:

```cmd
cmake -S . -B build-win-release -G Ninja ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE=../hal_bsp/toolchain-windows.cmake ^
  -DSIERRA_INTELFPGA_ROOT=C:/intelFPGA_lite/25.1std
cmake --build build-win-release
```

## Files Added for Windows

- `software/hal_bsp/toolchain-windows.cmake`
  - Finds `riscv32-unknown-elf-gcc.exe`, `riscv32-unknown-elf-ar.exe`, and `riscv32-unknown-elf-objdump.exe`.
  - Keeps the same compile/link flags as the working Linux build.
  - Defaults to `Release` so `SIERRA_LOGGING=2` fits in on-chip memory.

- `software/app/CMakePresets.json`
  - Provides `windows-release-ninja`, `windows-release-mingw`, and `windows-release-make`.

- `software/build_app_windows.cmd`
  - Convenience wrapper for Windows command prompt.

## Notes for a Codex Agent on Windows

Start by checking these local facts:

```cmd
where cmake
where riscv32-unknown-elf-gcc
where niosv-stack-report
```

If `where riscv32-unknown-elf-gcc` fails, fix `SIERRA_INTELFPGA_ROOT` or `SIERRA_NIOSV_TOOLCHAIN_BIN` before changing project code.

If configure fails because the generator is missing, install Ninja or use the `windows-release-mingw` preset.
If both Ninja and MinGW Make are missing, use `windows-release-make`; Intel Quartus 25.1 installs the required `make.exe` under `riscfree\build_tools\bin`.

If link fails with `ld.exe: unrecognized option '--major-image-version'`, the project was configured without `CMAKE_TOOLCHAIN_FILE`. Reconfigure with one of the presets or run:

```cmd
cmake --fresh --preset windows-release-make
```

If link fails with `.bss is not within region onchip_memoy`, check that the build type is really `Release`:

```cmd
findstr CMAKE_BUILD_TYPE software\app\build-win-release\CMakeCache.txt
```

For the MinGW fallback preset, use:

```cmd
findstr CMAKE_BUILD_TYPE software\app\build-win-release-mingw\CMakeCache.txt
```

Do not re-enable hex generation. The hex files are not used by this project and previously caused build failures.

Do not move `sierra_log` into normal `.bss`. It must stay in the linker-script `.log_memory (NOLOAD)` section so the log buffer does not consume the main 65 KB on-chip memory image.
