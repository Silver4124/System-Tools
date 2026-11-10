# 🖥️ Console
```cpp
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
```
### - ESP
Este apartado abarca todo el conjunto de funciones de System Tools pertenecientes al `namespace console`, enfocadas en la escritura y lectura de texto en la terminal, el posicionamiento del cursor y el control del color y del modo de caracteres de la consola.\
El módulo proporciona una interfaz ligera e independiente del sistema operativo: la consola se representa mediante un **Handle Consola**, el texto se entrega como cadenas de `wchar` o de `char` ya codificadas, y el cursor y el color se materializan con la API propia de cada plataforma.\
El módulo trabaja con una sola clase de handle:\
* **Handle Consola**: lo entrega `console::get_handle` y representa la consola con la que el proceso fue abierto, junto con la memoria de sus colores y de su modo de caracteres. No se crea, no se destruye y no se copia: es un handle único para todo el programa.
Rasgos propios de este handle:\
* **Es único.** `console::get_handle` devuelve siempre la misma dirección, así que ninguna función del módulo reserva memoria que el programador deba liberar y no existe un equivalente a `sound::destroy` ni a `window::destroy`.\
* **Recuerda cómo estaba la consola.** Al construirse captura los colores y el modo de caracteres vigentes; `console::restorecolor` y `console::restoreunicode` los devuelven, y el handle los restituye solo cuando el programa termina.\
* **Puede entregarse implícitamente.** Casi todas las funciones aceptan `nullptr` en el puesto del handle y se sirven ellas mismas del handle único, así que `console::write(nullptr, "hola", 4)` funciona igual que `console::write(console::get_handle(), "hola", 4)`.\
* **Se comparte con el módulo de ventanas.** Con `ST_IMPORT_WINDOW` activado además de este, el Handle Consola lleva dentro una vista de ventana, y `window::create` lo acepta como madre para anclar la ventana gráfica a la propia consola.
Para la lectura de esta sección, se asume que el lector comprende el funcionamiento de una terminal de caracteres y la diferencia entre un texto ancho y un texto multibyte.

Convenios de la interfaz:\
* Las coordenadas de `console::gotoxy` y `console::getxy` van en celdas de caracteres, contadas desde `0`; no son píxeles.\
* Los colores de `console::setcolor` usan el mismo formato `0x00BBGGRR` de `window::draw`, con veinticuatro bits de color real.\
* La barrera mínima en Windows está en Windows 7, y el color real además exige una consola que interprete secuencias ANSI, propia de Windows 10 y posteriores. Antes de eso el módulo sigue funcionando, pero el color y el cursor pueden no materializarse.\
* En los sistemas tipo UNIX el handle intenta localizar el *tty* real de la terminal para leer y preguntar aunque la salida esté redirigida; si no lo encuentra trabaja sobre `stdout` y `stdin`, y las funciones que requieren respuesta de la terminal fallan.\
* La entrada es por líneas: `console::read` no devuelve el carácter en el momento en que se pulsa, sino cuando la terminal tiene una línea completa lista.

A continuación se enlistan las funciones disponibles en este módulo:
* [`get_handle`](get_handle.md).
* [`write`](write.md).
* [`read`](read.md).
* [`gotoxy`](gotoxy.md).
* [`getxy`](getxy.md).
* [`setcolor`](setcolor.md).
* [`getcolor`](getcolor.md).
* [`restorecolor`](restorecolor.md).
* [`setunicode`](setunicode.md).
* [`getunicode`](getunicode.md).
* [`restoreunicode`](restoreunicode.md).
### - ENG
This section covers the entire set of System Tools functions belonging to the `namespace console`, focused on writing and reading text in the terminal, cursor positioning, and control of the console's color and character mode.\
The module provides a lightweight operating-system independent interface: the console is represented by a **Console Handle**, text is handed over as strings of `wchar` or of `char` already encoded, and cursor and color are materialized with each platform's own API.\
The module works with a single kind of handle:\
* **Console Handle**: delivered by `console::get_handle`, it represents the console the process was opened with, along with the memory of its colors and its character mode. It is not created, not destroyed and not copied: it is a single handle for the whole program.
Traits proper to this handle:\
* **It is unique.** `console::get_handle` always returns the very same address, so no function of the module reserves memory the programmer must release and there is no equivalent to `sound::destroy` or `window::destroy`.\
* **It remembers how the console was.** On construction it captures the colors and character mode in effect; `console::restorecolor` and `console::restoreunicode` bring them back, and the handle restores them on its own when the program ends.\
* **It can be supplied implicitly.** Nearly every function accepts `nullptr` where the handle goes and helps itself to the unique handle, so `console::write(nullptr, "hola", 4)` works just like `console::write(console::get_handle(), "hola", 4)`.\
* **It is shared with the window module.** With `ST_IMPORT_WINDOW` active as well, the Console Handle carries a window view inside, and `window::create` accepts it as a father to anchor the graphical window onto the console itself.
To read this section, it is assumed that the reader understands how a character terminal works and the difference between wide text and multibyte text.

Interface conventions:\
* The coordinates of `console::gotoxy` and `console::getxy` are in character cells, counted from `0`; they are not pixels.\
* The colors of `console::setcolor` use the same `0x00BBGGRR` format as `window::draw`, with twenty-four bits of real color.\
* The minimum floor on Windows is Windows 7, and real color additionally asks for a console that interprets ANSI sequences, proper to Windows 10 and later. Before that the module still works, but color and cursor may fail to materialize.\
* On UNIX-like systems the handle tries to locate the terminal's real *tty* so it can read and enquire even when the output is redirected; if it fails it works over `stdout` and `stdin`, and the functions requiring an answer from the terminal fail.\
* Input comes by lines: `console::read` does not hand back the character the moment it is pressed, but when the terminal has a whole line ready.

Below is the list of available functions in this module:
* [`get_handle`](get_handle.md).
* [`write`](write.md).
* [`read`](read.md).
* [`gotoxy`](gotoxy.md).
* [`getxy`](getxy.md).
* [`setcolor`](setcolor.md).
* [`getcolor`](getcolor.md).
* [`restorecolor`](restorecolor.md).
* [`setunicode`](setunicode.md).
* [`getunicode`](getunicode.md).
* [`restoreunicode`](restoreunicode.md).
