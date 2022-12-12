.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ARCH := x86_64
CC   := ${ARCH}-elf-gcc

BUILD_DIR  := build
SRC_DIR    := src

EFI_INC_DIR  := /usr/include/efi
INCLUDE_DIRS := ${EFI_INC_DIR}    \
	${EFI_INC_DIR}/${ARCH}          \
	${EFI_INC_DIR}/protocol         \
	${SRC_DIR}/include

# The full list of includes in correct format for gcc.
INCLUDE_FLAG := $(foreach d, $(INCLUDE_DIRS), -I$d)


CFLAGS := ${INCLUDE_FLAG}   \
	-ffreestanding            \
	-fno-common               \
	-fno-stack-protector      \
	-fpic                     \
	-fshort-wchar             \
	-mno-red-zone             \
	-Wall                     \
	-Wextra                   \
	-Wmissing-prototypes      \
	-Wstrict-prototypes       \
	-DEFI_FUNCTION_WRAPPER

LIB          := /usr/lib
EFI_LIB      := /usr/lib
EFI_CRT_OBJS := ${EFI_LIB}/crt0-efi-${ARCH}.o
EFI_LDS      := ${EFI_LIB}/elf_${ARCH}_efi.lds
LDFLAGS      := -nostdlib    \
	-znocombreloc              \
	-T ${EFI_LDS}              \
	-shared                    \
	-Bsymbolic                 \
	-L ${EFI_LIB}              \
	-L ${LIB} ${EFI_CRT_OBJS}


C_SOURCES := ${SRC_DIR}/elf.c    \
	${SRC_DIR}/error.c             \
	${SRC_DIR}/fs.c                \
	${SRC_DIR}/graphics.c          \
	${SRC_DIR}/loader.c            \
	${SRC_DIR}/main.c              \
	${SRC_DIR}/serial.c

AS_SOURCES :=


OBJECTS := ${C_SOURCES:.c=.o}
OBJECTS += ${AS_SOURCES:.S=.o}

# The bootloader is initially compiled into a shared lib in ELF format. GNU-EFI links
# the lib in a special format as to be easily copied into a PE32+ compatible executable
# suitable for use as a UEFI bootloader.
BINARY_ELF := ${BUILD_DIR}/bootx64.so
# This is the ELF shared lib copied into the PE32+ format.
BINARY_EFI := ${BUILD_DIR}/bootx64.efi


.PHONY: all clean emu

all: ${BINARY_EFI}

${BINARY_EFI}: ${BINARY_ELF}
	objcopy -j .text          \
		-j .sdata               \
		-j .data                \
		-j .dynamic             \
		-j .dynsym              \
		-j .rel                 \
		-j .rela                \
		-j .reloc               \
		--target=efi-app-${ARCH} $^ $@

${BINARY_ELF}: ${OBJECTS} ${BUILD_DIR}
	ld ${LDFLAGS} ${OBJECTS} -o $@ -lefi -lgnuefi

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

%.o: %.S
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -f ${BINARY_ELF}
	rm -f ${BINARY_EFI}
	rm -rf ${BUILD_DIR}
