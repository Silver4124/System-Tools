# gotoxy
### - ESP
Firma:
```cpp
void* gotoxy(void* cnsl, UWORD x, UWORD y);
```
Parámetros:
* **Cnsl**: Handle Consola cuyo cursor se moverá, o `nullptr` para usar el handle único del programa.
* **X**: columna de destino, contada en celdas de carácter desde `0`.
* **Y**: fila de destino, contada en celdas de carácter desde `0`.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si el cursor quedó en el punto pedido.\
`nullptr` si el handle es inválido, si la consola no tiene salida utilizable o si la posición cayó fuera del área de escritura.

\
Comportamiento:\
Coloca el cursor sin borrar, sin pintar y sin desplazar nada: lo escrito después empezará en ese punto, que es la forma de redibujar en sitio.\
Las coordenadas van en celdas, no en píxeles, con el origen arriba a la izquierda. En Windows se refieren al búfer de pantalla completo, así que una consola desplazada acepta una posición que no está a la vista; en los sistemas tipo UNIX se refieren a la pantalla visible del terminal.\
Mover el cursor no trae a la vista la zona donde cae, ni devuelve la posición anterior: para conservar el punto de partida hay que tomarlo antes con `console::getxy`.

### - ENG
Signature:
```cpp
void* gotoxy(void* cnsl, UWORD x, UWORD y);
```
Parameters:
* **Cnsl**: Console Handle whose cursor is to be moved, or `nullptr` to use the program's unique handle.
* **X**: destination column, counted in character cells from `0`.
* **Y**: destination row, counted in character cells from `0`.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the cursor ended up at the asked point.\
`nullptr` if the handle is invalid, the console has no usable output, or the position fell outside the writable area.

\
Behavior:\
It places the cursor without erasing, painting or shifting anything: whatever is written afterwards starts at that point, which is how in-place redrawing is done.\
Coordinates go in cells, not in pixels, with the origin at the upper left. On Windows they refer to the whole screen buffer, so a scrolled console accepts a position that is not in view; on UNIX-like systems they refer to the terminal's visible screen.\
Moving the cursor neither brings the zone it lands on into view nor gives the previous position back: to keep the starting point it must be taken beforehand with `console::getxy`.
