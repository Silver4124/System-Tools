# getxy
### - ESP
Firma:
```cpp
void* getxy(void* cnsl, UWORD* _x, UWORD* _y);
```
Parámetros:
* **Cnsl**: Handle Consola cuya posición de cursor se consultará, o `nullptr` para usar el handle único del programa.
* **_X**: dirección del `UWORD` donde quedará la columna del cursor, o `nullptr` si la columna no interesa.
* **_Y**: dirección del `UWORD` donde quedará la fila del cursor, o `nullptr` si la fila no interesa.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) si la posición se consiguió; las cifras quedan escritas solo en los punteros entregados.\
`nullptr` si el handle es inválido, si la consola no tiene una entrada y salida utilizables o si la posición no se pudo averiguar.\
Con **_X** y **_Y** en `nullptr` devuelve el handle sin preguntar nada.

\
Comportamiento:\
Devuelve dónde está el cursor, en el mismo sistema de coordenadas que `console::gotoxy` acepta, así que una pareja tomada aquí se puede devolver allí tal cual.\
En Windows la posición sale del búfer de pantalla completo y no cambia al desplazar la ventana; en los sistemas tipo UNIX se le pregunta al terminal, lo que exige que la terminal conteste: con la salida redirigida, o con un terminal que no responde a la pregunta, la función devuelve `nullptr`.\
Mientras dura la pregunta en los sistemas tipo UNIX la entrada queda sin eco y sin espera de Enter, y lo tecleado en ese instante se puede perder. Por eso no debe convivir con un `console::read` pendiente sobre la misma terminal.

### - ENG
Signature:
```cpp
void* getxy(void* cnsl, UWORD* _x, UWORD* _y);
```
Parameters:
* **Cnsl**: Console Handle whose cursor position is to be consulted, or `nullptr` to use the program's unique handle.
* **_X**: address of the `UWORD` where the cursor's column will be left, or `nullptr` if the column is not wanted.
* **_Y**: address of the `UWORD` where the cursor's row will be left, or `nullptr` if the row is not wanted.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) if the position was obtained; the figures are written only into the pointers handed over.\
`nullptr` if the handle is invalid, the console has no usable input and output, or the position could not be found.\
With **_X** and **_Y** at `nullptr` it returns the handle without asking anything.

\
Behavior:\
It returns where the cursor stands, in the same coordinate system `console::gotoxy` accepts, so a pair taken here can be handed back there as it is.\
On Windows the position comes from the whole screen buffer and does not change when the window scrolls; on UNIX-like systems the terminal is asked, which requires the terminal to answer: with output redirected, or with a terminal that does not answer the question, the function returns `nullptr`.\
While the question lasts on UNIX-like systems the input is left without echo and without the wait for Enter, and whatever is typed in that instant can be lost. That is why it should not coexist with a pending `console::read` over the same terminal.
