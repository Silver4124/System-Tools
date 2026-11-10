# sscanf
### - ESP
Firma:
```cpp
int sscanf(const char* RESTRICT str, const char* RESTRICT format, ...);
```
Parámetros:
* **Str**: Cadena de texto o buffer donde el formato (Argumento **Format**) consultará los datos.
* **Format**: Cadena de texto que representa el formato a seguir para la lectura de la cadena de texto o buffer (Argumento **Str**).
* Argumentos adicionales, donde se consultarán los punteros donde escribir los valores obtenidos en base al formato, de ser necesario.

Retorno:\
Cantidad de punteros del Variadic Argument donde se escribieron resultados del escaneo, si es positivo.\
`0` si no se escribieron en ningún puntero (usualmente si el formato es una cadena nula o no se produjo ningún resultado).\
`-1` si hubo un fallo.

\
Comportamiento:\
Lee valores desde la cadena de texto o buffer **Str** según el formato **Format**.\
Cada valor producido se almacena en la dirección apuntada por el puntero correspondiente del Variadic Argument; todo argumento se considera indirectamente un puntero.\
Puede producirse un **Segmentation Fault** si algún puntero del Variadic Argument es `nullptr`.\
System Tools no implementa su propio analizador: el escaneo lo resuelve la runtime C del sistema, así que los especificadores de conversión, los modificadores de longitud y el soporte de `%n` o de conjuntos `[...]` dependen de esa runtime.\
Adaptación de `sscanf` de `<stdio.h>`. Para el formateo, consulte [[[C Formating]]](../C%20Formating.md).

### - ENG
Signature:
```cpp
int sscanf(const char* RESTRICT str, const char* RESTRICT format, ...);
```
Parameters:
* **Str**: Text string or buffer where the format (Argument **Format**) will query the data.
* **Format**: Text string representing the format to follow for reading the text string or buffer (Argument **Str**).
* Additional arguments, where the pointers to write the values getted in base the format, if necessary.

Return:\
Number of pointers passed to the Variadic Argument where the scan results were written, if positive.\
`0` if they were not written to any pointer (usually if the format is a null string or no result was produced).\
`-1` if there was a failure.

\
Behavior:\
It reads values from the text string or buffer **Str** according to the format **Format**.\
Each produced value is stored in the address pointed to by the corresponding pointer of the Variadic Argument; every argument is considered indirectly a pointer.\
A **Segmentation Fault** may occur if any pointer in the Variadic Argument is `nullptr`.\
System Tools does not implement its own parser: the scan is resolved by the system C runtime, so the conversion specifiers, length modifiers and the support for `%n` or `[...]` sets depend on that runtime.\
Adaptation of `sscanf` from `<stdio.h>`. For the formatting, consult [[[C Formatting]]](../C%20Formating.md).