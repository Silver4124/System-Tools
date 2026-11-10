# 📚 Documentation Index
### - ESP
Este es el índice maestro de la documentación de **System Tools**. La documentación está redactada en paralelo (español/inglés) en cada documento, y cada función del proyecto posee su propio archivo con la estructura: *Firma → Parámetros → Retorno → Comportamiento*, más secciones de *Overloads* o *Notas* cuando aplican. El apartado *Retorno* se omite en las funciones `void`.

Se recomienda el siguiente orden de lectura: primero los **Conceptos Globales**, luego [**Default Functions**](Default%20Functions/README.md) (funciones que no requieren ninguna macro de importación), y finalmente el o los **Módulos** que se deseen usar y/o estudiar.

## - Conceptos Globales
| Documento | Contenido |
| ---- | ---- |
| [**Types**](Types.md) | Los 4 tipos esenciales: `BYTE`, `wchar`, `WORD`, `UWORD`, y el término `stacked`. |
| [**Macros**](Macros.md) | Macros de entorno (`WORD_SIZE`, endianness), de formato (`*_PRINT`), variádicas (`VA_*`) y de lenguaje (`RESTRICT`, `FORCEINLINE`, `ALWAYSINLINE`, `NOINLINE`). |
| [**Modules System**](Modules%20System.md) | El sistema de importación `ST_IMPORT_*`, `ST_NO_DEFAULT`, `ST_NO_GLOBAL_FUNCTIONS`, `ST_NO_GLOBAL_TYPES`, y la matriz de compatibilidad entre módulos. |
| [**C Formating**](C%20Formating.md) | Referencia de especificadores de formato de las funciones estilo C de la librería. |

## - Default Functions
Incluidas siempre, sin macros. Ver su respectivo [README](Default%20Functions/README.md).
* [**System Core**](Default%20Functions/get_system_function.md): [`get_system_function`](Default%20Functions/get_system_function.md), [`get_system_variable`](Default%20Functions/get_system_variable.md).
* [**Adaptaciones de C y E/S básicas**](Default%20Functions/README.md): `system`, `clear`, `puts`, `putchar`, `printf`, `sprintf`, `scanf`, `sscanf`, `mbstowcs`, `wcstombs`, `language`, `sleep`, `message`.
* [**Administración de la memoria**](Default%20Functions/memory/README.md): `namespace memory` (`alloc`, `clloc`, `slloc`, `mlloc`, `realloc`, `free`, `cmplloc`, `cnrlloc`, `szlen`, `on_stack`).
* [**Administración de hilos**](Default%20Functions/thread/README.md): `namespace thread` (`create`, `join`, `kill`, `yield`).

## - Módulos
Cada módulo se habilita definiendo su macro antes de incluir `System_Tools.h`. Ver la [matriz de compatibilidad](Modules%20System.md).
| Macro | Módulo | Documentación |
| ---- | ---- | ---- |
| `ST_IMPORT_JSON` | Análisis y manipulación de JSON (`namespace json`, enum [`json_type`](Jsons/Enum%20json_type.md)) | [README](Jsons/README.md) |
| `ST_IMPORT_FILE` | Archivos y sistema de ficheros (`namespace file`, enums [`file_mode`](Files/Enum%20file_mode.md), [`seek_mode`](Files/Enum%20seek_mode.md), [`time_mode`](Files/Enum%20time_mode.md)) | [README](Files/README.md) |
| `ST_IMPORT_SOUND` | Audio (`namespace sound`) | [README](Sounds/README.md) |
| `ST_IMPORT_WINDOW` | Ventanas gráficas (`namespace window`, enum [`window_box`](Windows/Enum%20window_box.md)) | [README](Windows/README.md) |
| `ST_IMPORT_CONSOLE` | Terminales (`namespace console`) | [README](Console/README.md) |

### - ENG
This is the master index of the **System Tools** documentation. Each document is written in parallel (Spanish/English), and every function in the project has its own file following the structure: *Signature → Parameters → Return → Behavior*, plus *Overloads* or *Notes* sections where applicable. The *Return* section is omitted for `void` functions.

The recommended reading order is: first the **Global Concepts**, then [**Default Functions**](Default%20Functions/README.md) (functions that require no import macro), and finally the **Module(s)** you intend to use.

## - Global Concepts
| Document | Content |
| ---- | ---- |
| [**Types**](Types.md) | The 4 essential types: `BYTE`, `wchar`, `WORD`, `UWORD`, and the `stacked` term. |
| [**Macros**](Macros.md) | Environment macros (`WORD_SIZE`, endianness), format macros (`*_PRINT`), variadic macros (`VA_*`) and language macros (`RESTRICT`, `FORCEINLINE`, `ALWAYSINLINE`, `NOINLINE`). |
| [**Modules System**](Modules%20System.md) | The `ST_IMPORT_*` import system, `ST_NO_DEFAULT`, `ST_NO_GLOBAL_FUNCTIONS`, `ST_NO_GLOBAL_TYPES`, and the module compatibility matrix. |
| [**C Formating**](C%20Formating.md) | Reference of the format specifiers used by the library's C-style functions. |

## - Default Functions
Always included, no macros required. See their [README](Default%20Functions/README.md).
* [**System Core**](Default%20Functions/get_system_function.md): [`get_system_function`](Default%20Functions/get_system_function.md), [`get_system_variable`](Default%20Functions/get_system_variable.md).
* [**C Adaptations and Basic I/O**](Default%20Functions/README.md): `system`, `clear`, `puts`, `putchar`, `printf`, `sprintf`, `scanf`, `sscanf`, `mbstowcs`, `wcstombs`, `language`, `sleep`, `message`.
* [**Memory Management**](Default%20Functions/memory/README.md): `namespace memory` (`alloc`, `clloc`, `slloc`, `mlloc`, `realloc`, `free`, `cmplloc`, `cnrlloc`, `szlen`, `on_stack`).
* [**Thread Management**](Default%20Functions/thread/README.md): `namespace thread` (`create`, `join`, `kill`, `yield`).

## - Modules
Each module is enabled by defining its macro before including `System_Tools.h`. See the [compatibility matrix](Modules%20System.md).
| Macro | Module | Documentation |
| ---- | ---- | ---- |
| `ST_IMPORT_JSON` | JSON parsing and manipulation (`namespace json`, enum [`json_type`](Jsons/Enum%20json_type.md)) | [README](Jsons/README.md) |
| `ST_IMPORT_FILE` | Files and filesystem (`namespace file`, enums [`file_mode`](Files/Enum%20file_mode.md), [`seek_mode`](Files/Enum%20seek_mode.md), [`time_mode`](Files/Enum%20time_mode.md)) | [README](Files/README.md) |
| `ST_IMPORT_SOUND` | Audio (`namespace sound`) | [README](Sounds/README.md) |
| `ST_IMPORT_WINDOW` | Graphical windows (`namespace window`, enum [`window_box`](Windows/Enum%20window_box.md)) | [README](Windows/README.md) |
| `ST_IMPORT_CONSOLE` | Terminals (`namespace console`) | [README](Console/README.md) |
