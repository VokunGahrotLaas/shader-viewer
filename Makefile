# options
builddir = build
mode = debug
sanitize = false
lto = false
std = gnu23
args =
target = gnu
O = 0

CC = gcc
CPPFLAGS = -MMD -MP -D_GNU_SOURCE
CFLAGS = ${CPPFLAGS} -std=${std} -O$O -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -O$O

DIRS = src
BUILDDIRS = ${addprefix ${builddir}/,${DIRS}}

SRC = ${wildcard src/*.c}
OBJ = ${SRC:%.c=${builddir}/%.o}
DEP = ${OBJ:.o=.d}
EXEC = ${builddir}/sdl3${EXT}

SDL_DIR = libs/sdl
LIBS =
TRASH = ${builddir}/compile_commands.json

ifeq (${target},gnu)
CC = gcc
EXT =
# SDL3
CFLAGS += ${shell pkg-config --cflags sdl3}
LDFLAGS += ${shell pkg-config --libs sdl3}
else ifeq (${target},web)
CC = emcc
EXT = .html
# SDL3
CFLAGS += -I${SDL_DIR}/include
LIBS += ${SDL_DIR}/build/libSDL3.a
TRASH += ${builddir}/sdl3.wasm ${builddir}/sdl3.js
else
$(error "Unknown target: ${target}")
endif

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

ifeq (${lto},true)
CFLAGS += -flto=auto -fuse-linker-plugin
LDFLAGS += -flto=auto
else ifneq (${lto},false)
${error "lto should be true or false"}
endif

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
	${RM} ${EXEC} ${OBJ} ${DEP} ${TRASH}
	${foreach dir,${BUILDDIRS},if [[ -d ${dir} ]]; then rmdir ${dir}; fi${newline}}
	if [[ -d ${builddir} ]]; then rmdir ${builddir}; fi

clean_sdl:
	${RM} -r ${SDL_DIR}/build

.PHONY: all obj bear run clean clean_sdl

${EXEC}: ${OBJ} ${LIBS}
	${CC} -o $@ $^ ${LDFLAGS}

${builddir}/%.o: %.c
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

${SDL_DIR}/build/libSDL3.a:
	emcmake cmake -S ${SDL_DIR} -B ${SDL_DIR}/build
	emmake make -C ${SDL_DIR}/build

-include ${DEP}

