# 🧩 Modules System
### - ESP
El sistema de módulos de **System Tools** permite incluir o excluir, de forma explicita, componentes específicos de la librería, mediante directivas del preprocesador. Esto optimiza los tiempos de compilación y reduce la huella de memoria al importar únicamente las funcionalidades requeridas para el proyecto.

Para habilitar un módulo específico, se debe definir la macro correspondiente antes de incluir las cabeceras principales de la librería (`#include "System_Tools.h"`), permitiendo elegir que y como se añadirá contenido de la librería.

* `ST_NO_DEFAULT`: Indica que no se autodefinan todas las macros con patron de nombre `ST_IMPORT_...` al incluirse la libreria. Esta macro es autodefinida cuanto se detecta el uso de la macro para habilitar un módulo, con el fin de evitar que se habiliten otros modulos no especificados.
* `ST_NO_GLOBAL_FUNCTIONS`: Inhabilita la publicación en el espacio de nombres global de las funciones sin prefijo (`printf`, `sprintf`, `system`, `alloc`, `free`...). Definirla antes de incluir la cabecera es la salida cuando el fichero quiere las versiones de la crt e incluye `<stdio.h>` o `<stdlib.h>` despues de `System_Tools.h`.
* [`ST_IMPORT_JSON`](Jsons/README.md): Habilita el módulo de análisis, creación y manipulación de estructuras JSON.
* [`ST_IMPORT_FILE`](Files/README.md): Habilita el módulo de manipulación de ficheros y archivos.
* [`ST_IMPORT_SOUND`](Sounds/README.md): Habilita el módulo de manipulación y reproducción de audio.
* [`ST_IMPORT_CONSOLE`](Console/README.md): Habilita el módulo de manipulación de terminales.
* `ST_NO_GLOBAL_TYPES`: Inhabilita la publicación en el espacio de nombres global de `BYTE`, `wchar`, `WORD` y `UWORD`, que viven dentro de `System_Tools`. Definirla antes de incluir la cabecera es la salida cuando el fichero incluye `<windows.h>` despues de `System_Tools.h`, porque el SDK tambien tiene su `WORD` y su `BYTE`.
* `ST_STOP_MAILMAN_DECL`: Inhabilita la definición de la función `window::defMailMan` al usarse la macro de importación `ST_IMPORT_WINDOW`.
* [`ST_IMPORT_WINDOW`](Windows/README.md): Habilita el módulo de manipulación, creación y destrucción de ventanas gráficas.

Compatibilidades:
* Un módulo se nombra como **Unico** cuando no posee ninguna compatibilidad semantica con otros módulos.
* Un módulo se nombra como **Conjugado** cuando posee compatibilidades semanticas con otros módulos.
* Un módulo se nombra como **Dependiente** cuando su existencia depende de la inclusión de otros módulos.

| **Módulos** | `ST_IMPORT_JSON` | `ST_IMPORT_FILE` | `ST_IMPORT_SOUND` | `ST_IMPORT_CONSOLE` | `ST_IMPORT_WINDOW` |
| ----: | :----: | :----: | :----: | :----: | :----: |
| `ST_IMPORT_JSON` | ✅ | ✅ | ❌ | ❌ | ❌ |
| `ST_IMPORT_FILE` | ✅ | ✅ | ✅ | ❌ | ❌ |
| `ST_IMPORT_SOUND` | ❌ | ✅ | ✅ | ❌ | ❌ |
| `ST_IMPORT_CONSOLE` | ❌ | ✅ | ❌ | ✅ | ✅ |
| `ST_IMPORT_WINDOW` | ❌ | ✅ | ❌ | ❌ | ✅ |

### - ENG
The **System Tools** module system allows for explicit inclusion or exclusion of specific library components using preprocessor directives. This optimizes compilation times and reduces the memory footprint by importing only the functionality required for the project.

To enable a specific module, the corresponding macro must be defined before including the main library header (`#include "System_Tools.h"`), giving you full control over what and how library contents are added.

* `ST_NO_DEFAULT`: Prevents all macros following the naming pattern `ST_IMPORT_...` from being automatically defined upon including the library. This macro is auto-defined whenever any module-enabling macro is detected.
* `ST_NO_GLOBAL_FUNCTIONS`: Disables publishing the unprefixed functions (`printf`, `sprintf`, `system`, `alloc`, `free`...) into the global namespace. Defining it before the header is the way out when the file wants the crt versions and includes `<stdio.h>` or `<stdlib.h>` after `System_Tools.h`.
* [`ST_IMPORT_JSON`](Jsons/README.md): Enables the JSON structure parsing, creation, and manipulation module.
* [`ST_IMPORT_FILE`](Files/README.md): Enables the file and filesystem manipulation module.
* [`ST_IMPORT_SOUND`](Sounds/README.md): Enables the audio manipulation and playback module.
* [`ST_IMPORT_CONSOLE`](Console/README.md): Enables the terminal and console manipulation module.
* `ST_NO_GLOBAL_TYPES`: Disables publishing `BYTE`, `wchar`, `WORD` and `UWORD` into the global namespace, where they live inside `System_Tools`. Defining it before the header is the way out when the file includes `<windows.h>` after `System_Tools.h`, because the SDK has a `WORD` and a `BYTE` of its own.
* `ST_STOP_MAILMAN_DECL`: Disables the declaration of the `window::defMailMan` function when using the `ST_IMPORT_WINDOW` import macro.
* [`ST_IMPORT_WINDOW`](Windows/README.md): Enables the graphical window creation, manipulation, and destruction module.

Compatibility:
* A module is categorized as **Single** when it shares no semantic compatibility with other modules.
* A module is categorized as **Conjugated** when it features semantic compatibility with other modules.
* A module is categorized as **Dependent** when its presence relies on the inclusion of other modules.

| **Modules** | `ST_IMPORT_JSON` | `ST_IMPORT_FILE` | `ST_IMPORT_SOUND` | `ST_IMPORT_CONSOLE` | `ST_IMPORT_WINDOW` |
| ----: | :----: | :----: | :----: | :----: | :----: |
| `ST_IMPORT_JSON` | ✅ | ✅ | ❌ | ❌ | ❌ |
| `ST_IMPORT_FILE` | ✅ | ✅ | ✅ | ❌ | ❌ |
| `ST_IMPORT_SOUND` | ❌ | ✅ | ✅ | ❌ | ❌ |
| `ST_IMPORT_CONSOLE` | ❌ | ✅ | ❌ | ✅ | ✅ |
| `ST_IMPORT_WINDOW` | ❌ | ✅ | ❌ | ❌ | ✅ |