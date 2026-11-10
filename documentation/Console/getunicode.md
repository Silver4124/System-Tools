# getunicode
### - ESP
Firma:
```cpp
bool getunicode(void* cnsl);
```
Parámetros:
* **Cnsl**: Handle Consola cuyo modo de caracteres se consultará, o `nullptr` para usar el handle único del programa.

Retorno:\
`true` si el handle tiene anotado que la consola está en modo Unicode, `false` en caso contrario.\
`false` también cuando el handle es inválido, que es el mismo valor que responde una consola en modo heredado: si la distinción importa, hay que comprobar antes que `console::get_handle` devolvió una dirección válida.

\
Comportamiento:\
Lee el estado que `console::setunicode` dejó anotado en el handle, sin preguntar ni a la consola ni al terminal.\
El valor inicial no es una convención, sino lo que la consola tenía al abrirse: la página de código en Windows, y las variables de idioma en los sistemas tipo UNIX, con `true` por defecto cuando ninguna está definida.\
Sirve para decidir antes de escribir: si la consola no está en Unicode y se va a volcar texto con acentos por la vía de los bytes, queda `console::setunicode` o entregar texto ancho a `console::write`, que no depende del modo.

### - ENG
Signature:
```cpp
bool getunicode(void* cnsl);
```
Parameters:
* **Cnsl**: Console Handle whose character mode is to be consulted, or `nullptr` to use the program's unique handle.

Return:\
`true` if the handle has noted that the console is in Unicode mode, `false` otherwise.\
Also `false` when the handle is invalid, which is the very same value a legacy-mode console answers: if the distinction matters, check first that `console::get_handle` returned a valid address.

\
Behavior:\
It reads the state `console::setunicode` left noted in the handle, asking neither the console nor the terminal.\
The starting value is no convention but whatever the console had when it opened: the code page on Windows, and the language variables on UNIX-like systems, with `true` as the default when none is defined.\
It serves to decide before writing: if the console is not in Unicode and accented text is about to go out through the byte route, the choice is `console::setunicode` or handing wide text to `console::write`, which does not depend on the mode.
