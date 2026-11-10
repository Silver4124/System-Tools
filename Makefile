# System Tools: la libreria no se compila aparte (header/impl-based);
# este Makefile construye solo los ejemplos. Requiere GNU Make + g++/clang++.
# Uso:   make            -> compila todos los ejemplos en bin/
#        make clean      -> elimina bin/
CXX      ?= g++
CXXFLAGS ?= -std=c++11 -O2

# En Windows (MinGW/MSYS) no se enlaza nada extra: los simbolos del sistema
# se resuelven en tiempo de ejecucion. En Linux tampoco desde glibc 2.34, que es
# cuando dlopen y pthread_create pasan a vivir dentro de libc.so.6: ahi la libreria
# los encuentra con el mismo recorrido de la link map que ya usa get_system_function.
# Con un glibc anterior esas dos siguen en libdl.so.2 y libpthread.so.0, y como el
# unico cargador que hay dentro de libc es el propio dlopen, la libreria no puede
# mapearlas ella sola: ahi si que hay que enlazarlas.
# $(OS) solo existe en cmd.exe/PowerShell; desde un shell MSYS no esta exportado,
# asi que tambien se consulta uname.
UNAME_S := $(shell uname -s 2>/dev/null)
ifneq ($(filter MINGW% MSYS% CYGWIN%,$(UNAME_S)),)
	ON_WINDOWS := 1
endif
ifeq ($(OS),Windows_NT)
	ON_WINDOWS := 1
endif
ifeq ($(ON_WINDOWS),1)
	LDLIBS ?=
else
	# La prueba es la misma consulta que hace la libreria en tiempo de ejecucion: estar o
	# no estar el simbolo en la tabla de dinamicos de libc. Si falta nm o libc.so.6 no
	# responde, el resultado no es "yes" y se enlaza como siempre: sobran banderas, no faltan.
	LIBC_SONAME := $(shell $(CXX) -print-file-name=libc.so.6 2>/dev/null)
	LIBC_HAS_DLOPEN := $(shell nm -D --defined-only "$(LIBC_SONAME)" 2>/dev/null | grep -q dlopen && echo yes)
ifeq ($(LIBC_HAS_DLOPEN),yes)
	LDLIBS ?=
else
	LDLIBS ?= -ldl -lpthread
endif
endif

SRCS := $(wildcard examples/*/*.cpp)
# examples/counterexamples esta escrito a proposito para fallar: queda fuera del barrido.
SRCS := $(filter-out examples/counterexamples/%,$(SRCS))
BINS := $(patsubst examples/%.cpp,bin/%,$(SRCS))

.PHONY: all clean
all: $(BINS)

# El tallo de bin/<modulos>/<nombre> viene de examples/<modulos>/<nombre>.cpp:
# sin el prefijo examples/ la prereq se pide en la raiz y make no encuentra nada.
bin/%: examples/%.cpp System_Tools.h System_Tools.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I. $< -o $@ $(LDLIBS)

clean:
	rm -rf bin
