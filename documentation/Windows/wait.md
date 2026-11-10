# wait
### - ESP
Firma:
```cpp
void* wait(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana sobre el que se desea aguardar.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie al comenzar la espera. `nullptr` si el handle es inválido o si la ventana ya había cerrado antes de la llamada.

\
Comportamiento:\
Esta función bloquea al hilo llamador hasta que la ventana cierra.\
Es la forma habitual de mantener un programa vivo mientras el usuario tiene su ventana abierta.\
Si el handle proviene de `window::active` (una ventana ajena a la librería), el bloqueo se extiende hasta que esa ventana deja de existir en el sistema.\
Antes de bloquear, conviene distinguir qué tipo de handle se tiene con `window::is_extern`.\
Al regresar, el handle ha sido destruido: la ventana ya no existe y el puntero entregado deja de ser utilizable para cualquier función del módulo.\
El programador no debe liberarlo por su cuenta.

### - ENG
Signature:
```cpp
void* wait(void* wnd);
```
Parameters:
* **Wnd**: Handle Window to wait on.

Return:\
The very same handle if it was valid and the window was standing when the wait began. `nullptr` if the handle is invalid or the window had already closed before the call.

\
Behavior:\
This function blocks the calling thread until the window closes.\
It is the usual way to keep a program alive while the user has its window open.\
If the handle comes from `window::active` (a window foreign to the library), the block extends until that window stops existing in the system.\
Before blocking, it is worth telling which kind of handle is at hand with `window::is_extern`.\
Upon returning, the handle has been destroyed: the window no longer exists and the delivered pointer stops being usable for any module function.\
The programmer must not release it by hand.
