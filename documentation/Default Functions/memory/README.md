# 📦 Memory Management
### - ESP
Este apartado abarca todo el conjunto de funciones de System Tools pertenecientes al `namespace memory`, encargadas de la manipulación y gestión directa de la memoria. Al ser una funcionalidad fundamental en el desarrollo de software, este módulo se incluye de forma nativa dentro de [**Default Functions**](../README.md) en la librería.\
Para la lectura de esta sección, se asume que el lector posee conocimientos previos sobre el manejo de punteros y memoria en C/C++, por lo que no se profundizará en dichos conceptos teóricos a fin de mantener la brevedad y concisión del documento.

A continuación se enlistan las funciones disponibles en este módulo:
* [`alloc`](alloc.md): Reemplazo de la función `malloc`.
* [`clloc`](clloc.md): Reemplazo de la función `memcpy`.
* [`slloc`](slloc.md): Reemplazo de la función `memset`.
* [`mlloc`](mlloc.md): Reemplazo de la función `memmove`.
* [`realloc`](realloc.md).
* [`free`](free.md).
* [`cmplloc`](cmplloc.md): Reemplazo booleano de la función `memcmp`.
* [`cnrlloc`](cnrlloc.md): Reemplazo ternario de la función `memcmp`.
* [`szlen`](szlen.md).
* [`on_stack`](on_stack.md).


### - ENG
This section covers all System Tools functions belonging to the `memory` namespace, designed for direct memory management and manipulation. As a core requirement for application development, this module is natively included alongside the rest of the [**Default Functions**](../README.md) within the library.\
This documentation assumes prior familiarity with C and C++ pointer mechanics and memory management; therefore, foundational concepts will not be reviewed in order to keep the documentation concise.

Below is the list of available functions in this module:
* [`alloc`](alloc.md): Replacement for the `malloc` function.
* [`clloc`](clloc.md): Replacement for the `memcpy` function.
* [`slloc`](slloc.md): Replacement for the `memset` function.
* [`mlloc`](mlloc.md): Replacement for the `memmove` function.
* [`realloc`](realloc.md).
* [`free`](free.md).
* [`cmplloc`](cmplloc.md): Boolean replacement for the `memcmp` function.
* [`cnrlloc`](cnrlloc.md): Ternary replacement for the `memcmp` function.
* [`szlen`](szlen.md).
* [`on_stack`](on_stack.md).