# restoreunicode
### - ESP
Firma:
```cpp
void* restoreunicode(void* cnsl);
```
Parámetros:
* **Cnsl**: Handle Consola cuyo modo de caracteres se devolverá al estado original, o `nullptr` para usar el handle único del programa.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si el cambio quedó aplicado, con las mismas causas de `nullptr` que `console::setunicode`.

\
Comportamiento:\
Devuelve el modo de caracteres al valor que `console::get_handle` capturó al abrir la consola, y deja ese valor anotado como el corriente, así que `console::getunicode` responderá después el estado de arranque.\
El único estado recordado es el del arranque: quien necesite volver a un modo intermedio debe haberlo tomado antes con `console::getunicode`.\
Se puede llamar cuantas veces se quiera, con o sin cambios previos.\
No hace falta llamarla al terminar el programa: la destrucción del handle restituye el modo por su cuenta.

### - ENG
Signature:
```cpp
void* restoreunicode(void* cnsl);
```
Parameters:
* **Cnsl**: Console Handle whose character mode is to be brought back to its original state, or `nullptr` to use the program's unique handle.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the change was applied, with the same causes of `nullptr` as `console::setunicode`.

\
Behavior:\
It brings the character mode back to the value `console::get_handle` captured when the console opened, and leaves that value noted down as the current one, so `console::getunicode` will answer with the starting state afterwards.\
The only remembered state is the one at the start: whoever needs an in-between mode must have taken it beforehand with `console::getunicode`.\
It can be called as many times as one likes, with or without previous changes.\
There is no need to call it when the program ends: the handle's destruction restores the mode on its own.
