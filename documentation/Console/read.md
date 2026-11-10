# read
### - ESP
Firma:
```cpp
void* read(void* cnsl, wchar** _text, UWORD* _len);
```
Parámetros:
* **Cnsl**: Handle Consola de la que se leerá, o `nullptr` para usar el handle único del programa.
* **_Text**: dirección del puntero donde quedará el texto leído. La función lo realoca, y lo libera cuando no hay lectura, así que debe ser `nullptr` la primera vez o una dirección devuelta por `memory::alloc` o `memory::realloc`. Cada llamada reemplaza lo anterior, nunca añade.
* **_Len**: dirección del `UWORD` donde quedará la cantidad de unidades leídas, o `nullptr` si solo interesa el texto.

Retorno:\
el handle entregado (o el handle único, si se entregó `nullptr`) cuando la lectura se resolvió, haya texto o no.\
`nullptr` si el handle es inválido o si **_Text** y **_Len** son `nullptr` a la vez.\
Sin lectura devuelve el handle igual, deja `*_Len` en `0` y deja `*_Text` en `nullptr`, que es la forma de distinguir "leí vacío" de "el handle no vale".

\
Comportamiento:\
Detiene el hilo hasta que la terminal tiene una línea completa, o sea hasta que se pulsa Enter. Lo tecleado se ve en pantalla porque la terminal hace eco por sí sola, no porque la función escriba nada.\
El texto se corta en 1023 unidades: una línea más larga se parte y el resto queda pendiente en la terminal para la llamada siguiente. El salto de línea no se quita, forma parte del texto devuelto.\
La memoria donde queda el texto es del módulo: su capacidad se consulta con `memory::szlen(*_text)` y su liberación puede hacerse con `memory::free` o dejarse en manos de la siguiente llamada.\
Lo leído desaparece de la cola de la terminal, así que la función no sirve como sondeo: no hay forma de preguntar si hay algo tecleado sin bloquearse.

### - ENG
Signature:
```cpp
void* read(void* cnsl, wchar** _text, UWORD* _len);
```
Parameters:
* **Cnsl**: Console Handle to read from, or `nullptr` to use the program's unique handle.
* **_Text**: address of the pointer where the read text will be left. The function reallocates that pointer, and frees it when there is no reading, so it must be `nullptr` the first time or an address returned by `memory::alloc` or `memory::realloc`. Each call replaces what came before, it never appends.
* **_Len**: address of the `UWORD` where the amount of units read will be left, or `nullptr` if only the text is wanted.

Return:\
the handle handed over (or the unique handle, if `nullptr` was handed) once the reading resolved, with text or without.\
`nullptr` if the handle is invalid, or if **_Text** and **_Len** are `nullptr` at the same time.\
Without a reading the handle comes back all the same, `*_Len` is left at `0` and `*_Text` is left at `nullptr`, which is how "I read nothing" is told apart from "the handle is worthless".

\
Behavior:\
It stops the thread until the terminal has a whole line ready, that is until Enter is pressed. What is typed shows on screen because the terminal echoes on its own, not because the function writes anything.\
The text is cut at 1023 units: a longer line splits and the rest stays pending in the terminal for the next call. The line break is not stripped, it is part of the returned text.\
The buffer where the text lands is the module's memory: its capacity is queried with `memory::szlen(*_text)` and its release can be done with `memory::free` or left to the next call.\
Whatever is read vanishes from the terminal queue, so the function is no good as a probe: there is no way to ask whether something is typed without blocking.

## Overloads
### - ESP
Firma:
```cpp
void* read(void* cnsl, char** _text, UWORD* _len);
```
Ruta de byte en byte: lo recibido depende de la página de código de entrada de la consola. **_Len** cuenta bytes, no caracteres, así que con una codificación multibyte la cifra puede ser mayor que lo tecleado.\

Firma:
```cpp
void* read(void* cnsl, void** _text, UWORD* _len);
```
Toma la ruta ancha y reinterpreta el resultado como `void*`; la capacidad de la memoria devuelta sigue midiéndose con `memory::szlen`.\

Firma:
```cpp
void* read(void* cnsl, decltype(nullptr), UWORD* _len);
```
Responde `nullptr` siempre, sin intentar la lectura.

### - ENG
Signature:
```cpp
void* read(void* cnsl, char** _text, UWORD* _len);
```
The byte-by-byte route: what comes back depends on the console's input code page. **_Len** counts bytes, not characters, so under a multibyte encoding the figure may be larger than what was typed.\

Signature:
```cpp
void* read(void* cnsl, void** _text, UWORD* _len);
```
It takes the wide path and reinterprets the result as `void*`; the capacity of the returned memory is still measured with `memory::szlen`.\

Signature:
```cpp
void* read(void* cnsl, decltype(nullptr), UWORD* _len);
```
It answers `nullptr` always, without attempting the reading.
