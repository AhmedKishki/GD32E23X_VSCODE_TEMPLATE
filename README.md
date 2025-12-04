# GD32E23x Project Skeleton

This repository provides a minimal working setup for developing firmware for the **GD32E23x** series microcontrollers using:

- GCC toolchain (`arm-none-eabi`)
- OpenOCD with **ST-LINK V2 / V3** or **GD-Link** (CMSIS-DAP)
- VSCode + Cortex-Debug extension

It is:

- **Developed and tested on Linux (Ubuntu)**  
- A **VSCode template** for working on GD32E23x-based projects  
- Ready for projects with or without **FreeRTOS** (the Makefile is FreeRTOS-friendly)

The setup is designed to work **without any vendor IDE** such as GigaDevice Embedded Builder and **without the GD-Link GUI tools**: you can rely entirely on a standard, open toolchain (Make + GCC + OpenOCD + VSCode).

A ready-to-use `openocd_gdlink.cfg` is included so you can use **GD-Link hardware** directly from OpenOCD if you want, but you can just as well work only with **ST-LINK V2/V3** for flashing and debugging.

It includes:

- Project structure for C and C++ source files
- Working `launch.json` and `tasks.json` for debugging and building in VSCode
- A functional `openocd_gdlink.cfg` script for flashing via GD-Link (CMSIS-DAP)
- CMSIS headers and SVD file for peripheral visualization
- A `Makefile` supporting:
  - Debug and Release builds
  - ELF, HEX, BIN, LST, MAP, and size report generation
  - Optional integration of **FreeRTOS** (drop in the kernel and enable it via configuration)

This skeleton can be used as a base for new GD32E23x projects on Linux/Ubuntu.

---

## 📦 Dependencies

The following tools must be installed and available in your terminal:

| Tool                  | Purpose                                                    |
|-----------------------|------------------------------------------------------------|
| `arm-none-eabi-gcc`   | Compiler for ARM Cortex-M23                                |
| `make`                | Used to run the build system (Makefile)                    |
| `openocd`             | Flashing and debugging via ST-LINK / GD-Link (CMSIS-DAP)   |
| `VSCode`              | IDE for editing and debugging                              |
| `Cortex-Debug`        | VSCode extension for GDB-based debugging                   |
| `ST-LINK V2` / `V3` or `GD-Link` | Debug adapters (CMSIS-DAP compatible)          |

You can verify toolchain installation with:

```bash
arm-none-eabi-gcc --version
openocd --version
make --version
