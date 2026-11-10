# file_stream
### - ESP
Firma:
```cpp
void* file_stream(void* file, bool looping, float pitch, bool canIcloser = false);
```
Parámetros:
* **File**: Handle Fichero abierto con `file_mode::read` sobre un WAV de audio sin comprimir.
* **Looping**: con `true`, al llegar al final del audio vuelve a empezar sin parar.
* **Pitch**: tono con el que nace la reproducción; después se modifica con `sound::set_pitch`.
* **CanIcloser**: permiso para que el módulo cierre el fichero cuando la reproducción termina. Por defecto `false`, y entonces el Handle Fichero sigue abierto y su cierre le toca al programador.

Retorno:\
**Handle Sonido Hilo** con la reproducción ya en marcha.\
`nullptr` si el handle no es utilizable, si no está abierto en modo lectura, si la salida de audio no pudo construirse o si el hilo de alimentación no pudo nacer.

\
Comportamiento:\
Esta función monta ella sola una reproducción completa desde un fichero: toma de la cabecera del WAV los canales, la frecuencia y los bits por muestra, abre una salida con ese formato y lanza un hilo que le va entregando trozos de audio sin que el programa intervenga.\
Solo entiende WAV de audio sin comprimir con la cabecera estándar, donde los datos empiezan justo después de los primeros 44 bytes.\
Un fichero con trozos extra antes del audio, o con audio comprimido, se lee igualmente y suena como ruido: la función no avisa porque no comprueba el formato.\
El tamaño de cada entrega sale de la frecuencia y del tono, y resulta de alrededor de una décima de segundo; con dos turnos por delante hay unas dos décimas de audio dentro del handle en todo momento.\
Eso es lo que se pierde al detener, y eso es también el retraso con el que responden los cambios de volumen.\
La llamada espera a que la salida quede abierta y regresa con la reproducción corriendo; desde ese momento no bloquea y el programa puede seguir con lo suyo.\
Si al nacer no hay ninguna salida de audio conectada, la llamada regresa igual y el hilo arranca: sigue leyendo el fichero y entregando trozos que no suenan, así que la posición avanza aunque no haya quién escuche.\
Cuando alguien enchufe la salida, la reproducción tomará el audio donde el hilo hubiese llegado, y lo leído mientras tanto se pierde.\
El handle devuelto se maneja con el resto del módulo: `sound::stop` sirve de pausa y `sound::play` de reanudación desde el mismo punto del archivo.\
`sound::set_volume`, `sound::set_pitch` y `sound::set_master_volume` actúan sobre su salida.\
`sound::seek`, `sound::reload`, `sound::get_position`, `sound::get_duration`, `sound::fade` y `sound::get_spectrum` son exclusivas de esta clase de handle, y `sound::stream` en cambio rechaza cualquier dato: la alimentación ya tiene dueño.\
Cambiar el tono mientras suena obliga al hilo a reconstruir la salida, y esa reconstrucción corta el audio un instante.\
Con **Looping** el hilo rebobina el archivo cada vez que lo agota.\
Sin bucle, al terminar se despide: cierra la salida, conserva el sonido interno vivo y cierra el fichero solo si **CanIcloser** era `true`.\
Ese adiós se puede notar: mientras el programa no haya pausado con `sound::stop` ni la salida se haya ido, un `sound::playing` en `false` indica que el hilo ya se despidió.\
El handle sigue siendo consultable con todas las funciones del módulo hasta `sound::destroy`, que además espera al hilo antes de liberar nada.\
Si la salida se va en plena reproducción, el hilo no se detiene: sigue leyendo el fichero, ninguna entrega suena y la posición avanza, así que ese audio se pierde; cómo vuelve la salida lo cuenta `sound::stream`.\
Con **CanIcloser** en `false`, la norma para cerrar o destruir el Handle Fichero por fuera es esperar primero a ese `sound::playing` en `false`: hasta ese momento el hilo todavía lo está leyendo.

### - ENG
Signature:
```cpp
void* file_stream(void* file, bool looping, float pitch, bool canIcloser = false);
```
Parameters:
* **File**: Handle File opened with `file_mode::read` over an uncompressed audio WAV.
* **Looping**: with `true`, on reaching the end of the audio it starts over without stopping.
* **Pitch**: pitch the playback is born with; later it is changed with `sound::set_pitch`.
* **CanIcloser**: permission for the module to close the file when playback ends. Defaults to `false`, and then the Handle File stays open and closing it falls to the programmer.

Return:\
A **Handle Threaded Sound** with playback already running.\
`nullptr` if the handle is unusable, if it is not opened in read mode, if the audio output could not be built, or if the feeding thread could not be born.

\
Behavior:\
This function assembles a complete playback from a file on its own: it takes channels, frequency and bits per sample from the WAV header, opens an output in that format, and launches a thread that keeps handing it pieces of audio with no help from the program.\
It only understands WAV with uncompressed audio and the standard header, where data begins right after the first 44 bytes.\
A file with extra chunks before the audio, or with compressed audio, is read all the same and sounds as noise: the function does not warn because it does not check the format.\
The size of each delivery comes from the frequency and the pitch and works out to around a tenth of a second; with two turns ahead there are about two tenths of audio inside the handle at all times.\
That is what is lost on stopping, and that is also the delay with which volume changes respond.\
The call waits until the output is open and returns with playback running; from that moment it does not block and the program can go on with its own business.\
If no audio output is plugged in at birth, the call returns all the same and the thread starts: it keeps reading the file and handing over pieces that make no sound, so the position advances even though nobody is listening.\
Once somebody plugs an output in, playback takes the audio wherever the thread had gotten to, and what was read in the meantime is lost.\
The returned handle works with the rest of the module: `sound::stop` serves as pause and `sound::play` as resume from the same point of the file.\
`sound::set_volume`, `sound::set_pitch` and `sound::set_master_volume` act on its output.\
`sound::seek`, `sound::reload`, `sound::get_position`, `sound::get_duration`, `sound::fade` and `sound::get_spectrum` are exclusive to this kind of handle, while `sound::stream` refuses any data: the feeding already has an owner.\
Changing pitch while sounding forces the thread to rebuild the output, and that rebuild cuts the audio for an instant.\
With **Looping** the thread rewinds the file every time it drains it.\
Without looping, at the end it says goodbye: it closes the output, keeps the internal sound alive, and closes the file only if **CanIcloser** was `true`.\
That farewell can be noticed: as long as the program has not paused with `sound::stop` and the output has not been taken away, a `sound::playing` answering `false` means the thread already said goodbye.\
The handle stays queryable through every module function until `sound::destroy`, which also waits for the thread before releasing anything.\
If the output is taken away mid-playback, the thread does not stop: it keeps reading the file, no delivery sounds, and the position advances, so that audio is lost; how the output comes back is what `sound::stream` tells.\
With **CanIcloser** at `false`, the rule for closing or destroying the Handle File from outside is to wait for that `sound::playing` at `false` first: until then the thread is still reading it.

## Overloads
### - ESP
Firma:
```cpp
void* file_stream(const char* filepath, bool looping, float pitch);
```
Parámetros:
* **Filepath**: ruta del WAV de audio sin comprimir que desea reproducirse.
* **Looping**: con `true`, la reproducción se repite sin parar al llegar al final.
* **Pitch**: tono con el que nace la reproducción.

Retorno:\
**Handle Sonido Hilo** con la reproducción en marcha, o `nullptr` si la ruta no pudo abrirse o si el sonido no pudo construirse.

\
Comportamiento:\
Forma breve para disparar un WAV sin gestionar handles de fichero: abre la ruta en modo lectura por su cuenta y el permiso de cierre queda concedido internamente, de modo que el fichero se cierra solo cuando la reproducción termina.\
Si el sonido no llega a crearse, la ruta se cierra antes de responder `nullptr`.\
A cambio, el programador no recibe el Handle Fichero: no puede consultarlo, mover su puntero ni conservar la ruta abierta.\
Quien necesite eso usa la firma base.

### - ENG
Signature:
```cpp
void* file_stream(const char* filepath, bool looping, float pitch);
```
Parameters:
* **Filepath**: Path of the uncompressed audio WAV to be played.
* **Looping**: with `true`, playback repeats without stopping on reaching the end.
* **Pitch**: pitch the playback is born with.

Return:\
A **Handle Threaded Sound** with playback running, or `nullptr` if the path could not be opened or the sound could not be built.

\
Behavior:\
The brief form for firing a WAV without managing file handles: it opens the path in read mode on its own and the closing permission is granted internally, so the file closes by itself once playback ends.\
If the sound never gets created, the path is closed before answering `nullptr`.\
In exchange, the programmer receives no Handle File: the file cannot be queried, its I/O pointer moved, nor the path kept open.\
Whoever needs that uses the base signature.
