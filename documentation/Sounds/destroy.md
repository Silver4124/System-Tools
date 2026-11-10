# destroy
### - ESP
Firma:
```cpp
void destroy(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo que desea eliminarse.

Comportamiento:\
Esta función apaga la salida y suelta la memoria del handle: sobre un Handle Sonido libera el formato, las ganancias y los dos búferes de envío.\
El puntero deja de ser utilizable para cualquier función del módulo en cuanto la llamada regresa.\
Si el handle aún estaba sonando, `destroy` lo detiene por su cuenta; no hace falta llamar antes a `sound::stop`.\
Sobre un Handle Sonido Hilo la petición es distinta: se anuncia el final de la reproducción y la llamada espera a que el hilo que alimentaba el audio termine de apagarse (recupera los búferes pendientes, cierra la salida y suelta el fichero si el handle se creó con permiso para hacerlo), y solo entonces libera el sonido interno y la memoria del handle.\
Sin ese permiso, el caso de la sobrecarga que recibe un Handle Fichero ya abierto, el fichero queda abierto y su liberación sigue siendo responsabilidad del programador.\
Un handle `nullptr`, uno inválido o uno de otro módulo no produce ningún efecto.\
Para pausar sin perder nada se usa `sound::stop`; `destroy` es el paso definitivo y no deja nada consultable.

### - ENG
Signature:
```cpp
void destroy(void* snd);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound to be eliminated.

Behavior:\
This function shuts the output down and frees the handle's memory: over a Handle Sound it releases the format, the gains and the two sending buffers.\
The pointer stops being usable for any module function as soon as the call returns.\
If the handle was still sounding, `destroy` stops it on its own; calling `sound::stop` beforehand is not needed.\
Over a Handle Threaded Sound the request differs: the end of playback is announced and the call waits for the thread that fed the audio to finish shutting down (it retrieves the pending buffers, closes the output and releases the file if the handle was created with permission to do so), and only then is the internal sound and the handle's memory freed.\
Without that permission, the case of the overload taking an already open Handle File, the file stays open and releasing it remains the programmer's responsibility.\
A `nullptr` handle, an invalid one, or one belonging to another module has no effect.\
To pause without losing anything one uses `sound::stop`; `destroy` is the definitive step and leaves nothing queryable.
