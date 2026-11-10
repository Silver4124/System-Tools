# set
### - ESP
Firma:
```cpp
void set(void* snd, unsigned int rate, BYTE channels, BYTE bits);
```
Parámetros:
* **Snd**: Handle Sonido que desea configurarse, o Handle Sonido Hilo (actúa sobre su sonido interno).
* **Rate**: frecuencia de muestreo, en muestras por segundo. `0` deja la vigente.
* **Channels**: canales por trama: `1` monofónico, `2` estéreo, más para envolventes multicanal. `0` deja el vigente.
* **Bits**: bits por muestra: `8`, `16` o `32`. `0` deja el vigente.

Comportamiento:\
Esta función declara cómo debe interpretarse cada byte que se entregue con `sound::stream` y con qué formato se abrirá la salida en `sound::play`: frecuencia, canales y ancho de muestra.\
Los tres valores definen juntos la trama: su tamaño en bytes es `canales × bits / 8`, y de él dependen tanto el número de muestras que caben en un turno de búfer como la forma en que `sound::stream` recorre los datos.\
Mientras el handle está sonando la configuración no cambia: la función se limita a no hacer nada.\
Un handle `nullptr` o inválido no produce ningún efecto.\
El orden correcto es `sound::stop`, `set`, `sound::play`.\
Un `0` en cualquiera de los tres parámetros significa "conserva el valor anterior", no "apaga ese canal"; por eso no puede fijarse una frecuencia de `0`, y conviene no enviar `0` en **Rate** esperando un borrón.\
Los anchos distintos de `8`, `16` y `32` quedan guardados, pero el recorrido de muestras no los sabe tratar: `sound::stream` aceptará el envío y no copiará nada.\
Con **Bits** `24`, por ejemplo, la salida sonará con el resto de la memoria del búfer.\
Cambiar el formato no redimensiona los búferes: la capacidad fijada en `sound::create` sigue siendo la misma y lo que cambia es cuántos milisegundos de audio caben en ella.\
Pasar de 16 a 32 bits, o de mono a estéreo, parte la duración de cada turno a la mitad.\
Los valores por defecto con los que nace el handle son 44100 Hz, 2 canales y 16 bits.\
Sobre un Handle Sonido Hilo el formato ya lo impuso el archivo WAV al crearse; modificarlo desde fuera desalinea la lectura de `sound::get_position` y `sound::get_duration` con el contenido real del fichero, así que esta función no debe usarse ahí.

### - ENG
Signature:
```cpp
void set(void* snd, unsigned int rate, BYTE channels, BYTE bits);
```
Parameters:
* **Snd**: Handle Sound to be configured, or Handle Threaded Sound (acts on its internal sound).
* **Rate**: sampling frequency, in samples per second. `0` keeps the current one.
* **Channels**: channels per frame: `1` mono, `2` stereo, more for multichannel surround. `0` keeps the current one.
* **Bits**: bits per sample: `8`, `16` or `32`. `0` keeps the current one.

Behavior:\
This function declares how each byte handed over with `sound::stream` must be read and with which format the output will be opened at `sound::play`: frequency, channels and sample width.\
The three values define the frame together: its size in bytes is `channels × bits / 8`, and from it depend both how many samples fit in a buffer turn and how `sound::stream` traverses the data.\
While the handle is sounding the configuration does not change: the function simply does nothing.\
A `nullptr` or invalid handle has no effect.\
The correct order is `sound::stop`, `set`, `sound::play`.\
A `0` in any of the three parameters means "keep the previous value", not "turn that channel off"; therefore a frequency of `0` cannot be set, and sending `0` in **Rate** expecting a blank is a mistake.\
Widths other than `8`, `16` and `32` are stored but the sample traversal does not know how to treat them: `sound::stream` will accept the delivery and copy nothing.\
With **Bits** `24`, for instance, the output sounds with the rest of the buffer's memory.\
Changing the format does not resize the buffers: the capacity fixed in `sound::create` stays the same and what changes is how many milliseconds of audio fit in it.\
Going from 16 to 32 bits, or from mono to stereo, cuts each turn's duration in half.\
The default values the handle is born with are 44100 Hz, 2 channels and 16 bits.\
Over a Handle Threaded Sound the format was already imposed by the WAV file when it was created; changing it from outside misaligns `sound::get_position` and `sound::get_duration` with the file's real content, so this function should not be used there.
