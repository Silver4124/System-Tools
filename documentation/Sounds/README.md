# 🔊 Sounds
```cpp
#define ST_IMPORT_SOUND
#include "System_Tools.h"
```
### - ESP
Este apartado abarca todo el conjunto de funciones de System Tools pertenecientes al `namespace sound`, enfocadas en la generación, reproducción y manipulación de audio por altavoz.\
El módulo proporciona una interfaz ligera e independiente del sistema operativo: el audio se representa mediante un **Handle Sonido**, los datos se entregan como muestras crudas ya codificadas, y la salida se alimenta con un par de búferes que el propio módulo intercambia para no interrumpir la reproducción.\
El módulo trabaja con dos clases de handle:\
* **Handle Sonido**: lo crea `sound::create` y representa una salida de audio con su formato, su ganancia y sus búferes de envío. Es el programador quien produce las muestras y las entrega con `sound::stream`.\
* **Handle Sonido Hilo**: lo crea `sound::file_stream` y representa un archivo WAV que se reproduce solo, alimentado por un hilo propio. Admite bucle, búsqueda, desvanecido y análisis.\
Un **Handle Sonido Hilo** contiene un **Handle Sonido** interno: casi todas las funciones del módulo aceptan cualquiera de los dos y actúan sobre la salida que corresponda, y `sound::is_threaded` es la forma de distinguirlos.
Para la lectura de esta sección, se asume que el lector comprende el muestreo de audio, la codificación de muestras en PCM y el intercambio de doble búfer.

Ciclo de uso de un **Handle Sonido**: `create` para reservar la salida, `set` para fijar el formato, `play` para abrirla, y luego un bucle que consulta `sync` y, cuando hay sitio, envía datos con `stream`; para terminar, `stop` o `destroy`.

Las funciones marcadas más abajo con *requieren archivo* necesitan además `ST_IMPORT_FILE`, porque operan sobre Handle Fichero o sobre el audio leído de disco.

A continuación se enlistan las funciones disponibles en este módulo:
* [`create`](create.md).
* [`stream`](stream.md).
* [`play`](play.md).
* [`stop`](stop.md).
* [`destroy`](destroy.md).
* [`sync`](sync.md).
* [`set`](set.md).
* [`playing`](playing.md).
* [`is_threaded`](is_threaded.md).
* [`set_volume`](set_volume.md).
* [`volume`](volume.md).
* [`set_pitch`](set_pitch.md).
* [`pitch`](pitch.md).
* [`set_master_volume`](set_master_volume.md).
* [`master_volume`](master_volume.md).

Requieren archivo:
* [`file_stream`](file_stream.md).
* [`reload`](reload.md).
* [`get_position`](get_position.md).
* [`get_duration`](get_duration.md).
* [`seek`](seek.md).
* [`get_spectrum`](get_spectrum.md).
* [`fade`](fade.md).
### - ENG
This section covers the entire set of System Tools functions belonging to the `namespace sound`, focused on the generation, playback and manipulation of speaker audio.\
The module provides a lightweight operating-system independent interface: audio is represented by a **Handle Sound**, data is handed over as already encoded raw samples, and the output is fed through a pair of buffers that the module itself swaps so playback never stutters.\
The module works with two kinds of handle:\
* **Handle Sound**: created by `sound::create`, it represents an audio output with its format, its gain and its sending buffers. The programmer produces the samples and delivers them with `sound::stream`.\
* **Handle Threaded Sound**: created by `sound::file_stream`, it represents a WAV file playing on its own, fed by its own thread. It supports looping, seeking, fading and analysis.\
A **Handle Threaded Sound** holds an internal **Handle Sound**: nearly every function of the module accepts either kind and acts on whichever output applies, and `sound::is_threaded` is how the two are told apart.
To read this section, it is assumed that the reader understands audio sampling, PCM sample encoding and double buffer swapping.

Usage cycle of a **Handle Sound**: `create` to reserve the output, `set` to fix its format, `play` to open it, then a loop that consults `sync` and, when there is room, sends data with `stream`; to finish, `stop` or `destroy`.

The functions marked below as *file required* additionally need `ST_IMPORT_FILE`, because they work on a Handle File or on audio read from disk.

Below is the list of available functions in this module:
* [`create`](create.md).
* [`stream`](stream.md).
* [`play`](play.md).
* [`stop`](stop.md).
* [`destroy`](destroy.md).
* [`sync`](sync.md).
* [`set`](set.md).
* [`playing`](playing.md).
* [`is_threaded`](is_threaded.md).
* [`set_volume`](set_volume.md).
* [`volume`](volume.md).
* [`set_pitch`](set_pitch.md).
* [`pitch`](pitch.md).
* [`set_master_volume`](set_master_volume.md).
* [`master_volume`](master_volume.md).

File required:
* [`file_stream`](file_stream.md).
* [`reload`](reload.md).
* [`get_position`](get_position.md).
* [`get_duration`](get_duration.md).
* [`seek`](seek.md).
* [`get_spectrum`](get_spectrum.md).
* [`fade`](fade.md).
