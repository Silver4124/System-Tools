# 🪟 Windows
```cpp
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
```
### - ESP
Este apartado abarca las funciones de System Tools del `namespace window`, enfocadas en la creación y gestión de ventanas gráficas, su superficie de píxeles, y el flujo de eventos de teclado y ratón.\
El módulo proporciona una interfaz ligera independiente del sistema operativo.\
La ventana se representa mediante un **Handle Ventana**.\
Los eventos se reparten a través de un callback (**mailman**).\
El contenido visible se produce dibujando en un búfer de píxeles que el propio módulo vuelca en pantalla.

A continuación se enlistan las funciones y estructuras disponibles en este módulo:
* [`Enum window_box`](Enum%20window_box.md).
* [`create`](create.md).
* [`defMailMan`](defMailMan.md).
* [`wait`](wait.md).
* [`destroy`](destroy.md).
* [`sendmessage`](sendmessage.md).
* [`draw`](draw.md).
* [`pixels`](pixels.md).
* [`clear`](clear.md).
* [`resize`](resize.md).
* [`getsize`](getsize.md).
* [`retitle`](retitle.md).
* [`gettitle`](gettitle.md).
* [`father`](father.md).
* [`getscreen`](getscreen.md).
* [`show`](show.md).
* [`focus`](focus.md).
* [`focuswait`](focuswait.md).
* [`mouse`](mouse.md).
* [`keyboard`](keyboard.md).
* [`active`](active.md).
* [`is_extern`](is_extern.md).
* [`release`](release.md).
* [`yield`](yield.md).
### - ENG
This section covers the System Tools functions of the `namespace window`, focused on creating and managing graphical windows, their pixel surface, and the flow of keyboard and mouse events.\
The module provides a lightweight operating-system independent interface.\
The window is represented by a **Handle Window**.\
Events are dispatched through a callback (**mailman**).\
The visible content is produced by drawing into a pixel buffer that the module itself blits to the screen.

Below is the list of available functions and structures in this module:
* [`Enum window_box`](Enum%20window_box.md).
* [`create`](create.md).
* [`defMailMan`](defMailMan.md).
* [`wait`](wait.md).
* [`destroy`](destroy.md).
* [`sendmessage`](sendmessage.md).
* [`draw`](draw.md).
* [`pixels`](pixels.md).
* [`clear`](clear.md).
* [`resize`](resize.md).
* [`getsize`](getsize.md).
* [`retitle`](retitle.md).
* [`gettitle`](gettitle.md).
* [`father`](father.md).
* [`getscreen`](getscreen.md).
* [`show`](show.md).
* [`focus`](focus.md).
* [`focuswait`](focuswait.md).
* [`mouse`](mouse.md).
* [`keyboard`](keyboard.md).
* [`active`](active.md).
* [`is_extern`](is_extern.md).
* [`release`](release.md).
* [`yield`](yield.md).
