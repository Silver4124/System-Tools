# create
### - ESP
Firma:
```cpp
void* create(unsigned int buffer_size);
```
Parámetros:
* **Buffer_size**: capacidad, en bytes, de cada uno de los dos búferes de envío del handle. La memoria total reservada para audio es el doble de esta cifra.

Retorno:\
**Handle Sonido** recién creado, con formato y ganancias por defecto: 44100 Hz, 2 canales, 16 bits por muestra, volumen `1.0`, tono `1.0` y sin volumen maestro.\
El handle nace detenido; `sound::play` es lo que lo pone a sonar.

\
Comportamiento:\
Esta función da a luz una salida de audio con sus dos búferes de envío ya vacíos, lista para que `sound::set` la configure y `sound::stream` la alimente.\
No abre ningún dispositivo: hasta `play` el handle solo existe en memoria, así que crear muchos handles no interfiere con otras aplicaciones de sonido.\
**Buffer_size** fija la granularidad con la que después se entregará el audio: `sound::stream` nunca acepta más bytes que esa cifra en una sola llamada, y cuando los dos búferes siguen sonando `sound::sync` responde `false` hasta que uno se libera.\
El tamaño acertado es el que guarda algunas decenas de milisegundos de audio.\
Se calcula como `frecuencia × canales × (bits / 8) × segundos`; con el formato por defecto, 44100 × 2 × 2 × 0.05 ≈ 8820 bytes caben media décima de segundo por búfer.\
Un búfer pequeño obliga a enviar datos con mucha frecuencia; uno grande deja más audio en espera dentro del handle, y ese retraso se nota al cambiar el volumen o al detener la salida.\
`buffer_size` en `0` produce un handle válido que jamás aceptará muestras: `sound::stream` siempre responderá `0`.\
El formato configurado en `sound::set` debe coincidir con el de los datos que se entreguen, porque el tamaño de muestra decide cuántas muestras caben en el búfer y cómo se les aplica la ganancia.\
La liberación corresponde a `sound::destroy`, que detiene la salida antes de soltar la memoria; después de eso el puntero deja de ser utilizable para el resto de funciones del módulo.

### - ENG
Signature:
```cpp
void* create(unsigned int buffer_size);
```
Parameters:
* **Buffer_size**: Capacity, in bytes, of each of the handle's two sending buffers. The memory reserved for audio is double this figure.

Return:\
The newly created **Handle Sound**, with default format and gains: 44100 Hz, 2 channels, 16 bits per sample, volume `1.0`, pitch `1.0` and no master volume.\
The handle is born stopped; `sound::play` is what sets it sounding.

\
Behavior:\
This function brings an audio output into existence with its two sending buffers already empty, ready for `sound::set` to configure it and `sound::stream` to feed it.\
It opens no device: before `play` the handle only exists in memory, so creating many handles does not interfere with other sound applications.\
**Buffer_size** fixes the granularity with which audio is handed over later: `sound::stream` never accepts more than that amount of bytes in a single call, and while both buffers are still sounding `sound::sync` answers `false` until one frees up.\
The right size is the one holding a few tens of milliseconds of audio.\
It is computed as `rate × channels × (bits / 8) × seconds`; with the default format, 44100 × 2 × 2 × 0.05 ≈ 8820 bytes fits half a tenth of a second per buffer.\
A small buffer forces data to be sent very often; a large one leaves more audio waiting inside the handle, and that delay shows up when changing volume or stopping the output.\
A `buffer_size` of `0` yields a valid handle that will never accept samples: `sound::stream` always answers `0`.\
The format configured with `sound::set` must match the data being handed over, because the sample width decides how many samples fit in the buffer and how gain is applied to them.\
Releasing it belongs to `sound::destroy`, which stops the output before freeing the memory; after that the pointer stops being usable for the rest of the module functions.

## Overloads
### - ESP
Firma:
```cpp
void* create(unsigned int buffer_size, float* mst_vol);
```
Parámetros:
* **Buffer_size**: capacidad, en bytes, de cada uno de los dos búferes de envío del handle.
* **Mst_vol**: dirección del `float` que hará de volumen maestro compartido, o `nullptr` para dejar el handle sin maestro.

Retorno:\
El mismo **Handle Sonido** que la firma base, con el volumen maestro ya enlazado cuando **Mst_vol** no es `nullptr`.

\
Comportamiento:\
Reserva la salida y le enlaza el maestro en un solo paso, sin necesidad de una llamada extra a `sound::set_master_volume`.\
El módulo no copia ni libera el valor: lo lee cada vez que `sound::stream` escala una muestra, así que un único `float` puede gobernar varios handles a la vez y reflejar al instante los cambios que el programa haga en él.\
La propiedad del `float` sigue siendo del programador, que debe mantenerlo vivo mientras algún handle lo apunte.\
Entregar `nullptr` equivale a la firma base.

### - ENG
Signature:
```cpp
void* create(unsigned int buffer_size, float* mst_vol);
```
Parameters:
* **Buffer_size**: Capacity, in bytes, of each of the handle's two sending buffers.
* **Mst_vol**: Address of the `float` acting as shared master volume, or `nullptr` to leave the handle without one.

Return:\
The same **Handle Sound** as the base signature, with the master volume already linked whenever **Mst_vol** is not `nullptr`.

\
Behavior:\
It reserves the output and links its master in one step, with no extra call to `sound::set_master_volume` needed.\
The module neither copies nor frees the value: it reads it every time `sound::stream` scales a sample, so a single `float` can rule many handles at once while staying live to any change the program makes on it.\
Ownership of the `float` remains with the programmer, who must keep it alive while some handle points at it.\
Handing over `nullptr` is equivalent to the base signature.
