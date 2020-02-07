.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ARCH := x86_64
CC   := ${ARCH}-elf-gcc

SRC_DIR := src

INCLUDE_DIRS := ${SRC_DIR}/include
INCLUDE_FLAG := ${foreach d, ${INCLUDE_DIRS}, -I$d}

CFLAGS := -std=gnu99    \
	-ffreestanding        \
	-O2                   \
	-Wall                 \
	-Wextra

LDFLAGS := -ffreestanding    \
	-O2                        \
	-nostdlib

LIBS := -lgcc


AS_SOURCES :=

C_SOURCES  := ${SRC_DIR}/kernel.c

OBJECTS    := ${AS_SOURCES:.S=.o}
OBJECTS    += ${C_SOURCES:.c=.o}

BUILD_DIR  := build
BINARY     := ${BUILD_DIR}/kernel.elf


.PHONY: all clean

all: ${BINARY}

${BINARY}: ${BUILD_DIR} ${OBJECTS}
	${CC} -T ${SRC_DIR}/kernel.ld -g3 -o ${BINARY} ${LDFLAGS} ${OBJECTS} ${LIBS}

%.o: %.S
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

%.o: %.c
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -f ${BINARY}
