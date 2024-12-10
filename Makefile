# options
builddir =
mode = debug
sanitize = true
lto = false
std = gnu23
args =
target = gnu
O = 0
prefix =

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
SDL_BUILD_DIR = ${builddir}/libs/sdl
LIBS =
TRASH = ${builddir}/compile_commands.json

ifeq (${target},gnu)
CC = gcc
EXT =
ifeq (${mode},debug)
builddir = build-debug
else
builddir = build-release
endif
LIBS += ${SDL_BUILD_DIR}/libSDL3.so
else ifeq (${target},web)
sanitize = false
CC = emcc
EXT = .html
TRASH += ${builddir}/sdl3.wasm ${builddir}/sdl3.js ${builddir}/sdl3.data
LDFLAGS += --embed-file files -s FULL_ES2=1 --shell-file shell.html
ifeq (${mode},debug)
builddir = build-web-debug
LDFLAGS += --emrun
else
builddir = build-web-release
endif
LIBS += ${SDL_BUILD_DIR}/libSDL3.a
else
$(error "Unknown target: ${target}")
endif

# SDL3
CFLAGS += -I${SDL_DIR}/include
LDFLAGS += -L${SDL_BUILD_DIR} -lSDL3 -lGLESv2

ifeq (${mode},debug)
CFLAGS += -g3
LDFLAGS += -g3
SDL_CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug -DSDL_LIBC=ON
O = 0
else ifeq (${mode},release)
CFLAGS +=
LDFLAGS +=
SDL_CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Release
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
CFLAGS += -fsanitize=address,leak,undefined -fno-omit-frame-pointer \
		  -fsanitize-address-use-after-scope
LDFLAGS += -fsanitize=address,leak,undefined
LIBS += ${builddir}/libs/dlopen/libdlopen.so
TRASH += ${builddir}/libs/dlopen/libdlopen.so
DIRS += ${builddir}/libs/dlopen
prefix := LD_PRELOAD="/usr/lib/libasan.so:${builddir}/libs/dlopen/libdlopen.so" \
	ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:\
	check_initialization_order=1:strict_init_order=1 \
	LSAN_OPTIONS=suppressions=lsan.ignore ${prefix}
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

run: ${EXEC}
ifeq (${target},web)
	emrun -- $< ${args}
else
	${prefix} $< ${args}
endif

clean:
	${RM} ${EXEC} ${OBJ} ${DEP} ${TRASH}
	${foreach dir,${BUILDDIRS},if [[ -d ${dir} ]]; then rmdir ${dir}; fi${newline}}
	-if [[ -d ${builddir} ]]; then rmdir ${builddir}; fi

clean_sdl: clean
	${RM} -r ${SDL_BUILD_DIR}
	-if [[ -d ${builddir}/libs ]]; then rmdir ${builddir}/libs; fi

.PHONY: all obj bear run clean clean_sdl

${EXEC}: ${OBJ} | ${LIBS}
	${CC} -o $@ $^ ${LDFLAGS}

${builddir}/%.o: %.c
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

${SDL_BUILD_DIR}/libSDL3.so:
	cmake -S ${SDL_DIR} -B ${SDL_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	+make -C ${SDL_BUILD_DIR}

${SDL_BUILD_DIR}/libSDL3.a:
	emcmake cmake -S ${SDL_DIR} -B ${SDL_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	+emmake make -C ${SDL_BUILD_DIR}

${builddir}/libs/dlopen/libdlopen.so: libs/dlopen/dlopen.c
	@mkdir -p ${dir $@}
	${CC} -fpic --shared -o $@ $< -ldl

-include ${DEP}

