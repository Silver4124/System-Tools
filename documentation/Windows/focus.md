# focus
### - ESP
Firma:
```cpp
void* focus(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana que debe pasar a primer plano.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido, si la ventana ya había cerrado o si la operación de foco no estaba disponible.

\
Comportamiento:\
Esta función coloca la ventana en primer plano y le entrega el foco de entrada, como si el usuario hiciera clic sobre ella.\
Es una solicitud: el sistema puede denegar el foco si otra aplicación lo retiene por fuerza mayor, por lo que la ventana puede no quedar enfocada pese a la llamada.\
Cuando el programa necesita certificar el foco antes de continuar, se usa `window::focuswait`.\
La entrega de foco se notifica al mailman de la ventana como el evento [[[Enum window_box]]](Enum%20window_box.md) `window_box::activate`, y también se produce cuando el usuario enfoca o desenfoca la ventana por su cuenta.\
Solo una ventana con el foco recibe la consulta de teclado de `window::keyboard`, así que esta función es el paso previo habitual antes de leer teclas.\
Sobre un handle externo devuelto por `window::active`, la función intenta enfocar la ventana ajena.

### - ENG
Signature:
```cpp
void* focus(void* wnd);
```
Parameters:
* **Wnd**: Handle Window that must come to the foreground.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid, the window had already closed, or the focus operation was unavailable.

\
Behavior:\
This function brings the window to the foreground and hands it the input focus, as if the user clicked on it.\
It is a request: the system may deny the focus if another application withholds it by force majeure, so the window may end up unfocused despite the call.\
When the program needs to certify the focus before continuing, `window::focuswait` is used.\
The handing over of focus is notified to the window's mailman as the [[[Enum window_box]]](Enum%20window_box.md) `window_box::activate` event, and it also happens when the user focuses or unfocuses the window on their own.\
Only a focused window receives the keyboard query of `window::keyboard`, so this function is the usual preceding step before reading keys.\
Over an external handle returned by `window::active`, the function tries to focus the foreign window.
