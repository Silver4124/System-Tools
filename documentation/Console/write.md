# write
### - ESP
Firma:
```cpp
UWORD write(void* cnsl, const wchar* text, UWORD len);
```
Parámetros:
* **Cnsl**: Handle Consola donde se escribirá, o `nullptr` para usar el handle único del programa.
* **Text**: dirección del texto ancho que se volcará. No se modifica, no se libera y no se espera que esté terminado en cero.
* **Len**: cantidad de unidades de `wchar` que se volcarán desde **Text**. No se cuenta ni se añade ningún terminador.

Retorno:\
cantidad de unidades volcadas de verdad.\
`0` si el handle es inválido, si **Text** es `nullptr`, si **Len** vale `0` o si la consola no tiene una salida utilizable.\
Una cifra menor que **Len** significa que la consola aceptó el texto a medias.

\
Comportamiento:\
Vuelca el texto en el punto donde esté el cursor y deja el cursor justo después de lo último escrito. No agrega espacios ni saltos de línea, y no consulta el color vigente: `console::write(nullptr, L"a", 1)` escribe exactamente una `a`.\
**Len** es lo que permite volcar textos que no terminan en cero (un trozo de búfer, una vista dentro de un JSON), sin que la función recorra la cadena.\
La escritura no pasa por los búferes de la biblioteca C, así que mezclada con `printf` o `puts` sobre la misma terminal el orden visible puede no ser el orden de las llamadas.\
El texto puede llevar secuencias de escape ANSI; la consola las interpreta según su propia configuración. Para el color está `console::setcolor`, y para mover el cursor `console::gotoxy`.\
Nada de lo escrito se puede deshacer: borrar una zona de pantalla exige reescribir encima.

### - ENG
Signature:
```cpp
UWORD write(void* cnsl, const wchar* text, UWORD len);
```
Parameters:
* **Cnsl**: Console Handle where the text goes, or `nullptr` to use the program's unique handle.
* **Text**: address of the wide text to be dumped. It is neither modified nor freed, and it is not expected to be zero-terminated.
* **Len**: amount of `wchar` units to be dumped from **Text**. No terminator is counted and none is added.

Return:\
amount of units actually dumped.\
`0` if the handle is invalid, **Text** is `nullptr`, **Len** is `0`, or the console has no usable output.\
A figure lower than **Len** means the console took the text halfway.

\
Behavior:\
It dumps the text wherever the cursor stands and leaves the cursor right after the last thing written. It appends no spaces and no line breaks, and it consults no current color: `console::write(nullptr, L"a", 1)` writes exactly one `a`.\
**Len** is what makes it possible to dump texts that are not zero-terminated (a stretch of a buffer, a view inside a JSON) without the function walking the string.\
The writing does not go through the C library buffers, so mixed with `printf` or `puts` over the same terminal the visible order may not be the order of the calls.\
The text may carry ANSI escape sequences; the console interprets them according to its own configuration. For color there is `console::setcolor`, and for moving the cursor `console::gotoxy`.\
Nothing written can be undone: erasing a stretch of screen means rewriting over it.

## Overloads
### - ESP
Firma:
```cpp
UWORD write(void* cnsl, const char* text, UWORD len);
```
Ruta de byte en byte: la página de código de la consola decide cómo se traducen los bytes, así que un texto UTF-8 sobre una página heredada se ve partido. **Len** cuenta bytes, no caracteres.\

Firma:
```cpp
UWORD write(void* cnsl, const void* text, UWORD len);
```
Trata **Text** como `const wchar*` y delega en la firma base.\

Firma:
```cpp
UWORD write(void* cnsl, const wchar* text);
UWORD write(void* cnsl, const char* text);
UWORD write(void* cnsl, const void* text);
```
Sin **Len**: recorren la cadena hasta su terminador para averiguar el largo y delegan en la firma con longitud. Responden `0` inmediato si **Text** es `nullptr`.\

Firma:
```cpp
UWORD write(void* cnsl, decltype(nullptr), UWORD len);
UWORD write(void* cnsl, decltype(nullptr));
```
Responden `0` siempre, sin escribir nada ni consultar la consola, sea cual sea **Len**.

### - ENG
Signature:
```cpp
UWORD write(void* cnsl, const char* text, UWORD len);
```
The byte-by-byte route: the console's code page decides how the bytes are translated, so UTF-8 text over a legacy page shows in pieces. **Len** counts bytes, not characters.\

Signature:
```cpp
UWORD write(void* cnsl, const void* text, UWORD len);
```
Treats **Text** as `const wchar*` and delegates to the base signature.\

Signature:
```cpp
UWORD write(void* cnsl, const wchar* text);
UWORD write(void* cnsl, const char* text);
UWORD write(void* cnsl, const void* text);
```
Without **Len**: they walk the string to its terminator to find the length and delegate to the length-taking signature. They answer `0` right away if **Text** is `nullptr`.\

Signature:
```cpp
UWORD write(void* cnsl, decltype(nullptr), UWORD len);
UWORD write(void* cnsl, decltype(nullptr));
```
They answer `0` always, writing nothing and consulting no console, whatever **Len** may be.
