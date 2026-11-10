# restorecolor
### - ESP
Firma:
```cpp
void* restorecolor(void* cnsl);
```
Parámetros:
* **Cnsl**: Handle Consola cuyos colores se devolverán al estado original, o `nullptr` para usar el handle único del programa.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si la secuencia de color quedó en manos de la consola, con las mismas causas de `nullptr` que `console::setcolor`.

\
Comportamiento:\
Devuelve los colores al par que `console::get_handle` capturó al abrir la consola, y deja ese mismo par anotado como los corrientes, así que `console::getcolor` responderá después los originales.\
El único estado recordado es el del arranque: quien necesite volver a un color intermedio debe haberlo tomado antes con `console::getcolor`.\
No repinta lo ya escrito, y se puede llamar tantas veces como se quiera.\
No hace falta llamarla al terminar el programa: la destrucción del handle restituye los colores por su cuenta.

### - ENG
Signature:
```cpp
void* restorecolor(void* cnsl);
```
Parameters:
* **Cnsl**: Console Handle whose colors are to be brought back to their original state, or `nullptr` to use the program's unique handle.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the color sequence was in the console's hands, with the same causes of `nullptr` as `console::setcolor`.

\
Behavior:\
It brings the colors back to the pair `console::get_handle` captured when the console opened, and leaves that same pair noted down as the current ones, so `console::getcolor` will answer with the originals afterwards.\
The only remembered state is the one at the start: whoever needs an in-between color must have taken it beforehand with `console::getcolor`.\
It does not repaint what was already written, and it can be called as many times as one likes.\
There is no need to call it when the program ends: the handle's destruction restores the colors on its own.
