# options
builddir = build
mode = debug
asan = true
lto = false
std = gnu23
args =
O = 0

CC = gcc
CPPFLAGS = -MMD -MP -D_GNU_SOURCE
CFLAGS = ${CPPFLAGS} -std=${std} -O$O -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -O$O

# SDL3
CFLAGS += ${shell pkg-config --cflags sdl3}
LDFLAGS += ${shell pkg-config --libs sdl3}

DIRS = src
BUILDDIRS = ${addprefix ${builddir}/,${DIRS}}

SRC = ${wildcard src/*.c}
OBJ = ${SRC:%.c=${builddir}/%.o}
DEP = ${OBJ:.o=.d}
EXEC = ${builddir}/sdl3

mode = release
ifeq (${mode},debug)
CFLAGS += -ggdb3
LDFLAGS += -ggdb3
O = 0
else ifeq (${mode},release)
CFLAGS +=
LDFLAGS +=
O = 2
else
$(error "Unknown mode: ${mode}")
endif

lto = false
ifeq (${lto},true)
CFLAGS += -flto=auto -fuse-linker-plugin
LDFLAGS += -flto=auto
else ifneq (${lto},false)
${error "lto should be true or false"}
endif

sanitize = false
ifeq (${sanitize},true)
CFLAGS += -fsanitize=address,leak,undefined
LDFLAGS += -fsanitize=address,leak,undefined
else ifneq (${sanitize},false)
${error "sanitize should be true or false"}
endif

empty :=
define newline

${empty}
endef

all: ${EXEC}

obj: ${OBJ}

bear: clean
	mkdir -p ${builddir}
	bear --output ${builddir}/compile_commands.json -- make obj
	ln -s ${builddir}/compile_commands.json compile_commands.json

run: ${EXEC}
	$< ${args}

clean:
	${RM} ${EXEC} ${OBJ} ${DEP}
	${foreach dir,${BUILDDIRS},if [[ -d  ${dir} ]]; then rmdir ${dir}; fi${newline}}

.PHONY: all obj bear run clean

${EXEC}: ${OBJ}
	${CC} -o $@ $^ ${LDFLAGS}

${builddir}/%.o: %.c
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

-include ${DEP}

