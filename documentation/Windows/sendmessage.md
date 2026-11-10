# sendmessage
### - ESP
Firma:
```cpp
void* sendmessage(void* wnd, UWORD msg, UWORD wp, WORD lp);
```
Parámetros:
* **Wnd**: Handle Ventana destino del mensaje.
* **Msg**: Etiqueta del evento a inyectar, tomada de [[[Enum window_box]]](Enum%20window_box.md).
* **Wp**: Complemento corto del mensaje; su significado depende de **Msg**.
* **Lp**: Complemento largo del mensaje; su significado depende de **Msg**.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido o si la ventana ya había cerrado.

\
Comportamiento:\
Esta función mete un evento en la cola de la ventana como si el sistema lo hubiera producido.\
Es la vía para repintar, redimensionar o cerrar una ventana por iniciativa propia.\
El mailman se entera de lo inyectado y puede responderle: `window_box::def` deja que la librería termine de atender el evento, `window_box::accepted` lo da por resuelto y la librería se retira.\
Que la llamada regrese no garantiza que el evento ya esté procesado.\
Las etiquetas de eventos de ratón llevan en **Lp** la posición dentro de la ventana (**x** en la mitad de bajo orden, **y** en la de alto orden).\
Las de teclado llevan en **Wp** la etiqueta de la tecla.\
La etiqueta `window_box::size` lleva en **Lp** las nuevas dimensiones con el mismo reparto.\
Una dimensión en `0` cuenta como minimización en Windows; en Linux esa señal la entrega el gestor de ventanas, así que un `0` inyectado a mano no minimiza.\
Inyectar `window_box::paint` vuelca el búfer de píxeles en pantalla; es la forma de hacer visible todo lo escrito con `window::draw` o `window::clear`.\
Inyectar `window_box::close` cierra la ventana en ambas plataformas, y `window_box::destroy` o `window_box::quit` dan por terminado el reparto de sus eventos.\
Terminado el reparto, el handle deja de servir: en Windows la ventana puede quedar en pantalla hasta que se destruya, y en Linux desaparece porque el propio reparto es lo que la mantiene viva.\
En Linux, `window_box::destroy` y `window_box::quit` llegan al mailman como `window_box::close`, que es la única señal de cierre que el reparto reconoce.\
El cierre es la excepción al trato de `window_box::accepted`: en Windows el mailman lo puede frenar, en Linux la ventana se cae aunque el programador responda.\
Sobre un handle externo devuelto por `window::active`, la función transmite la solicitud de cierre a la ventana ajena, y puede que el resto de etiquetas no produzcan efecto alguno.\
La tabla de eventos está en [[[Enum window_box]]](Enum%20window_box.md).

### - ENG
Signature:
```cpp
void* sendmessage(void* wnd, UWORD msg, UWORD wp, WORD lp);
```
Parameters:
* **Wnd**: Destination Handle Window of the message.
* **Msg**: Event tag to inject, taken from [[[Enum window_box]]](Enum%20window_box.md).
* **Wp**: Short complement of the message; its meaning depends on **Msg**.
* **Lp**: Long complement of the message; its meaning depends on **Msg**.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid or the window had already closed.

\
Behavior:\
This function drops an event into the window's queue as if the system had produced it.\
It is the way to repaint, resize, or close a window on one's own initiative.\
The mailman is told about the injected event and may answer it: `window_box::def` lets the library finish attending the event, `window_box::accepted` counts it as settled and the library steps back.\
That the call returns does not guarantee the event was already processed.\
Mouse event tags carry the position inside the window in **Lp** (**x** in the low-order half, **y** in the high-order one).\
Keyboard tags carry the key tag in **Wp**.\
The `window_box::size` tag carries the new dimensions in **Lp** with the same split.\
A `0` dimension counts as minimization on Windows; on Linux that signal comes from the window manager, so a hand-injected `0` does not minimize.\
Injecting `window_box::paint` blits the pixel buffer onto the screen; it is the way to make visible everything written with `window::draw` or `window::clear`.\
Injecting `window_box::close` closes the window on both platforms, and `window_box::destroy` or `window_box::quit` end the dispatch of its events.\
Once the dispatch ends the handle stops working: on Windows the window may stay on screen until it is destroyed, on Linux it disappears because the dispatch itself is what keeps it alive.\
On Linux, `window_box::destroy` and `window_box::quit` reach the mailman as `window_box::close`, the only closing signal the dispatch recognizes.\
Closing is the exception to the `window_box::accepted` treatment: on Windows the mailman can hold it back, on Linux the window falls even when the programmer answers.\
Over an external handle returned by `window::active`, the function conveys the closing request to the foreign window, and the remaining tags may produce no effect at all.\
The event table is in [[[Enum window_box]]](Enum%20window_box.md).
