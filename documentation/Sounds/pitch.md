# pitch
### - ESP
Firma:
```cpp
bool pitch(void* snd, float* pit);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuyo tono desea leerse.
* **Pit**: destino `float*` donde se deja el multiplicador leído.

Retorno:\
`true` si **Pit** no es `nullptr` y el handle es utilizable, con el valor ya escrito.\
`false` si **Pit** es `nullptr`, o si el handle es nulo o inválido.\
Cuando el handle resulta inválido y el destino sí era utilizable, **Pit** queda escrito en `0.0`.

\
Comportamiento:\
Esta función devuelve el multiplicador guardado en el handle, el mismo que `sound::set_pitch` fijó y que `sound::play` aplica a la frecuencia al abrir la salida.\
Nace en `1.0`.\
Es la cifra pedida, no la confirmación de que la salida la haya adoptado: si el cambio sobre una salida ya abierta no llegó al dispositivo, aquí seguirá apareciendo el valor intentado.\
El resultado no es una frecuencia ni una nota: para saber a cuántas muestras por segundo se presenta el audio hay que multiplicarlo por la frecuencia configurada.\
Sobre un Handle Sonido Hilo se lee el tono del sonido interno, que es el que su hilo de alimentación vigila para decidir cuándo reconstruir la salida.

### - ENG
Signature:
```cpp
bool pitch(void* snd, float* pit);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose pitch is to be read.
* **Pit**: `float*` destination where the read multiplier is left.

Return:\
`true` if **Pit** is not `nullptr` and the handle is usable, with the value already written.\
`false` if **Pit** is `nullptr`, or if the handle is null or invalid.\
When the handle turns out invalid and the destination was usable, **Pit** is left written at `0.0`.

\
Behavior:\
This function returns the multiplier saved in the handle, the one `sound::set_pitch` fixed and the one `sound::play` applies to the frequency when opening the output.\
It is born at `1.0`.\
It is the figure requested, not confirmation that the output adopted it: if the change over an already open output never reached the device, the attempted value still shows up here.\
The result is neither a frequency nor a note: to know at how many samples per second audio is being presented, the multiplier has to be applied to the configured frequency.\
Over a Handle Threaded Sound the pitch read is the internal sound's, the very figure its feeding thread watches to decide when to rebuild the output.

## Overloads
### - ESP
Firma:
```cpp
float pitch(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuyo tono desea leerse.

Retorno:\
El multiplicador guardado, o `0.0` cuando el handle es nulo o inválido.\
Un handle cuyo tono fue puesto en `0.0` y un handle inválido responden igual.

\
Comportamiento:\
Forma breve de la firma base para el caso en que solo interesa el número.\
El `0.0` de un handle inválido no se separa del `0.0` de un tono realmente enmudecido: para distinguirlos está la firma base, cuyo `false` delata al handle inválido.

### - ENG
Signature:
```cpp
float pitch(void* snd);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose pitch is to be read.

Return:\
The saved multiplier, or `0.0` when the handle is null or invalid.\
A handle whose pitch was set to `0.0` and an invalid handle answer the same.

\
Behavior:\
The brief form of the base signature, for when only the number matters.\
The `0.0` of an invalid handle is not separable from the `0.0` of a genuinely muted pitch: to tell them apart, the base signature is the one whose `false` gives the invalid handle away.
