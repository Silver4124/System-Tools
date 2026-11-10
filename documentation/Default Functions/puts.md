# puts
### - ESP
Firma:
```cpp
int puts(const char* str);
```
Parámetros:
* **Str**: Cadena de texto que se desea imprimir en la terminal.

Retorno:\
`-1` si **Str** es `nullptr`, si la función del sistema no está disponible o si la impresión falló; cualquier otro valor significa que la impresión salió bien.\
El valor positivo no es un conteo de caracteres: de la impresión solo se garantiza que no sea negativo, así que no conviene medir por esta vía la longitud de lo impreso.

\
Comportamiento:\
Imprime la cadena en la terminal y añade un salto de línea al final del texto.\
Adaptación de `puts` de `<stdio.h>` (véase [Default Functions](README.md)).

### - ENG
Signature:
```cpp
int puts(const char* str);
```
Parameters:
* **Str**: Text string to be printed to the terminal.

Return:\
`-1` if **Str** is `nullptr`, if the system function is unavailable, or if the print failed; any other value means the print went through.\
The positive value is not a character count: of the print only the non-negative is guaranteed, so this is not a way to measure how much was written.

\
Behavior:\
It prints the string to the terminal and adds a newline at the end of the text.\
Adaptation of `puts` from `<stdio.h>` (see [Default Functions](README.md)).