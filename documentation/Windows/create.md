# create
### - ESP
Firma:
```cpp
void* create(void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*, UWORD, UWORD, WORD));
```
Parámetros:
* **Father**: Handle Ventana de la ventana madre, `nullptr` si la ventana nacirá sin madre. Con el módulo de consola activado en Windows, también se puede entregar un Handle Consola para vincularla a la consola propia del programa.
* **Title**: Texto que encabezará la ventana. Puede modificarse después con `window::retitle`; se ignora si es `nullptr`.
* **Width**: Ancho, en píxeles, de la superficie dibujable de la ventana.
* **Height**: Alto, en píxeles, de la superficie dibujable de la ventana.
* **Mailman**: Callback que recibirá los eventos de la ventana. Si no se entrega, el módulo registra [`defMailMan`](defMailMan.md) si este está definido.

Retorno:\
**Handle Ventana** recién creado, en caso de éxito. `nullptr` si **Width** o **Height** son `0`, si **Mailman** es `nullptr`, si **Father** no es un handle utilizable, si el entorno gráfico no está disponible o si la ventana no pudo construirse.

\
Comportamiento:\
Esta función da a luz una ventana gráfica visible y entrega un **Handle Ventana** listo para ser operado por el resto de funciones del módulo.\
La ventana existe de forma independiente al hilo que la creó: `create` regresa en cuanto la ventana está en pie, sin bloquear.\
Para saber si sigue viva se usa `window::yield`, y para aguardar su cierre se usa `window::wait`.\
**Width** y **Height** describen la superficie dibujable, no el marco completo que el sistema añade alrededor; las dimensiones reales en pantalla pueden consultarse con `window::getsize`.\
El contenido visible se escribe primero en el búfer de píxeles de la ventana, y luego se vuelca en pantalla.\
El búfer se obtiene con `window::pixels` o se pinta por zonas con `window::draw`.\
El volcado se pide inyectando la etiqueta [[[Enum window_box]]](Enum%20window_box.md) `paint` mediante `window::sendmessage`.\
El argumento **Mailman** recibe las etiquetas de [[[Enum window_box]]](Enum%20window_box.md) y debe responder `window_box::def` para dejar que la librería atienda el evento, o `window_box::accepted` para darlo por procesado.\
Entregar un handle de ventana en **Father** vincula ambas ventanas: si la madre desaparece, la hija se cierra sin consultar a su mailman.\
`window::wait` y `window::destroy` destruyen el handle al consumarse el cierre, y tras ello el puntero deja de ser utilizable para el resto de funciones del módulo.

### - ENG
Signature:
```cpp
void* create(void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*, UWORD, UWORD, WORD));
```
Parameters:
* **Father**: Handle Window of the mother window, `nullptr` if the window will be born without a mother. With the console module active on Windows, a Handle Console can also be handed over to bind the window to the program's own console.
* **Title**: Text heading the window. It can be changed later with `window::retitle`; ignored when `nullptr`.
* **Width**: Width, in pixels, of the window's drawable surface.
* **Height**: Height, in pixels, of the window's drawable surface.
* **Mailman**: Callback receiving the window's events. When none is handed over, the module registers [`defMailMan`](defMailMan.md) if it is defined.

Return:\
The newly created **Handle Window** upon success. `nullptr` if **Width** or **Height** is `0`, if **Mailman** is `nullptr`, if **Father** is not a usable handle, if the graphical environment is unavailable, or if the window could not be built.

\
Behavior:\
This function brings a visible graphical window into existence and delivers a **Handle Window** ready to be operated by the rest of the module functions.\
The window exists independently of the thread that created it: `create` returns as soon as the window is standing, without blocking.\
Use `window::yield` to know whether it is still alive, and `window::wait` to await its closing.\
**Width** and **Height** describe the drawable surface, not the full frame the system adds around it; the real on-screen dimensions can be queried with `window::getsize`.\
Visible content is written into the window's pixel buffer first, then blitted to the screen.\
The buffer is obtained with `window::pixels` or painted region by region with `window::draw`.\
The blit is requested by injecting the [[[Enum window_box]]](Enum%20window_box.md) `paint` tag through `window::sendmessage`.\
The **Mailman** argument takes the tags of [[[Enum window_box]]](Enum%20window_box.md) and must answer `window_box::def` to let the library attend the event, or `window_box::accepted` to consider it processed.\
Handing over a window handle in **Father** binds both windows: if the mother disappears, the daughter closes without consulting its mailman.\
`window::wait` and `window::destroy` destroy the handle once the closure is consummated, after which the pointer stops being usable for the rest of the module functions.
