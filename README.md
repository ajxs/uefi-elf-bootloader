# uefi-elf-bootloader

This repository contains a simple UEFI ELF bootloader which loads a simple demonstration kernel. It provides an extremely basic example implementation of a UEFI bootloader for a bare-metal x86-64 system, though this example should be portable to other architectures.

The aim of this repository is to serve as a basic teachable example of how to implement a UEFI bootloader.

## Build instructions
This bootloader assumes a GCC cross-compiler toolchain targeting the bare-metal `x86_64-elf` architecture. Instructions for building and obtaining a valid cross-compiler toolchain can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler).

This bootloader can be built simply by running `make` within the `src` directory. This will create the `build/kernel.img` file, which is a bootable disk image containing the bootloader loading a demonstration kernel. There is a `run` script within the root directory containing a script for testing the bootloader/kernel combination using QEMU.

### Build dependencies
- GNU Make
- GNU EFI
- An `x86_64-elf-gcc` cross-compiler toolchain present in `PATH`

## Project structure
This project is broken down into two distinct components: The bootloader, and the example kernel. These can be found in the `src/bootloder` and `src/kernel` directories respectively. These can be built and tested individually using the makefiles within their individual directories. Running the makefile within the top level `src` directory will build the entire project.

## Bootloader
The bootloader component of this repository, contained within the `src/bootloader` directory,  contains the basic implementation of a UEFI ELF bootloader for the x86-64 platform. The bootloader is hardcoded to load and execute a bare-metal x86-64 application located at `/kernel.elf` on the boot media.
This path can be modifid from within the `src/bootloader/src/include/bootloader.h` file by modifying the `KERNEL_INCLUDE_PATH` preprocessor directive.

The bootloader will output debugging information over the system's serial port, if present. Otherwise VGA output will be used.

The bootloader passes a `Kernel_Boot_Info` struct to the loaded kernel containing basic system information, such as the memory map. This struct is defined within the `src/bootloader/src/include/bootloader.h` header file. This implementation is not tied to any specific architecture.

The bootloader will open the Graphics Output Protocol and Serial Protocol. A routine has been provided for drawing a test screen to demonstrate that the graphics output protocol has been loaded correctly. This can be toggled by setting the `DRAW_TEST_SCREEN` preprocessor directive at the top of the `src/bootloader/src/main.c` file.

## Kernel
This repository contains a minimal x86-64 kernel for testing purposes. This is located within the `src/kernel` directory. It contains a basic UART implementation suitable for testing that the kernel has been correctly loaded.

## Feedback
Feel free to direct any questions or feedback to me directly at `ajxs [at] panoptic.online`
