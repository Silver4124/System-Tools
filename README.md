# 🛠️ System Tools v1.5
**The Smallest, Modular, and Dependency-Free System Library for C++**

## Description
### - ESP:
System Tools es una librería enfocada en la inclusión modular de características minimas de la ejecución de cáracteristicas del sistema. Estas están abstraídas de tal forma que, el código escrito en una versión, pueda ser altamente portable a otras plataformas. Su estructura de diseño y uso está pensada para el aprendizaje y la facilidad de uso.

Se recomienda leer primero [**Default Functions**](documentation/Default%20Functions/README.md) en la documentación para tener una idea de las funcionalidades base de la libreria.
### - ENG:
System Tools is a library focused on the modular inclusion of essential system-level features. These features are abstracted in such a way that code written for one version remains highly portable across platforms. Its design and usage structure are intended to facilitate learning and ease of use.

It is recommended to first read [**Default Functions**](documentation/Default%20Functions/README.md) in the documentation to get an idea of the basic functionalities of the library.
## Characteristics
### - ESP:
* **Cero Dependencias Externas**: Carga y resolución dinámica de símbolos y funciones del sistema, con el objetivo de no depender de código externo.
* **Multiplataforma Nativa**: Código idéntico para Windows y Linux.
* **Sistema de Módulos**: Solo compila lo que necesitas, mediante la definición manual previa de macros con el formato: `ST_IMPORT_*`.
### - ENG:
* **Zero External Dependencies**: Dynamic loading and resolution of system symbols and functions, aiming to avoid reliance on external code.
* **Native Cross-Platform Support**: Identical code for Windows and Linux.
* **Module System**: Compiles only what you need, based on the manual pre-definition of macros using the format: `ST_IMPORT_*`.

## Minimum Requirements
* **Minimum Supported OS**: Windows 7 or Any POSIX-compliant Linux distribution.
* **Minimum RAM**: 1GB.
* **Processor Architecture**: Any.
* **Native Word Size**: 32 or 64 bits.
* **Compilation Requirements**: Do not need include C/C++ standard libraries.
* **Cplusplus compile version**: ≥ C++11

## 📦 How To Install / Cómo se instala
### - ESP:
La librería no se compila aparte: es *header/impl-based*, todo su código vive en dos ficheros.

1. Copiad `System_Tools.h` y `System_Tools.cpp` **juntos, en el mismo directorio**. El encabezado incluye la implementación, así que separarlos rompe la inclusión. También vale clonar el repositorio y apuntar la inclusión a su raíz.
2. Poned ese directorio en la ruta de includes (`-I` en g++/clang++, *Additional Include Directories* en MSVC, o `add_subdirectory` con el `CMakeLists.txt` que trae el repositorio).
3. Incluíd la cabecera y listo:

```cpp
#define ST_IMPORT_FILE          // opcional: sin macro alguna se cargan todos los módulos
#include "System_Tools.h"
using namespace System_Tools;

int main(){
    void* cfg = file::open("config.txt", file_mode::read);
    if(cfg){ file::close(cfg); }
    return 0;
}
```

Notas:
* Definir cualquier `ST_IMPORT_*` desactiva los módulos que no mencionéis; no definir ninguna los activa todos. Está explicado en [Modules System](documentation/Modules%20System.md).
* En Windows no hay que enlazar ninguna librería: los símbolos del sistema se resuelven en tiempo de ejecución. En Linux tampoco desde glibc 2.34, que es cuando `dlopen` y `pthread_create` pasan a vivir dentro de `libc.so.6` y `get_system_function` los encuentra justo ahí; con un glibc anterior enlazad `-ldl -lpthread` si usáis `get_system_function`, los módulos o `thread`.
* Solo hace falta `-std=c++11` (o superior). No se incluye ninguna cabecera de C ni de C++.
### - ENG:
The library is not compiled on its own: it is header/impl-based, all its code lives in two files.

1. Copy `System_Tools.h` and `System_Tools.cpp` **together, into the same directory**. The header includes the implementation, so splitting them breaks the inclusion. Cloning the repository and pointing the include at its root works just as well.
2. Put that directory on your include path (`-I` with g++/clang++, *Additional Include Directories* with MSVC, or `add_subdirectory` through the `CMakeLists.txt` shipped here).
3. Include the header and you are done:

```cpp
#define ST_IMPORT_FILE          // optional: with no macro at all, every module gets loaded
#include "System_Tools.h"
using namespace System_Tools;

int main(){
    void* cfg = file::open("config.txt", file_mode::read);
    if(cfg){ file::close(cfg); }
    return 0;
}
```

Notes:
* Defining any `ST_IMPORT_*` turns off the modules it does not mention; defining none turns every module on. It is explained in [Modules System](documentation/Modules%20System.md).
* On Windows nothing extra has to be linked: system symbols are resolved at runtime. On Linux the same is true from glibc 2.34 up, when `dlopen` and `pthread_create` move inside `libc.so.6` and `get_system_function` finds them right there; with an older glibc, link `-ldl -lpthread` if you use `get_system_function`, the modules or `thread`.
* All it takes is `-std=c++11` (or newer). No C or C++ header is required.

## 🧪 Building The Examples / Compilar los ejemplos
### - ESP:
En [`examples/`](examples/) hay un ejemplo por tema, comentado en español e inglés. Cada carpeta lleva un `System_Tools.h` de treinta bytes que reenvía al de la raíz, así que cualquier ejemplo se compila desde su propia carpeta, sin banderas de include:

```sh
cd examples/file
g++ -std=c++11 -O2 write_read.cpp -o write_read
```

Desde la raíz del repositorio, con GNU Make:

```sh
make          # todos los ejemplos, volcados en bin/
make clean    # borra bin/
```

O con CMake:

```sh
cmake -S . -B build
cmake --build build
```

`make` y CMake dejan fuera [`examples/counterexamples/`](examples/counterexamples/README.md) a propósito: ahí viven los ejemplos escritos para fallar — unos no compilan, otros petan en tiempo de ejecución— y su README explica qué demuestra cada uno.
### - ENG:
Under [`examples/`](examples/) there is one example per topic, commented in Spanish and English. Each folder holds a thirty-byte `System_Tools.h` that forwards to the root one, so any example compiles from inside its own folder with no include flags:

```sh
cd examples/file
g++ -std=c++11 -O2 write_read.cpp -o write_read
```

From the repository root, with GNU Make:

```sh
make          # every example, dropped into bin/
make clean    # deletes bin/
```

Or with CMake:

```sh
cmake -S . -B build
cmake --build build
```

`make` and CMake leave [`examples/counterexamples/`](examples/counterexamples/README.md) out on purpose: that folder holds the examples written to fail — some never compile, others crash at runtime — and its README spells out what each one proves.

## 🔗 More
* [Documentation index](documentation/README.md)
* [Examples](examples/) · [Counterexamples](examples/counterexamples/README.md)
* [CHANGELOG](CHANGELOG.md)
* [LICENSE](LICENSE)
