# scanf
### - ESP
Firma:
```cpp
int scanf(const char* RESTRICT format, ...);
```
Parámetros:
* **Format**: Cadena de texto que representa el formato a seguir para la obtención de valores.
* Argumentos adicionales, donde se consultarán los punteros donde escribir los valores obtenidos en base al formato, de ser necesario.

Retorno:\
Cantidad de punteros del Variadic Argument donde se escribieron resultados del escaneo, si es positivo.\
`0` si no se escribieron en ningún puntero (usualmente si el formato es una cadena nula o no se produjo ningún resultado).\
`-1` si hubo un fallo.

\
Comportamiento:\
Causa una interrupción bloqueante en el programa o hilo que la ejecute, esperando a que el usuario termine de escribir texto en la terminal.\
Cada valor producido se almacena en la dirección apuntada por el puntero correspondiente del Variadic Argument; todo argumento se considera indirectamente un puntero.\
Puede producirse un **Segmentation Fault** si algún puntero del Variadic Argument es `nullptr`.\
System Tools no implementa su propio analizador: el escaneo lo resuelve la runtime C del sistema, así que especificadores, modificadores de longitud y el tratamiento de anchos y precisiones son los de esa runtime.\
Adaptación de `scanf` de `<stdio.h>`. Para el formateo, consulte [[[C Formating]]](../C%20Formating.md).

### - ENG
Signature:
```cpp
int scanf(const char* RESTRICT format, ...);
```
Parameters:
* **Format**: Text string that represents the format to follow for the values getting.
* Additional arguments, where the pointers to write the values getted in base the format, if necessary.

Return:\
Number of pointers passed to the Variadic Argument where the scan results were written, if positive.\
`0` if they were not written to any pointer (usually if the format is a null string or no result was produced).\
`-1` if there was a failure.

\
Behavior:\
It causes a blocking interruption in the program or thread that executes it, waiting for the user to finish typing text in the terminal.\
Each produced value is stored in the address pointed to by the corresponding pointer of the Variadic Argument; every argument is considered indirectly a pointer.\
A **Segmentation Fault** may occur if any pointer in the Variadic Argument is `nullptr`.\
System Tools does not implement its own parser: the scan is resolved by the system C runtime, so the specifiers, length modifiers and the handling of widths and precisions are those of that runtime.\
Adaptation of `scanf` from `<stdio.h>`. For the formatting, consult [[[C Formatting]]](../C%20Formating.md).