# setcolor
### - ESP
Firma:
```cpp
void* setcolor(void* cnsl, unsigned int bg, unsigned int fg);
```
Parámetros:
* **Cnsl**: Handle Consola cuyo par de colores se cambiará, o `nullptr` para usar el handle único del programa.
* **Bg**: color de fondo en el formato `0x00BBGGRR`.
* **Fg**: color de primer plano (el de los caracteres), en el formato `0x00BBGGRR`.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) una vez que la secuencia de color quedó en manos de la consola.\
`nullptr` si el handle es inválido, si la consola no tiene salida utilizable, si faltan las funciones necesarias o si en los sistemas tipo UNIX la escritura de la secuencia falló.\
Un terminal que no entiende el pedido lo ignora, o lo imprime como texto, sin que la función pueda saberlo.

\
Comportamiento:\
Tiñe el par de colores con el que se escribirá de aquí en adelante, las celdas ya escritas conservan el color con el que nacieron, y el cambio se nota a partir de los siguientes carácteres a imprimir.\
El formato es de veinticuatro bits por canal, y la paleta del terminal no se toca. Ejemplos son que `0x000000FF` es el canal para rojo, `0x0000FF00` el canal para verde, y `0x00FF0000` el canal para azul. El byte de mayor orden se ignora, por lo que no hay canal para transparencia, así que `0x00FFFFFF` y `0x01FFFFFF` (o `0x02FFFFFF`, `0x04FFFFFF`, `0x08FFFFFF`, `0x0FFFFFFF`, `0xF0FFFFFF`, `0xFFFFFFFF`, etc.) son el mismo blanco.\
`console::setcolor(cnsl, bg, fg)` con los dos colores en `0x00000000` produce negro sobre negro, texto invisible, pero eso no significa necesariamente problemas de escritura.\
El estado actual se puede consultar por medio de `console::getcolor`.\
El estado se puede retornar al original por medio de `console::restorecolor`.

### - ENG
Signature:
```cpp
void* setcolor(void* cnsl, unsigned int bg, unsigned int fg);
```
Parameters:
* **Cnsl**: Console Handle whose pair of colors is to be changed, or `nullptr` to use the program's unique handle.
* **Bg**: background color in the format `0x00BBGGRR`.
* **Fg**: foreground color (the color of the characters), in the format `0x00BBGGRR`.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) once the color sequence was in the console's hands.\
`nullptr` if the handle is invalid, the console has no usable output, the needed functions are missing, or the sequence writing failed on UNIX-like systems.\
The answer says the notice went out, not that the terminal obeyed it: a terminal that does not understand the request ignores it, or prints it, without the function being able to know.

\
Behavior:\
It dyes the pair of colors that will be used from here on; the cells already written keep the color they were born with, and the change is noticed starting with the next characters to be printed.\
The format is of twenty-four bits per channel, and the terminal's palette is not touched. Examples are that `0x000000FF` is the channel for red, `0x0000FF00` the channel for green, and `0x00FF0000` the channel for blue. The highest-order byte is ignored, so there is no channel for transparency, and thus `0x00FFFFFF` and `0x01FFFFFF` (or `0x02FFFFFF`, `0x04FFFFFF`, `0x08FFFFFF`, `0x0FFFFFFF`, `0xF0FFFFFF`, `0xFFFFFFFF`, etc.) are the same white.\
`console::setcolor(cnsl, bg, fg)` with both colors at `0x00000000` produces black on black, invisible text, but that does not necessarily mean there are writing problems.\
The current state can be queried using `console::getcolor`.\
The state can be reset to the original using `console::restorecolor`.