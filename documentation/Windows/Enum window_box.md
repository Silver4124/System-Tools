# Enum window_box
### - ESP
Este enumerador agrupa las etiquetas de eventos y mensajes que manejan las funciones del módulo `ST_IMPORT_WINDOW`.\
Cada etiqueta identifica una situación concreta que la librería entrega al **mailman** de la ventana (el callback registrado con `window::create`).\
El programador también puede inyectar etiquetas por su cuenta mediante `window::sendmessage`.\
El valor devuelto por el mailman debe ser una de las dos etiquetas de respuesta: `def` o `accepted`.\
La información acompañante de cada evento viaja en los argumentos `wp` y `lp` que recibe el callback; el significado de esos valores se detalla en cada etiqueta.

| Etiqueta del Enumerador | Valor | Evento o Significado |
| :---- | ---- | :---- |
| `window_box::def` | `0x0000` | Respuesta del mailman: no se hace nada con el evento, la librería lo atiende con su comportamiento por defecto. También indica ausencia de estado en algunas consultas. |
| `window_box::accepted` | `0x0001` | Respuesta del mailman: el evento fue procesado por el programador y la librería no debe intervenir. |
| `window_box::destroy` | `0x0002` | La ventana está siendo destruida. |
| `window_box::size` | `0x0005` | La ventana cambió de tamaño. |
| `window_box::activate` | `0x0006` | La ventana ganó o perdió el foco de entrada. |
| `window_box::paint` | `0x000F` | La ventana pide ser repintada: su búfer de píxeles debe volver a volcarse en pantalla. |
| `window_box::close` | `0x0010` | Se solicitó el cierre de la ventana. |
| `window_box::quit` | `0x0012` | Se solicita finalizar el reparto de eventos de la ventana. |
| `window_box::key_down` | `0x0100` | Se pulsó una tecla. |
| `window_box::key_up` | `0x0101` | Se liberó una tecla. |
| `window_box::mouse_move` | `0x0200` | El cursor del ratón se movió dentro de la ventana. |
| `window_box::lbutton_down` | `0x0201` | Se pulsó el botón izquierdo del ratón. |
| `window_box::lbutton_up` | `0x0202` | Se liberó el botón izquierdo del ratón. |
| `window_box::lbutton_hold` | `0x0203` | El botón izquierdo se mantiene pulsado mientras el cursor se mueve. |
| `window_box::rbutton_down` | `0x0204` | Se pulsó el botón derecho del ratón. |
| `window_box::rbutton_up` | `0x0205` | Se liberó el botón derecho del ratón. |
| `window_box::rbutton_hold` | `0x0206` | El botón derecho se mantiene pulsado mientras el cursor se mueve. |

En los eventos de teclado, la etiqueta de la tecla implicada viaja en `wp`.\
En los eventos de movimiento y botones del ratón, la posición del cursor dentro de la ventana viaja en `lp`, con la coordenada **x** en la mitad de bajo orden y la coordenada **y** en la de alto orden.\
En el evento `window_box::size`, las nuevas dimensiones de la ventana viajan en `lp` con el mismo reparto de orden.\
La librería puede reconocer además etiquetas numéricas que no aparecen en este enumerador: si el mailman devuelve `0`, el evento queda en manos del sistema y no se da por atendido.

### - ENG
This enumerator groups the event and message tags handled by the functions of the `ST_IMPORT_WINDOW` module.\
Each tag identifies a concrete situation that the library delivers to the window's **mailman** (the callback registered with `window::create`).\
The programmer can also inject tags on their own through `window::sendmessage`.\
The value returned by the mailman must be one of the two answer tags: `def` or `accepted`.\
The accompanying information of each event travels in the `wp` and `lp` arguments received by the callback; the meaning of those values is detailed on each tag.

| Enumerator Tag | Value | Event or Meaning |
| :---- | ---- | :---- |
| `window_box::def` | `0x0000` | Mailman answer: nothing is done with the event, the library handles it with its default behaviour. Also indicates absence of state in some queries. |
| `window_box::accepted` | `0x0001` | Mailman answer: the event was processed by the programmer and the library must not intervene. |
| `window_box::destroy` | `0x0002` | The window is being destroyed. |
| `window_box::size` | `0x0005` | The window changed size. |
| `window_box::activate` | `0x0006` | The window gained or lost input focus. |
| `window_box::paint` | `0x000F` | The window asks to be repainted: its pixel buffer must be blitted to the screen again. |
| `window_box::close` | `0x0010` | Closing the window was requested. |
| `window_box::quit` | `0x0012` | The dispatch of the window's events is asked to end. |
| `window_box::key_down` | `0x0100` | A key was pressed. |
| `window_box::key_up` | `0x0101` | A key was released. |
| `window_box::mouse_move` | `0x0200` | The mouse cursor moved inside the window. |
| `window_box::lbutton_down` | `0x0201` | The left mouse button was pressed. |
| `window_box::lbutton_up` | `0x0202` | The left mouse button was released. |
| `window_box::lbutton_hold` | `0x0203` | The left button stays pressed while the cursor moves. |
| `window_box::rbutton_down` | `0x0204` | The right mouse button was pressed. |
| `window_box::rbutton_up` | `0x0205` | The right mouse button was released. |
| `window_box::rbutton_hold` | `0x0206` | The right button stays pressed while the cursor moves. |

In keyboard events, the tag of the key involved travels in `wp`.\
In mouse movement and button events, the cursor position inside the window travels in `lp`, with the **x** coordinate in the low-order half and the **y** coordinate in the high-order half.\
In the `window_box::size` event, the window's new dimensions travel in `lp` using the same order split.\
The library also recognizes numeric tags missing from this enumerator: if the mailman returns `0`, the event is left in the system's hands and is not considered attended.
