# GD32E23x Project Skeleton

This repository provides a minimal working setup for developing firmware for the **GD32E23x** series microcontrollers using:

- GCC toolchain (`arm-none-eabi`)
- OpenOCD with **GD-Link** (CMSIS-DAP)
- VSCode + Cortex-Debug extension

It includes:

- Project structure for C and C++ source files
- Working `launch.json` and `tasks.json` for debugging and building in VSCode
- A functional `openocd_gdlink.cfg` script for flashing via GD-Link
- CMSIS headers and SVD file for peripheral visualization
- A `Makefile` supporting:
  - Debug and Release builds
  - ELF, HEX, BIN, LST, MAP, and size report generation

This skeleton can be used as a base for new GD32E23x projects on Linux.

---

## 📦 Dependencies

The following tools must be installed and available in your terminal:

| Tool               | Purpose                                      |
|--------------------|----------------------------------------------|
| `arm-none-eabi-gcc`| Compiler for ARM Cortex-M23                  |
| `make`             | Used to run the build system (Makefile)      |
| `openocd`          | Flashing and debugging via GD-Link (CMSIS-DAP) |
| `VSCode`           | IDE for editing and debugging                |
| `Cortex-Debug`     | VSCode extension for GDB-based debugging     |
| `GD-Link`          | Debug adapter (CMSIS-DAP compatible)         |

You can verify toolchain installation with:

```bash
arm-none-eabi-gcc --version
openocd --version
make --version

