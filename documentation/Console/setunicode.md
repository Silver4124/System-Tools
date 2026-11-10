# setunicode
### - ESP
Firma:
```cpp
void* setunicode(void* cnsl, bool unicode);
```
Parámetros:
* **Cnsl**: Handle Consola cuyo modo de caracteres se cambiará, o `nullptr` para usar el handle único del programa.
* **Unicode**: `true` para poner la consola en modo Unicode, `false` para el modo heredado.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si el cambio quedó aplicado.\
`nullptr` si el handle es inválido, si la consola no tiene salida utilizable o si faltan las funciones con las que se cambia el modo.

\
Comportamiento:\
Esta función decide cómo la consola interpreta los carácteres que reciba. En modo Unicode (`unicode = true`), se espera ver texto correspondiente de UTF-8 o UTF-16; en modo heredado (`unicode = false`), se espera ver texto correspondiente de ANSI. Carácteres impresos de UTF-8 o UTF-16 usando el modo heredado, se verán afectados, siendo impresos como sus secuencias ANSI correspondientes.\
El estado actual se puede consultar por medio de `console::getunicode`.\
El estado se puede retornar al original por medio de `console::restoreunicode`.

### - ENG
Signature:
```cpp
void* setunicode(void* cnsl, bool unicode);
```
Parameters:
* **Cnsl**: Console Handle whose character mode is to be changed, or `nullptr` to use the program's unique handle.
* **Unicode**: `true` to put the console in Unicode mode, `false` to put the Legacy mode.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the change ended up applied.\
`nullptr` if the handle is invalid, the console has no usable output, or the functions used to change the mode are missing.

\
Behavior:\
This function decides how the console interprets the characters it receives. In Unicode mode (`unicode = true`), you should expect to see corresponding UTF-8 or UTF-16 text; In legacy mode (`unicode = false`), you expect to see the corresponding ANSI text. If UTF-8 or UTF-16 characters are printed while in legacy mode, they will be affected and rendered as their corresponding ANSI sequences.\
The current state can be queried using `console::getunicode`.\
The state can be restored to its original setting using `console::restoreunicode`.