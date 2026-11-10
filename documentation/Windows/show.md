# show
### - ESP
Firma:
```cpp
void* show(void* wnd, bool visible, bool minimize = false);
```
Parámetros:
* **Wnd**: Handle Ventana cuyo estado de pantalla se desea cambiar.
* **visible**: `true` para que la ventana vuelva a estar a la vista, `false` para ocultarla.
* **minimize**: `true` para que la ventana aparezca minimizada en lugar de restaurada. Por defecto `false`. Solo tiene efecto cuando **visible** es `true`.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido, si la ventana ya había cerrado o si el cambio no pudo aplicarse.

\
Comportamiento:\
Esta función hace aparecer o desaparecer la ventana sin cerrarla: una ventana oculta conserva su búfer de píxeles, su título y su cola de eventos, y el mailman sigue atendiendo.\
Ocultar la ventana no la destruye; para terminarla se usa `window::destroy`.\
Tampoco detiene `window::wait`, que seguirá bloqueado hasta el cierre definitivo.\
Pedir **minimize** en `true` muestra la ventana reducida al espacio que el sistema reserva para ella.\
El módulo registra entonces la ventana como minimizada, condición que puede leerse con `window::gettitle`.\
Deja de registrarse cuando la ventana vuelve a mostrarse sin **minimize** o cuando el usuario la restaura.\
Ocultar una ventana ya oculta, o mostrar una ya visible, es inocuo: el estado resultante es el mismo y la ventana queda registrada como no minimizada al ocultarse.\
En Linux, mostrar una ventana sin **minimize** además le entrega el foco; para controlar el foco por separado existen `window::focus` y `window::focuswait`.

### - ENG
Signature:
```cpp
void* show(void* wnd, bool visible, bool minimize = false);
```
Parameters:
* **Wnd**: Handle Window whose screen state is to be changed.
* **visible**: `true` to bring the window back into view, `false` to hide it.
* **minimize**: `true` to make the window appear minimized instead of restored. Defaults to `false`. It only takes effect when **visible** is `true`.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid, the window had already closed, or the change could not be applied.

\
Behavior:\
This function makes the window appear or disappear without closing it: a hidden window keeps its pixel buffer, its title, and its event queue, and the mailman keeps attending.\
Hiding the window does not destroy it; `window::destroy` is used to end it.\
It also does not release `window::wait`, which stays blocked until the definitive closing.\
Requesting **minimize** as `true` shows the window reduced into the space the system reserves for it.\
The module then records the window as minimized, a condition readable with `window::gettitle`.\
It stops being recorded once the window is shown again without **minimize** or the user restores it.\
Hiding an already hidden window, or showing an already visible one, is harmless: the resulting state is the same and the window is recorded as non-minimized when hidden.\
On Linux, showing a window without **minimize** also hands it the focus; `window::focus` and `window::focuswait` exist to control focus separately.
