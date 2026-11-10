# yield
### - ESP
Firma:
```cpp
bool yield(void* wnd, bool _wait = true);
```
Parámetros:
* **Wnd**: Handle Ventana cuyo estado desea consultarse.
* **_wait**: Si es `true`, la función se detiene unos instantes antes de responder, lo que suaviza el consumo de procesador en un bucle de consulta repetida. Por defecto `true`.

Retorno:\
`true` mientras la ventana exista y siga en pie. `false` si el handle es `nullptr` o inválido, o si la ventana ya había cerrado.

\
Comportamiento:\
Esta función responde sin bloquear: dice si la ventana consultada sigue viva.\
Es la alternativa no bloqueante a `window::wait`, y sirve para descubrir que la ventana cerró por iniciativa del usuario sin tener que aguardar a ello.\
Consultar una ventana no la cierra, no modifica su estado ni libera su memoria.\
Cuando responde `false`, el handle ya apunta a una ventana destruida y no debe volverse a invocar: su liberación corresponde a `window::wait` o `window::destroy`, no a esta función.\
Mientras la ventana esté viva, el handle sigue siendo utilizable para cualquier otra función del módulo.\
Sobre un handle externo devuelto por `window::active`, la respuesta depende únicamente del estado registrado en el momento de crearlo, así que este detector no es fiable en ventanas ajenas; para ellas existe el bloqueo de `window::wait`.

### - ENG
Signature:
```cpp
bool yield(void* wnd, bool _wait = true);
```
Parameters:
* **Wnd**: Handle Window whose state is to be consulted.
* **_wait**: When `true`, the function pauses for a brief instant before answering, which softens the processor consumption of a repeated polling loop. Defaults to `true`.

Return:\
`true` while the window exists and is still standing. `false` if the handle is `nullptr` or invalid, or if the window had already closed.

\
Behavior:\
This function answers without blocking: it tells whether the window being consulted is still alive.\
It is the non-blocking alternative to `window::wait`, and serves to discover that the window closed on the user's initiative without having to await it.\
Consulting a window does not close it, modify its state, nor free its memory.\
When it answers `false`, the handle already points at a destroyed window and must not be invoked again: releasing it belongs to `window::wait` or `window::destroy`, not to this function.\
While the window is alive, the handle remains usable for any other module function.\
Over an external handle returned by `window::active`, the answer depends solely on the state recorded when the handle was created, so this detector is not reliable for foreign windows; the blocking of `window::wait` exists for them.