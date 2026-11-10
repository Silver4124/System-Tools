# retitle
### - ESP
Firma:
```cpp
void* retitle(void* wnd, const char* title);
```
Parámetros:
* **Wnd**: Handle Ventana a la que se cambiará el encabezado.
* **Title**: Nuevo texto del encabezado. Se aplica cadena vacía si es `nullptr`.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido, si la ventana ya había cerrado o si el encabezado no pudo actualizarse.

\
Comportamiento:\
Esta función reemplaza el texto que encabeza la ventana (el mismo que se entregó a `window::create`), sin crear una ventana nueva ni perder su contenido.\
El cambio es inmediato y no genera ningún evento en la cola: el mailman no es notificado.\
No hay límite de longitud impuesto por el módulo, aunque el espacio visible lo decide el sistema; un encabezado muy largo puede quedar recortado en pantalla.\
Sobre un handle externo devuelto por `window::active`, la función intenta renombrar la ventana ajena: el texto se transmite, pero quien decide mostrarlo es el dueño de esa ventana.\
El encabezado resultante puede leerse en cualquier momento con `window::gettitle`.

### - ENG
Signature:
```cpp
void* retitle(void* wnd, const char* title);
```
Parameters:
* **Wnd**: Handle Window whose heading will be changed.
* **Title**: New heading text. An empty string is applied when it is `nullptr`.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid, the window had already closed, or the heading could not be updated.

\
Behavior:\
This function replaces the text heading the window (the same one handed to `window::create`) without creating a new window nor losing its content.\
The change is immediate and produces no event in the queue: the mailman is not notified.\
The module imposes no length limit, though the visible space is decided by the system; a very long heading may end up trimmed on screen.\
Over an external handle returned by `window::active`, the function tries to rename the foreign window: the text is conveyed, but the owner of that window decides whether to show it.\
The resulting heading can be read at any time with `window::gettitle`.
