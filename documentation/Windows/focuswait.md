# focuswait
### - ESP
Firma:
```cpp
void* focuswait(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana cuyo foco se desea certificar.

Retorno:\
El mismo handle entregado una vez la ventana obtuvo el foco. `nullptr` si el handle es inválido, si la ventana ya había cerrado, si la consulta de foco no estaba disponible, o si la ventana cerró mientras se esperaba por ella.

\
Comportamiento:\
Esta función bloquea al hilo llamador hasta que la ventana es la que posee el foco de entrada del sistema.\
No solicita el foco, solo lo vigila: si nadie se lo entrega, el bloqueo se prolonga indefinidamente.\
La secuencia habitual es invocar primero `window::focus` y después `window::focuswait` para confirmar que el sistema lo aceptó.\
Mientras espera, la ventana puede cerrar (por el usuario o por `window::destroy`), y en ese caso la función se libera y entrega `nullptr` en lugar de quedarse bloqueada para siempre.\
Es el paso previo recomendado antes de leer teclado con `window::keyboard`, pues esa consulta solo responde cuando la ventana está enfocada.

### - ENG
Signature:
```cpp
void* focuswait(void* wnd);
```
Parameters:
* **Wnd**: Handle Window whose focus is to be certified.

Return:\
The very same handle once the window obtained the focus. `nullptr` if the handle is invalid, the window had already closed, the focus query was unavailable, or the window closed while being waited on.

\
Behavior:\
This function blocks the calling thread until the window is the one holding the system's input focus.\
It does not request the focus, it only watches for it: if nobody hands it over, the block lasts indefinitely.\
The usual sequence is invoking `window::focus` first and then `window::focuswait` to confirm the system accepted it.\
While waiting, the window may close (by the user or by `window::destroy`), and in that case the function frees itself and delivers `nullptr` instead of staying blocked forever.\
It is the recommended preceding step before reading the keyboard with `window::keyboard`, since that query only answers when the window is focused.
