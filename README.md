# uefi-elf-bootloader

This repository contains a simple UEFI ELF bootloader which loads a simple demonstration kernel. It provides an extremely basic example implementation of a UEFI bootloader for a bare-metal x86-64 system, though this example should be portable to other architectures.

The aim of this repository is to serve as a basic teachable example of how to implement a basic UEFI bootloader.

## Build instructions
This bootloader assumes a GCC cross-compiler toolchain targeting the bare-metal `x86_64-elf` architecture. Instructions for building and obtaining a valid cross-compiler toolchain can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler).

This bootloader can be built simply by running `make` within the `src` directory. This will create the `build/kernel.img` file, which is a bootable disk image containing the bootloader loading a demonstration kernel. There is a `run` script within the root directory containing a script for testing the bootloader/kernel combination using QEMU.

### Build dependencies
- GNU Make
- GNU EFI
- An `x86_64-elf-gcc` cross-compiler toolchain present in `PATH`

## Bootloader
The bootloader component of this repository, contained within the `src/bootloader` directory,  contains the basic implementation of a UEFI ELF bootloader for the x86-64 platform. The bootloader is hardcoded to load a bare-metal x86-64 application located at `/kernel.elf` on the boot media.

The bootloader will output debugging information over the system's serial port, if present. Otherwise VGA output will be used.

The bootloader passes a `Kernel_Boot_Info` struct to the loaded kernel containing basic system information, such as the memory map.

## Kernel
This repository contains a minimal x86-64 kernel for testing purposes. This is located within the `src/kernel` directory. It contains a basic UART implementation suitable for testing that the kernel has been correctly loaded.

## Feedback
Feel free to direct any questions or feedback to me directly at `ajxs [at] panoptic.online`
