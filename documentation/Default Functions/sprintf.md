# sprintf
### - ESP
Firma:
```cpp
int sprintf(char* RESTRICT str, const char* RESTRICT format, ...);
```
Parámetros:
* **Str**: Cadena de texto o buffer donde se desea que se almacene el resultado textual.
* **Format**: Cadena de texto que representa el formato a seguir para la impresión en la cadena de texto o buffer (Argumento **Str**).
* Argumentos adicionales, donde se consultarán los valores para rellenar el formato, de ser necesario.

Retorno:\
Cantidad de carácteres impresos en la cadena de texto o buffer **Str**, si es positivo.\
`0` si no se imprimieron carácteres (usualmente si el formato es una cadena nula o no se imprimió texto).\
`-1` si hubo un fallo.

\
Comportamiento:\
Imprime texto en la cadena de texto o buffer **Str** según el formato **Format** y los valores del Variadic Argument.\
System Tools no implementa su propio motor de formato: la impresión la resuelve la runtime C del sistema, así que los especificadores y modificadores admitidos son los de esa runtime.\
Adaptación de `sprintf` de `<stdio.h>`. Para el formateo, consulte [[[C Formating]]](../C%20Formating.md).

\
Nota:\
Esta función no recibe ningún tamaño de buffer: es responsabilidad del programador que el espacio apuntado por **Str** quepa el resultado completo, incluido el terminador `'\0'`, que sí se escribe automáticamente.\
La librería no expone todavía un `snprintf`, así que no existe una variante con truncamiento garantizado; para calcular la capacidad necesaria use el valor de retorno de una invocación previa (indica los caracteres escritos, sin contar el terminador) o reserve holgura de sobra.

### - ENG
Signature:
```cpp
int sprintf(char* RESTRICT str, const char* RESTRICT format, ...);
```
Parameters:
* **Str**: Text string or buffer where are writted the textual result.
* **Format**: Text string that represents the format to follow for printing the text in the buffer or text string (Argument **Str**).
* Additional arguments, where the values to fill in the format will be consulted, if necessary.

Return:\
Number of characters printed on the text string or buffer (Argument **Str**) if positive.\
`0` if no characters were printed (usually if the format is a null string or no text was printed).\
`-1` if there was a failure.

\
Behavior:\
It prints text to the text string or buffer **Str** based on the format **Format** and the values of the Variadic Argument.\
System Tools does not implement its own formatting engine: the print is resolved by the system C runtime, so the accepted specifiers and modifiers are those of that runtime.\
Adaptation of `sprintf` from `<stdio.h>`. For the formatting, consult [[[C Formatting]]](../C%20Formating.md).

\
Note:\
This function takes no buffer size: it is the programmer's responsibility that the space pointed to by **Str** fits the whole result, including the `'\0'` terminator, which is indeed written automatically.\
The library does not expose a `snprintf` yet, so there is no variant with guaranteed truncation; to determine the required capacity, use the return value of a previous call (which reports the characters written, excluding the terminator) or reserve generous headroom.