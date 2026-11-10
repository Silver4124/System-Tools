# 🔤 C Formatting
### - ESP
En System Tools, las funciones formateadas (como `printf`, `scanf`, `sprintf`, `sscanf`, entre otras) siguen el estándar tradicional de cadenas de formato estilo C. Además, la librería proporciona macros semánticas (`WORD_PRINT`, `UWORD_PRINT`, `CWORD_PRINT`, etc.) que generan las secuencias que siguien este formato.  
A continuación, se detalla la sintaxis, estructura, modificadores y especificadores compatibles para la manipulación y formateo de texto dentro de la librería.\
Un matiz importante: System Tools no escribe un motor de formato propio. Las funciones `printf`, `sprintf`, `scanf` y `sscanf` reenvían el `va_list` a la runtime C del sistema (`msvcrt.dll` o la UCRT en Windows, `libc` en Linux), así que lo que sigue no es un subconjunto elegido por esta librería sino el contrato de esa runtime. Lo que aquí se documenta es el mínimo común que puede asumirse portable; constructos exóticos (`%a`/`%A`, `%n`, conjuntos `%[...]`, modificadores `z`/`t`) dependen de la versión concreta de la runtime destino y deben verificarse en ella.
### - ENG
In System Tools, formatted functions (such as `printf`, `scanf`, `sprintf`, `sscanf`, among others) follow the traditional C-style format string standard. Furthermore, the library provides semantic macros (`WORD_PRINT`, `UWORD_PRINT`, `CWORD_PRINT`, etc.) that generate the appropriate sequences to follow this format.  
Below is the detailed syntax, structure, modifiers, and specifiers supported for text manipulation and formatting within the library.\
One important nuance: System Tools does not ship its own formatting engine. The `printf`, `sprintf`, `scanf` and `sscanf` functions forward the `va_list` to the system C runtime (`msvcrt.dll` or the UCRT on Windows, `libc` on Linux), so what follows is not a subset chosen by this library but the contract of that runtime. What is documented here is the minimum common ground that can be assumed portable; exotic constructs (`%a`/`%A`, `%n`, `%[...]` sets, the `z`/`t` modifiers) depend on the specific runtime version of your target and must be verified there.

## Structure of a Format Specifier
### - ESP
Un especificador de formato sigue la siguiente estructura general:
```
%[flags][width][.precision][length]specifier
```
Los elementos entre corchetes son opcionales y modifican la apariencia y/o el tamaño del valor a formatear.
### - ENG
A format specifier for output strings (printf, sprintf) follows this general structure:
```
%[flags][width][.precision][length]specifier
```
Elements inside brackets are optional and modify the appearance and/or size of the formatted value.

## Conversion Specifiers
### - ESP
El especificador determina cómo se interpretará y mostrará el argumento correspondiente.
| Especificador | Descripción | Tipo/s Ideales | Valor de Ejemplo | Ejemplo visual de Salida |
| :---- | :---- | :---- | :---- | :---- |
| `%c` | Carácter individual en formato ASCII | `char`, `BYTE` | `'A'`/`65` | `A` |
| `%s` | Cadena de texto en formato ASCII, terminada en un carácter nulo | `const char*` | `"System Tools"` | `System Tools` |
| `%d` / `%i` | Entero decimal con signo | `int` | `-42` | `-42` |
| `%u` | Entero decimal sin signo | `unsigned int` | `42` | `42` |
| `%o` | Entero en base octal sin signo | `unsigned int` | `52` | `64` |
| `%x` | Entero en base hexadecimal sin signo en minúsculas | `unsigned int` | `42`/`0x2A`/`0x2a` | `2a` |
| `%X` | Entero en base hexadecimal sin signo en mayúsculas | `unsigned int` | `42`/`0x2A`/`0x2a` | `2A` |
| `%f` | Número en coma flotante en notación decimal, en minúsculas | `double` | `3.14f`/`3.14` | `3.140000` |
| `%F` | Número en coma flotante en notación decimal, en mayúsculas | `double` | `3.14f`/`3.14` | `3.140000` |
| `%e` | Número en coma flotante en notación científica completa, en minúsculas | `double` | `3.14f`/`3.14` | `3.140000e+00` |
| `%E` | Número en coma flotante en notación científica completa, en mayúsculas | `double` | `3.14f`/`3.14` | `3.140000E+00` |
| `%g` | Número en coma flotante resumida, en minúsculas | `double` | `3.14f`/`3.14` | `3.14` |
| `%G` | Número en coma flotante resumida, en mayúsculas | `double` | `3.14f`/`3.14` | `3.14` |
| `%a` | Número en coma flotante en base hexadecimal, con notación cientifica, en minúsculas | `double` | `3.14f`/`3.14` | `0x1.91eb851eb851fp+1` |
| `%A` | Número en coma flotante en base hexadecimal, con notación cientifica, en mayúsculas | `double` | `3.14f`/`3.14` | `0X1.91EB851EB851FP+1` |
| `%p` | Dirección de memoria | `void*` | `(void*)0x7ffd56a8` | `0x7ffd56a8` |
| `%%` | Imprime el carácter literal `'%'`, no toma argumentos | | | `%` |
### - ENG
The specifier determines how the corresponding argument will be interpreted and displayed.
| Specifier | Description | Ideal Type/s | Example Value | Visual Example |
| :---- | :---- | :---- | :---- | :---- |
| `%c` | Single Character in ASCII Format | `char`, `BYTE` | `'A'`/`65` | `A` |
| `%s` | Text String in ASCII Format, terminated with a null character | `const char*` | `"System Tools"` | `System Tools` |
| `%d` / `%i` | Interger Signed Number | `int` | `-42` | `-42` |
| `%u` | Interger Unsigned Number | `unsigned int` | `42` | `42` |
| `%o` | Interger Unsigned Number in Octal base | `unsigned int` | `52` | `64` |
| `%x` | Lowercased Interger Unsigned Number | `unsigned int` | `42`/`0x2A`/`0x2a` | `2a` |
| `%X` | Uppercased Interger Unsigned Number | `unsigned int` | `42`/`0x2A`/`0x2a` | `2A` |
| `%f` | Lowercased Float Number with Decimal Notation | `double` | `3.14f`/`3.14` | `3.140000` |
| `%F` | Uppercased Float Number with Decimal Notation | `double` | `3.14f`/`3.14` | `3.140000` |
| `%e` | Lowercased Float Number with Scientific Notation | `double` | `3.14f`/`3.14` | `3.140000e+00` |
| `%E` | Uppercased Float Number with Scientific Notation | `double` | `3.14f`/`3.14` | `3.140000E+00` |
| `%g` | Lowercased Abbreviated Float Number | `double` | `3.14f`/`3.14` | `3.14` |
| `%G` | Uppercased Abbreviated Float Number | `double` | `3.14f`/`3.14` | `3.14` |
| `%a` | Lowercased Float Number in Hexadecimal, with Scientific Notation | `double` | `3.14f`/`3.14` | `0x1.91eb851eb851fp+1` |
| `%A` | Uppercased Float Number in Hexadecimal, with Scientific Notation | `double` | `3.14f`/`3.14` | `0X1.91EB851EB851FP+1` |
| `%p` | Pointer | `void*` | `(void*)0x7ffd56a8` | `0x7ffd56a8` |
| `%%` | Prints the literal character `'%'`; takes no arguments | | | `%` |

## Flags
### - ESP
Las banderas modifican la alineación, el relleno y la presentación de los especificadores numéricos.
| Bandera | Descripción | Especificador de Ejemplo | Valor de Ejemplo | Ejemplo Visual |
| :---- | :---- | :---- | :---- | :---- |
| `-` | Alinea la salida a la izquierda dentro de un ancho especificado | `%-10d` | `42` | <code>42&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</code> |
| `+` | Muestra explícitamente el signo para un especificador numeral | `%+d` | `52` | `+52` |
| (espacio) | Imprime un espacio en blanco inicial si un especificador numeral es positivo; de ser negativo, imprimirá su signo. | `% d` | `64` | <code>&nbsp;64</code> |
| `#` | Fuerza el prefijo `0x` o `0X` en especificador hexadecimal respectivo, `0` para el especificador octal, o el punto decimal para el especificador de número flotante | `%#X` | `63`/`0x3F`/`0x3f` | `0x3F` |
| `0` | Rellena con ceros a la izquierda en lugar de espacios, hasta que se complete el ancho acordado; es incompatible con `-` y `-` le gana | `%05d` | `314` | `00314` |
### - ENG
Flags modify alignment, padding, and presentation for numerical specifiers.
| Flag | Description | Example Specifier | Example Value | Visual Example |
| :---- | :---- | :---- | :---- | :---- |
| `-` | Left-aligns the output within the specified width | `%-10d` | `42` | <code>42&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</code> |
| `+` | Explicitly forces the sign for numeral specifiers | `%+d` | `52` | `+52` |
| (space) | Prints a leading blank space if a numeral specifier is positive; if its negative, prints the sign | `% d` | `64` | <code>&nbsp;64</code> |
| `#` | Forces `0x` or `0X` prefix in the respective hexadecimal specifier, `0` for the octal specifier, or decimal point for float numerical specifiers | `%#X` | `63`/`0x3F`/`0x3f` | `0x3F` |
| `0` | Left-pads with zeros instead of spaces, until the agreed width is reached; incompatible with `-`, and `-` wins | `%05d` | `314` | `00314` |

## Width & Precision
### - ESP
* **Width**: Especifica el número mínimo de caracteres a imprimir. Si el valor es más corto, se rellena con espacios o ceros (Dependiendo del especificador); si es más largo, no será truncado.
* **Precision**:
  * **Para especificadores enteros** (`%d`/`%i`, `%u`, `%x`/`%X` y `%o`): Especifica el número mínimo de dígitos a mostrar, en caso de no ser suficientes rellena con ceros a su izquierda.  
  * **Para especificadores flotantes** (`%f`/`%F`, `%e`/`%E` y `%g`/`%G`): Especifica el número de dígitos después del punto decimal.  
  * **Para el especificador de cadena de texto** (`%s`): Especifica el número máximo de carácteres a imprimir, truncará la cadena si excede este límite.

  La diferencia entre **Width** y **Precision** radica en como manejan los casos donde la longitud del valor que se desea imprimir excede el ancho acordado, mientras que **Width** no hará nada al respecto, **Precision** truncará el ancho de carácteres a imprimir del valor para que sea exactamente igual al ancho acordado.\
  Dados a su naturaleza plural, tanto **Width** como **Precision** no pueden alterar el ancho de carácteres a imprimir del especificador de carácter (`%c`), dado a la naturaleza singular de este ultimo.\
  En ambos se puede pasar dinámicamente una longitud, usando un asterisco (`*`) en lugar de un número, aunque esto hará que consuma un argumento antes del argumento correspondiente al especificador.\
  Dado que se necesita del signo de punto (`.`) antes del número o asterisco (`*`) para indicar si se desea usar **Precision** o **Width** (Este ultimo dado a la ausencia de dicho signo), ambos se pueden considerar opuestos entre sí.
### - ENG
* **Width**: Specifies the minimum number of characters to print. If the value is shorter, it is padded with spaces or zeros (Depends of the specifier); if it is longer, it is not truncated.
* **Precision**:  
  * **For integer specifiers** (`%d`/`%i`, `%u`, `%x`/`%X` and `%o`): Specifies the minimum number of digits to display; if there are not enough, it pads with leading zeros.
  * **For float specifiers** (`%f`/`%F`, `%e`/`%E` and `%g`/`%G`): Specifies the number of digits after the decimal point.  
  * **For the string specifier** (`%s`): Specifies the maximum number of characters to print, it will truncate the string if it exceeds this limit.

  The difference between **Width** and **Precision** lies in how they handle cases where the length of the value to be printed exceeds the agreed width; while **Width** will do nothing about it, **Precision** will truncate the width of the characters to be printed from the value so that it is exactly equal to the agreed width.\
  Due to their plural nature, neither **Width** nor **Precision** can alter the printed character width of the character specifier (`%c`), given the latter's singular nature.\
  In both, a length can be passed dynamically, using an asterisk (`*`) instead of a number, although this will cause it to consume an argument before the argument corresponding to the specifier.\
  Since the period (`.`) before the number or asterisk (`*`) is needed to indicate whether to use **Precision** or **Width** (the latter due to the absence of this symbol), both can be considered opposites of each other.

## Length
### - ESP
Los modificadores de longitud adaptan el especificador al tipo exacto en memoria del argumento, alterando explicitamente la forma en la que este es formateado.
| Modificador | Tipo/s Asociados | Especificador/es Asociados | Resultado/s Semanticos |
| :---- | :---- | :---- | :---- |
| `hh` | `char`, `BYTE` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%hhd`/`%hhi`, `%hhu`, `%hhx`/`%hhX`, `%hho` |
| `h` | `short` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%hd`/`%hi`, `%hu`, `%hx`/`%hX`, `%ho` |
| `l` | `long`, `wchar`, `wchar*` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o`, `%c`, `%s` | `%ld`/`%li`, `%lu`, `%lx`/`%lX`, `%lo`, `%lc`, `%ls` |
| `ll` | `long long` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%lld`/`%lli`, `%llu`, `%llx`/`%llX`, `%llo` |
| `z` | `WORD`, `UWORD` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%zd`/`%zi`, `%zu`, `%zx`/`%zX`, `%zo` |
| `t` | `WORD`, `UWORD` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%td`/`%ti`, `%tu`, `%tx`/`%tX`, `%to` |

> Las filas `z` y `t` solo equivalen a `WORD`/`UWORD` cuando la runtime destino las reconoce: la rama antigua de `msvcrt.dll` en Windows puede no aceptarlas, mientras que la UCRT y `libc` si lo hacen. Ante la duda, prefiere las macros [`WORD_PRINT`, `UWORD_PRINT` y `CWORD_PRINT`](Macros.md), que resuelven el modificador correcto según `WORD_SIZE`.
### - ENG
Length modifiers adapt the specifier to the argument's exact type in memory, explicitly altering the way it is formatted.
| Modifier | Associated Type/s | Associated Specifier/s | Semantic Result/s |
| :---- | :---- | :---- | :---- |
| `hh` | `char`, `BYTE` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%hhd`/`%hhi`, `%hhu`, `%hhx`/`%hhX`, `%hho` |
| `h` | `short` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%hd`/`%hi`, `%hu`, `%hx`/`%hX`, `%ho` |
| `l` | `long`, `wchar`, `wchar*` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o`, `%c`, `%s` | `%ld`/`%li`, `%lu`, `%lx`/`%lX`, `%lo`, `%lc`, `%ls` |
| `ll` | `long long` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%lld`/`%lli`, `%llu`, `%llx`/`%llX`, `%llo` |
| `z` | `WORD`, `UWORD` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%zd`/`%zi`, `%zu`, `%zx`/`%zX`, `%zo` |
| `t` | `WORD`, `UWORD` | `%d`/`%i`, `%u`, `%x`/`%X`, `%o` | `%td`/`%ti`, `%tu`, `%tx`/`%tX`, `%to` |

> The `z` and `t` rows only equal `WORD`/`UWORD` when the target runtime recognizes them: the legacy `msvcrt.dll` branch on Windows may not accept them, while the UCRT and `libc` do. When in doubt, prefer the [`WORD_PRINT`, `UWORD_PRINT` and `CWORD_PRINT`](Macros.md) macros, which resolve the right modifier from `WORD_SIZE`.

## Input Specifiers (`scanf`, `sscanf`)
### - ESP
Las funciones de entrada comparten la estructura de especificadores, banderas, anchos y modificadores de longitud descritos anteriormente, con diferencias semánticas que conviene tener presentes:
| Especificador | Lee | Argumento variádico esperado |
| :---- | :---- | :---- |
| `%d`/`%i`, `%u`, `%o`, `%x`/`%X` | Un entero según la base indicada (`%i` deduce la base por el prefijo `0x`/`0`) | `int*`, o el tipo del modificador de longitud (`%lld` → `long long*`, `%zu` → `UWORD*`) |
| `%f`, `%e`/`%E`, `%g`/`%G`, `%a`/`%A` | Un número en punto flotante | `float*` (con `%lf`/`%le`/`%lg`, `double*`) |
| `%c` | Un único carácter, sin saltar espacios en blanco | `char*`, con espacio para 1 byte |
| `%s` | Una palabra, detenida en el primer espacio en blanco | `char*`, con espacio para la palabra **más** el terminador `'\0'` |
| `%[` | Un conjunto de caracteres hasta encontrar uno fuera (o dentro, con `[^...]`) del conjunto | `char*`, con espacio para la secuencia **más** el terminador `'\0'` |
| `%%` | Un literal `%`, sin consumir argumento | Ninguno |
| `%n` | Nada; escribe en el `int*` la cantidad de caracteres consumidos hasta el momento | `int*` |

Diferencias frente a la salida:
* El ancho en entrada es un **límite máximo** de caracteres leídos (`%9s` lee hasta 9 y termina con `'\0'`), no un mínimo de relleno. Omitir el ancho en `%s` y `%[` deja la escritura sin límite, que es la fuente clásica de desbordamiento de buffer.
* Las banderas `-`, `+`, `#`, `0` y el ancho dinámico con `*` no se aplican igual que en `printf`; algunas runtime aceptan un `$` posfijo para dirigir el argumento por posición, pero es una extensión y no debe usarse como portable.
* El valor devuelto cuenta las conversiones **asignadas**: el escaneo se detiene en el primer carácter que no encaja con el especificador, así que `-1` significa "fin de entrada antes de la primera conversión", y un valor menor que el número de punteros pasados deja los punteros restantes sin escribir.
* Todo argumento variádico de entrada es un puntero a la ubicación de destino; pasar un puntero nulo, o un valor en lugar de su dirección, produce un **Segmentation Fault**.
### - ENG
The input functions share the specifier, flag, width and length-modifier structure described above, with semantic differences worth keeping in mind:
| Specifier | Reads | Expected variadic argument |
| :---- | :---- | :---- |
| `%d`/`%i`, `%u`, `%o`, `%x`/`%X` | An integer in the given base (`%i` deduces the base from the `0x`/`0` prefix) | `int*`, or the type implied by the length modifier (`%lld` → `long long*`, `%zu` → `UWORD*`) |
| `%f`, `%e`/`%E`, `%g`/`%G`, `%a`/`%A` | A floating-point number | `float*` (with `%lf`/`%le`/`%lg`, `double*`) |
| `%c` | A single character, without skipping whitespace | `char*`, with room for 1 byte |
| `%s` | One word, stopping at the first whitespace | `char*`, with room for the word **plus** the `'\0'` terminator |
| `%[` | A character set, until a character outside (or, with `[^...]`, inside) the set | `char*`, with room for the sequence **plus** the `'\0'` terminator |
| `%%` | A literal `%`, consuming no argument | None |
| `%n` | Nothing; writes into the `int*` how many characters were consumed so far | `int*` |

Differences compared to output:
* In input, the width is a **maximum** of characters read (`%9s` reads up to 9 and terminates with `'\0'`), not a minimum padding. Omitting the width on `%s` and `%[` leaves the write unbounded, which is the classic buffer-overflow source.
* The `-`, `+`, `#`, `0` flags and the dynamic `*` width do not behave as in `printf`; some runtimes accept a trailing `$` to address an argument positionally, but that is an extension and must not be relied on for portability.
* The return value counts the conversions **assigned**: scanning stops at the first character that does not match the specifier, so `-1` means "end of input before the first conversion", and a value lower than the number of pointers passed leaves the remaining pointers unwritten.
* Every variadic argument on the input side is a pointer to the destination; passing a null pointer, or a value instead of its address, produces a **Segmentation Fault**.
