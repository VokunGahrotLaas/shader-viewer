# options
builddir =
mode = debug
sanitize = true
lto = false
std =
args =
target = gnu
O = 0
prefix =

empty :=
define newline

${empty}
endef

CPPFLAGS =
CFLAGS =
LDFLAGS =

DIRS = src
BUILDDIRS = ${addprefix ${builddir}/,${DIRS}}

SRC = ${subst /,${SEP},${wildcard src/*.c}}
OBJ = ${SRC:%.c=${builddir}${SEPSH}%${OBJEXT}}
DEP = ${OBJ:${OBJEXT}=.d}
EXEC = ${builddir}${SEP}shader-viewer${OUTEXT}

SDL_DIR = libs${SEP}SDL
SDL_BUILD_DIR = ${builddir}${SEP}libs${SEP}SDL
SDL_TTF_DIR = libs${SEP}SDL_ttf
SDL_TTF_BUILD_DIR = ${builddir}${SEP}libs${SEP}SDL_ttf
LIBS =
DLLS =
TRASH = ${builddir}${SEP}compile_commands.json

ifeq (${target},gnu)
CC = gcc
OUTEXT =
ifeq (${mode},debug)
builddir = build-gnu-debug
else
builddir = build-gnu-release
endif
LIBS += ${SDL_BUILD_DIR}/libSDL3.so ${SDL_TTF_BUILD_DIR}/libSDL3_ttf.so
prefix := LD_LIBRARY_PATH="${SDL_BUILD_DIR}:${SDL_TTF_BUILD_DIR}" ${prefix}

else ifeq (${target},msvc)
sanitize = false
lto = false
CC = cl.exe /nologo
OUTEXT = .exe
ifeq (${mode},debug)
builddir = build-msvc-debug
LIBS += ${SDL_BUILD_DIR}${SEP}Debug${SEP}SDL3.dll ${SDL_TTF_BUILD_DIR}${SEP}Debug${SEP}SDL3_ttf.dll
else
builddir = build-msvc-release
LIBS += ${SDL_BUILD_DIR}${SEP}Release${SEP}SDL3.dll ${SDL_TTF_BUILD_DIR}${SEP}Release${SEP}SDL3_ttf.dll
endif

else ifeq (${target},web)
sanitize = false
CC = emcc
OUTEXT = .html
TRASH += ${builddir}/sdl3.wasm ${builddir}/sdl3.js ${builddir}/sdl3.data
LDFLAGS += --embed-file files -s FULL_ES2=1 --shell-file shell.html
ifeq (${mode},debug)
builddir = build-web-debug
LDFLAGS += --emrun
else
builddir = build-web-release
endif
LIBS += ${SDL_BUILD_DIR}/libSDL3.a ${SDL_TTF_BUILD_DIR}/libSDL3_ttf.a

else
$(error "Unknown target: ${target}")
endif

# flags
ifeq (${target},msvc)
std = clatest
SEP = \${empty}
SEPSH = \\
OBJEXT = .obj
OUTARG = /Fe:
OBJARG = /c /Fo:
PDB = ${builddir}${SEP}shader-viewer.pdb
TRASH += ${PDB}
RM = erase

CPPFLAGS +=
CFLAGS += ${CPPFLAGS} /std:${std} /O$O
LDFLAGS += /O$O

ifeq (${mode},debug)
CFLAGS += /Zi /Fd${PDB}
LDFLAGS += /Zi /Fd${PDB}
SDL_CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug
O = d
else ifeq (${mode},release)
CFLAGS +=
LDFLAGS +=
SDL_CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Release
O = 2
else
$(error "Unknown mode: ${mode}")
endif

# link.exe args
LDFLAGS += /link /incremental:no /subsystem:windows

# SDL3
CPPFLAGS += /I${SDL_DIR}${SEP}include /I${SDL_TTF_DIR}${SEP}include
ifeq (${mode},debug)
LDFLAGS += /libpath:${SDL_BUILD_DIR}/Debug /libpath:${SDL_TTF_BUILD_DIR}/Debug
else
LDFLAGS += /libpath:${SDL_BUILD_DIR}/Release /libpath:${SDL_TTF_BUILD_DIR}/Release
endif
LDFLAGS += /libpath:libs${SEP}angle
DLLS += ${builddir}${SEP}SDL3.dll ${builddir}${SEP}SDL3_ttf.dll

else # gnu-like
std = gnu23
SEP = /
SEPSH = /
OBJEXT = .o
OUTARG = -o
OBJARG = -c -o

CPPFLAGS += -MMD -MP -D_GNU_SOURCE
CFLAGS += ${CPPFLAGS} -std=${std} -O$O -Wall -Wextra -Wpedantic -Werror
LDFLAGS += -O$O

#SDL3
CPPFLAGS += -I${SDL_DIR}/include -I${SDL_TTF_DIR}/include
LDFLAGS += -L${SDL_BUILD_DIR} -L${SDL_TTF_BUILD_DIR} -lSDL3 -lSDL3_ttf -lGLESv2

ifeq (${mode},debug)
CFLAGS += -g3
LDFLAGS += -g3
SDL_CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug
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
endif

all: ${EXEC}

obj: ${OBJ}

bear: clean
	mkdir -p ${builddir}
	bear --output ${builddir}/compile_commands.json -- make obj

run: ${EXEC} ${DLLS}
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
	${RM} -r ${SDL_TTF_BUILD_DIR}
	-if [[ -d ${builddir}/libs ]]; then rmdir ${builddir}/libs; fi

.PHONY: all obj bear run clean clean_sdl

${EXEC}: ${OBJ} | ${LIBS}
	${CC} ${OUTARG} $@ $^ ${LDFLAGS}

${builddir}${SEPSH}%${OBJEXT}: %.c
ifeq (${target},gnu)
	mkdir -p ${dir $@}
else ifeq (${target},msvc)
	if not exist ${dir $@}${SEP} mkdir ${dir $@}
endif
	${CC} ${CFLAGS} ${OBJARG} $@ $<

${builddir}${SEP}SDL3.dll: ${SDL_BUILD_DIR}${SEP}Debug${SEP}SDL3.dll
	copy $< $@

${builddir}${SEP}SDL3_ttf.dll: ${SDL_TTF_BUILD_DIR}${SEP}Debug${SEP}SDL3_ttf.dll
	copy $< $@

${SDL_BUILD_DIR}${SEP}Debug${SEP}SDL3.dll ${SDL_BUILD_DIR}${SEP}Release${SEP}SDL3.dll:
	cmake -S ${SDL_DIR} -B ${SDL_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	cmake --build ${SDL_BUILD_DIR}

${SDL_BUILD_DIR}${SEP}libSDL3.so:
	cmake -S ${SDL_DIR} -B ${SDL_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	+cmake --build ${SDL_BUILD_DIR}

${SDL_BUILD_DIR}/libSDL3.a:
	emcmake cmake -S ${SDL_DIR} -B ${SDL_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	+emmake make -C ${SDL_BUILD_DIR}

${SDL_TTF_BUILD_DIR}${SEP}Debug${SEP}SDL3_ttf.dll ${SDL_TTF_BUILD_DIR}${SEP}Release${SEP}SDL3_ttf.dll:
	cmake -S ${SDL_TTF_DIR} -B ${SDL_TTF_BUILD_DIR} ${SDL_CMAKE_FLAGS} -DSDL3_DIR=${abspath ${SDL_BUILD_DIR}} -DSDLTTF_VENDORED=True
	cmake --build ${SDL_TTF_BUILD_DIR}

${SDL_TTF_BUILD_DIR}/libSDL3_ttf.so: | ${SDL_BUILD_DIR}/libSDL3.so
	cmake -S ${SDL_TTF_DIR} -B ${SDL_TTF_BUILD_DIR} ${SDL_CMAKE_FLAGS}
	+cmake --build ${SDL_TTF_BUILD_DIR}

${SDL_TTF_BUILD_DIR}/libSDL3_ttf.a: | ${SDL_BUILD_DIR}/libSDL3.a
	#cd ${SDL_TTF_DIR}; ./external/download.sh
	emcmake cmake -S ${SDL_TTF_DIR} -B ${SDL_TTF_BUILD_DIR} ${SDL_CMAKE_FLAGS} -DSDL3_DIR="${PWD}/${SDL_BUILD_DIR}" -DSDLTTF_VENDORED=True -DBUILD_SHARED_LIBS=OFF
	+emmake make -C ${SDL_TTF_BUILD_DIR}

${builddir}/libs/dlopen/libdlopen.so: libs/dlopen/dlopen.c
	@mkdir -p ${dir $@}
	${CC} -fpic --shared -o $@ $< -ldl

-include ${DEP}

