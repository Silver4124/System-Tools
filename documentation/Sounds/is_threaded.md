# is_threaded
### - ESP
Firma:
```cpp
bool is_threaded(void* snd);
```
Parámetros:
* **Snd**: Handle que desea clasificarse.

Retorno:\
`true` únicamente cuando el puntero es un **Handle Sonido Hilo** creado por `sound::file_stream`.\
`false` para un **Handle Sonido** de `sound::create`, para `nullptr` y para handles ajenas al módulo de sonido.\
La función no devuelve nada más: no informa de si el handle está en servicio ni de si sigue siendo utilizable.

\
Comportamiento:\
Esta función resuelve la única duda que las demás funciones del módulo dejan abierta: de qué clase es el handle que se tiene entre manos.\
Casi todas aceptan las dos clases y deciden por su cuenta a qué salida actúan, así que conocer el tipo sirve para elegir el camino de administración.\
Un **Handle Sonido** se alimenta con `sound::stream` desde el propio programa; un **Handle Sonido Hilo** se alimenta solo desde su archivo y admite `sound::seek`, `sound::reload`, `sound::get_spectrum`, `sound::get_position`, `sound::get_duration` y `sound::fade`, que sobre un handle normal responderían `false` o `0` sin más.\
Es el criterio que permite escribir una función que reciba un `void*` de audio y reparta el trabajo: se pregunta aquí y se ramifica.\
No sustituye a ninguna comprobación de validez: un handle ya destruido, o un puntero cualquiera que por casualidad parezca del módulo, puede dar una respuesta con aspecto de válida.

### - ENG
Signature:
```cpp
bool is_threaded(void* snd);
```
Parameters:
* **Snd**: Handle to be classified.

Return:\
`true` only when the pointer is a **Handle Threaded Sound** created by `sound::file_stream`.\
`false` for a **Handle Sound** from `sound::create`, for `nullptr` and for handles belonging to modules other than sound.\
The function gives nothing else back: it reports neither whether the handle is in service nor whether it is still usable.

\
Behavior:\
This function resolves the one doubt the rest of the module leaves open: which kind of handle is at hand.\
Nearly every function accepts both kinds and decides on its own which output it acts on, so knowing the class is what lets one pick the line of administration.\
A **Handle Sound** is fed with `sound::stream` from the program itself; a **Handle Threaded Sound** feeds itself from its file and takes `sound::seek`, `sound::reload`, `sound::get_spectrum`, `sound::get_position`, `sound::get_duration` and `sound::fade`, which over a plain handle would just answer `false` or `0`.\
It is the criterion that allows writing a function taking a `void*` of audio and dispatching the work: ask here and branch.\
It replaces no validity check: an already destroyed handle, or any pointer that happens to look like one of the module's, can yield an answer that appears valid.
