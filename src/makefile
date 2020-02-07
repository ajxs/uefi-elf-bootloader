#####################################################################
#  Copyright (c) 2019, AJXS.
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the
#  Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  Authors:
#     Anthony <ajxs [at] panoptic.online>
#####################################################################

.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

KERNEL_DIR        := kernel
KERNEL_BINARY     := ${KERNEL_DIR}/build/kernel.elf

BOOTLOADER_DIR    := bootloader
BOOTLOADER_BINARY := ${BOOTLOADER_DIR}/build/bootx64.efi

BUILD_DIR         := ../build
DISK_IMG          := ${BUILD_DIR}/kernel.img
DISK_IMG_SIZE     := 2880

QEMU_FLAGS :=                                                \
	-bios OVMF.fd                                              \
	-drive if=none,id=uas-disk1,file=${DISK_IMG},format=raw    \
	-device usb-storage,drive=uas-disk1                        \
	-serial stdio                                              \
	-usb                                                       \
	-vga std

.PHONY: all clean emu

all: ${DISK_IMG}

bootloader: ${BOOTLOADER_BINARY}

debug: ${DISK_IMG}
	qemu-system-x86_64    \
		${QEMU_FLAGS}       \
		-S                  \
		-gdb tcp::1234

emu: ${DISK_IMG}
	qemu-system-x86_64    \
		${QEMU_FLAGS}

kernel: ${KERNEL_BINARY}

${DISK_IMG}: ${BUILD_DIR} ${BOOTLOADER_BINARY} ${KERNEL_BINARY}
	# Create UEFI boot disk image in DOS format.
	dd if=/dev/zero of=${DISK_IMG} bs=1k count=${DISK_IMG_SIZE}
	mformat -i ${DISK_IMG} -f ${DISK_IMG_SIZE} ::
	mmd -i ${DISK_IMG} ::/EFI
	mmd -i ${DISK_IMG} ::/EFI/BOOT
	# Copy the bootloader to the boot partition.
	mcopy -i ${DISK_IMG} ${BOOTLOADER_BINARY} ::/efi/boot/bootx64.efi
	mcopy -i ${DISK_IMG} ${KERNEL_BINARY} ::/kernel.elf

${BOOTLOADER_BINARY}:
	make -C ${BOOTLOADER_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${KERNEL_BINARY}:
	make -C ${KERNEL_DIR}

clean:
	make clean -C ${BOOTLOADER_DIR}
	make clean -C ${KERNEL_DIR}
	rm -f ${DISK_IMG}
	rm -rf ${BUILD_DIR}
