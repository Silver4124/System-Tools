# getcolor
### - ESP
Firma:
```cpp
void* getcolor(void* cnsl, unsigned int* _bg, unsigned int* _fg);
```
Parámetros:
* **Cnsl**: Handle Consola cuyos colores corrientes se consultarán, o `nullptr` para usar el handle único del programa.
* **_Bg**: dirección del `unsigned int` donde quedará el color de fondo, en el formato `0x00BBGGRR`, o `nullptr` si el fondo no interesa.
* **_Fg**: dirección del `unsigned int` donde quedará el color de primer plano, en el mismo formato, o `nullptr` si el texto no interesa.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si la consulta se resolvió.\
`nullptr` si el handle es inválido.\
Con **_Bg** y **_Fg** en `nullptr` devuelve el handle sin escribir en ninguna parte.

\
Comportamiento:\
Devuelve los colores que el handle tiene anotados como corrientes, que son los últimos puestos por `console::setcolor`, o los que `console::get_handle` capturó al abrir la consola si nadie los cambió todavía.\
La consulta no pregunta ni a la consola ni al terminal: lee la memoria del handle, así que es inmediata y funciona con la salida redirigida. Su contrapartida es que el dato es una declaración, no una medición: si otro programa tiñó la consola por detrás, el handle sigue respondiendo lo suyo.\
Las dos cifras salen en el mismo orden y formato que `console::setcolor` acepta, así que se pueden tomar y devolver tal cual.

### - ENG
Signature:
```cpp
void* getcolor(void* cnsl, unsigned int* _bg, unsigned int* _fg);
```
Parameters:
* **Cnsl**: Console Handle whose current colors are to be consulted, or `nullptr` to use the program's unique handle.
* **_Bg**: address of the `unsigned int` where the background color will be left, in the `0x00BBGGRR` format, or `nullptr` if the background is not wanted.
* **_Fg**: address of the `unsigned int` where the foreground color will be left, in the same format, or `nullptr` if the text color is not wanted.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the query resolved.\
`nullptr` if the handle is invalid.\
With **_Bg** and **_Fg** at `nullptr` it returns the handle without writing anywhere.

\
Behavior:\
It returns the colors the handle has noted down as the current ones, which are the last ones `console::setcolor` left in place, or the ones `console::get_handle` captured when the console opened if nobody has changed them yet.\
The query asks neither the console nor the terminal: it reads the handle's memory, so it is immediate and works with output redirected. Its counterpart is that the figure is a declaration, not a measurement: if some other program dyed the console behind its back, the handle keeps answering its own.\
Both figures come out in the same order and format `console::setcolor` accepts, so they can be taken and handed back as they are.
