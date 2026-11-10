# release
### - ESP
Firma:
```cpp
void* release(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana externo cuyo almacenamiento se desea liberar.

Retorno:\
El mismo handle entregado, cuya dirección queda liberada y deja de ser utilizable para cualquier función del módulo.\
`nullptr` si el handle es `nullptr` o inválido, y también si representa a una ventana propia creada con `window::create`.

\
Comportamiento:\
Esta función libera únicamente el handle, sin dirigir mensaje alguno a la ventana que observa. La ventana ajena sigue viva o muerta según decida su dueño.\
Está pensado para los handles que entrega `window::active` cuando el programa no quiere ni esperar ni cerrar la ventana: `window::wait` bloquea hasta que la ventana desaparezca del sistema, y `window::destroy` le pide el cierre a la ventana ajena.\
Sobre un handle propio devuelve `nullptr` y no libera nada: ese handle lo destruye el hilo de eventos que lo acompaña, y liberarlo a mano dejaría a ese hilo leyendo memoria ajena. Para un handle propio existen `window::wait` y `window::destroy`.\
Es la forma recomendada de soltar el handle cuando el programa lo ha observado solo una vez y ya no lo necesita, y sustituye a dejarlo olvidado, que fuga memoria.

### - ENG
Signature:
```cpp
void* release(void* wnd);
```
Parameters:
* **Wnd**: External Handle Window whose storage is to be freed.

Return:\
The very same handle, whose address is released and stops being usable for any module function.\
`nullptr` if the handle is `nullptr` or invalid, and also if it represents a one's own window created with `window::create`.

\
Behavior:\
This function frees the handle only, sending no message at all to the window it observes. The foreign window stays alive or dead as its owner decides.\
It is meant for the handles `window::active` delivers when the program wants neither to wait nor to close the window: `window::wait` blocks until that window disappears from the system, and `window::destroy` asks the closing to the foreign window.\
Over one's own handle it returns `nullptr` and frees nothing: that handle is destroyed by the event thread that accompanies it, and freeing it by hand would leave that thread reading someone else's memory. For a one's own handle there are `window::wait` and `window::destroy`.\
It is the recommended way to let go of the handle when the program observed it only once and needs it no more, and it takes the place of leaving it forgotten, which leaks memory.
