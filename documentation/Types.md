# 🔢 Types
### - ESP
System Tools proporciona 4 tipos de uso esencial para el desarrollador; entenderlos proporcionará el suficiente conocimiento para entender más de la mitad de las funciones dentro de la librería.\
Los cuatro viven dentro de `namespace System_Tools` y la librería los publica además sin prefijo mientras el SDK de Windows no esté en la unidad; con `<windows.h>` incluido, o definiendo `ST_NO_GLOBAL_TYPES`, hay que escribir `System_Tools::UWORD`.
### - ENG
System Tools provides 4 essential usage types for the developer; understanding them will provide enough knowledge to understand more than half of the functions within the library.\
All four live inside `namespace System_Tools`, and the library also publishes them unprefixed while the Windows SDK is not in the unit; with `<windows.h>` included, or with `ST_NO_GLOBAL_TYPES` defined, they must be written as `System_Tools::UWORD`.

## BYTE
### - ESP
Tipo de dato numérico del tamaño de un único byte sin signo. Utilizado en funciones que requieran operabilidad bit a bit o como tipo de dato de longitud mínima para un argumento.
### - ENG
Numeric data type with the size of a single unsigned byte. Used in functions requiring bitwise operations or as a minimum-length data type for an argument.

## wchar
### - ESP
Tipo de dato textual, que es utilizado para la representación de caracteres largos en codificación UTF, su longitud es variable dependiendo de como el sistema implementa el procesamiento de caracteres largos. Es utilizado como un sustituto semantico del tipo `wchar_t`. Utilizado en funciones que requieran operabilidad semantica relacionada a texto.
### - ENG
Textual data type, which is used for the representation of long characters in UTF encoding, its length is variable depending on how the system implements the wide-character processing. It serves as a semantic substitute for the `wchar_t` type and is used in functions requiring text-related semantic operations.

## WORD
### - ESP
Tipo de dato numérico del tamaño de la palabra del procesador del sistema, con signo. Utilizado como tipo de uso general o para funciones que requieran operabilidad al tamaño de la palabra del procesador con signo, operabilidad y transformación entre tipos (sobre todo para aquellos que necesiten conservar el signo), representación de longitudes relativas entre bytes, enumeradores y prescripción de tipos enteros con signo. \
Para consultar su tamaño exacto en bytes se puede usar la macro `WORD_SIZE` o `sizeof(WORD)`.\
Su contraparte es `UWORD`.
### - ENG
Signed numeric data type with the size of the system processor word. It's used as a general-purpose type or for functions requiring operability at the signed processor word size, type conversions (especially those preserving the sign), representing relative lengths in bytes, enumerators, and specifying signed integer types. \
To inspect its exact byte size, use the `WORD_SIZE` macro or `sizeof(WORD)`.\
Its counterpart is `UWORD`.

## UWORD
### - ESP
Tipo de dato numérico del tamaño de la palabra del procesador del sistema, sin signo. Utilizado como tipo de uso general o para funciones que requieran operabilidad al tamaño de la palabra del procesador, operabilidad y transformación entre tipos (sobre todo punteros), representación de longitudes absolutas entre bytes, arreglos, memoria y prescripción de tipos enteros sin signo. \
Para consultar su tamaño exacto en bytes se puede usar la macro `WORD_SIZE` o `sizeof(UWORD)`.\
Su contraparte es `WORD`.
### - ENG
Unsigned numeric data type with the size of the system processor word. It's used as a general-purpose type or for functions requiring operability at the unsigned processor word size, type conversions (especially pointers and integers), representing absolute lengths in bytes, enumerators, and specifying unsigned integer types. \
To inspect its exact byte size, use the `WORD_SIZE` macro or `sizeof(UWORD)`.\
Its counterpart is `WORD`.

## The `stacked` term
### - ESP
Varias funciones de conversión y lectura aceptan un argumento o sobrecarga llamada `stacked` (`mbstowcs`, `wcstombs`, `window::keyboard`). Es una convención heredada del autor y **no** hace referencia a la pila de ejecución: `stacked = true` reserva el buffer resultante con los asignadores de C++ (`new`), mientras que `stacked = false` lo reserva con los asignadores de C.\
La diferencia observable está en cómo se libera: lo obtenido con `stacked = true` se libera con `delete`, y lo obtenido con `stacked = false` se libera con `memory::free`. Como el régimen de liberación lo decide un argumento en tiempo de ejecución, no debe inferirse del tipo del puntero.\
Este término no debe confundirse con `memory::on_stack`, que si consulta el origen real de una dirección: devuelve `true` únicamente cuando el puntero pertenece al espacio de la pila del programa.
### - ENG
Several conversion and reading functions accept an argument or overload named `stacked` (`mbstowcs`, `wcstombs`, `window::keyboard`). It is a naming convention inherited from the author and does **not** refer to the execution stack: `stacked = true` reserves the resulting buffer with C++ allocators (`new`), while `stacked = false` reserves it with C allocators.\
The observable difference lies in how it is released: what you obtained with `stacked = true` must be freed with `delete`, and what you obtained with `stacked = false` must be freed with `memory::free`. Since the release regime is decided by a runtime argument, it must not be inferred from the pointer's type.\
This term must not be confused with `memory::on_stack`, which does query the real origin of an address: it returns `true` only when the pointer belongs to the program's stack space.