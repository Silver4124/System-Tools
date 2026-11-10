# is_extern
### - ESP
Firma:
```cpp
bool is_extern(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana cuya naturaleza se desea conocer.

Retorno:\
`true` si el handle representa a una ventana ajena a la librería. `false` en caso contrario, y también cuando **Wnd** es `nullptr` o un handle inválido.\
Sobre un handle cuya ventana ya cerró, el dato leído puede haber dejado de tener sentido: la respuesta solo es fiable mientras el handle sigue siendo utilizable.

\
Comportamiento:\
Esta función distingue los dos orígenes posibles de un Handle Ventana: el de una ventana creada por el programa con `window::create`, y el de una ventana observada por `window::active` que pertenece a otra aplicación o al propio sistema.\
Es el comprobatorio previo recomendado antes de invocar funciones de dibujo o de geometría, pues un handle externo rechaza `window::draw`, `window::pixels` y `window::clear`, y `window::getsize` no entrega datos sobre él.\
También decide qué esperar de `window::wait`: sobre un handle externo aguarda a que la ventana ajena desaparezca del sistema, mientras que sobre un handle propio aguarda a que la ventana del programa cierre.\
Solo un handle externo puede liberarse a mano con `window::release`.\
No modifica el handle ni su ventana: es una consulta pura.

### - ENG
Signature:
```cpp
bool is_extern(void* wnd);
```
Parameters:
* **Wnd**: Handle Window whose nature is to be known.

Return:\
`true` if the handle represents a window foreign to the library. `false` otherwise, and also when **Wnd** is `nullptr` or an invalid handle.\
Over a handle whose window already closed, the datum read may have stopped making sense: the answer is only reliable while the handle remains usable.

\
Behavior:\
This function tells apart the two possible origins of a Handle Window: a window created by the program with `window::create`, and a window observed by `window::active` that belongs to another application or to the system itself.\
It is the recommended preceding check before invoking drawing or geometry functions, since an external handle rejects `window::draw`, `window::pixels`, and `window::clear`, and `window::getsize` delivers no data over it.\
It also decides what to expect from `window::wait`: over an external handle it awaits the disappearance of the foreign window from the system, while over one's own handle it awaits the closing of the program's window.\
Only an external handle can be released by hand with `window::release`.\
It modifies neither the handle nor its window: it is a pure query.
