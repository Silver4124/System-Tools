# printf
### - ESP
Firma:
```cpp
int printf(const char* RESTRICT format, ...);
```
Parámetros:
* **Format**: Cadena de texto que representa el formato a seguir para la impresión del texto.
* Argumentos adicionales, donde se consultarán los valores para rellenar el formato, de ser necesario.

Retorno:\
Cantidad de carácteres impresos en la terminal si es positivo.\
`0` si no se imprimieron carácteres (usualmente si el formato es una cadena nula o no se imprimió texto).\
`-1` si hubo un fallo.

\
Comportamiento:\
Imprime texto en la terminal según el formato **Format** y los valores del Variadic Argument.\
System Tools no implementa su propio motor de formato: el resultado lo resuelve la runtime C del sistema, así que los especificadores, modificadores de longitud y banderas admitidos son los de esa runtime.\
Adaptación de `printf` de `<stdio.h>`. Para el formateo, consulte [[[C Formating]]](../C%20Formating.md).

### - ENG
Signature:
```cpp
int printf(const char* RESTRICT format, ...);
```
Parameters:
* **Format**: Text string that represents the format to follow for printing the text.
* Additional arguments, where the values to fill in the format will be consulted, if necessary.

Return:\
Number of characters printed on the terminal if positive.\
`0` if no characters were printed (usually if the format is a null string or no text was printed).\
`-1` if there was a failure.

\
Behavior:\
It prints text to the terminal based on the format **Format** and the values of the Variadic Argument.\
System Tools does not implement its own formatting engine: the result is resolved by the system C runtime, so the accepted specifiers, length modifiers and flags are those of that runtime.\
Adaptation of `printf` from `<stdio.h>`. For the formatting, consult [[[C Formatting]]](../C%20Formating.md).