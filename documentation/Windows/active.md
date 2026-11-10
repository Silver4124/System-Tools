# active
### - ESP
Firma:
```cpp
void* active();
```
Retorno:\
**Handle Ventana** que representa a la ventana enfocada en ese instante, sea propia o ajena a la librería.\
`nullptr` si el entorno gráfico no está disponible o si no hay ninguna ventana con el foco.

\
Comportamiento:\
Entrega un handle que apunta a la ventana que el sistema tiene en primer plano, sea de este programa o de cualquier otra aplicación.\
El handle queda marcado como externo, condición que se comprueba con `window::is_extern`.\
Un handle externo admite `window::gettitle`, `window::retitle`, `window::father`, `window::resize`, `window::show`, `window::focus`, `window::destroy`, `window::wait`, y `window::sendmessage` con la etiqueta `window_box::close`.\
No admite dibujo: `window::draw`, `window::pixels` y `window::clear` lo rechazarán, y `window::getsize` dejará sus punteros intactos porque el módulo no conoce la geometría de una ventana ajena.\
Su ciclo de vida no pertenece a la librería: la ventana seguirá viva o muerta según decida su dueño.\
El handle queda liberado con `window::release`, que suelta solo el handle y deja la ventana ajena tranquila. `window::wait` y `window::destroy` también lo liberan, pero bloquean o piden el cierre; ninguno de los dos debe usarse a la fuerza sobre un handle propio.

### - ENG
Signature:
```cpp
void* active();
```
Return:\
**Handle Window** representing the window focused at that instant, whether belonging to the library or not.\
`nullptr` if the graphical environment is unavailable or no window holds the focus.

\
Behavior:\
It delivers a handle pointing to the window the system has in the foreground, be it of this program or of any other application.\
The handle is marked as external, a condition checked with `window::is_extern`.\
An external handle admits `window::gettitle`, `window::retitle`, `window::father`, `window::resize`, `window::show`, `window::focus`, `window::destroy`, `window::wait`, and `window::sendmessage` with the `window_box::close` tag.\
It admits no drawing: `window::draw`, `window::pixels`, and `window::clear` will reject it, and `window::getsize` leaves its pointers untouched because the module does not know the geometry of a foreign window.\
Its life cycle does not belong to the library: the window stays alive or dead as its owner decides.\
The handle is released with `window::release`, which lets go of only the handle and leaves the foreign window undisturbed. `window::wait` and `window::destroy` release it too, but they block or ask for the closing; neither one must be used by force over one's own handle.
