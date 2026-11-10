# set_volume
### - ESP
Firma:
```cpp
bool set_volume(void* snd, float vol);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuya ganancia desea fijarse.
* **Vol**: factor de ganancia. `0.0` silencio, `1.0` señal sin modificar, cifras mayores amplifican con recorte.

Retorno:\
`true` si la ganancia quedó registrada. `false` si el handle es `nullptr` o inválido.

\
Comportamiento:\
Esta función fija el factor con el que `sound::stream` multiplica cada muestra al copiarla, saturando el resultado al rango del formato: es una ganancia que se hornea en los datos, no un mando que actúe sobre el altavoz.\
La ganancia efectiva es el producto del volumen del handle por su volumen maestro, cuando lo tiene; ambos se aplican en la misma operación.\
El cambio rige desde la entrega siguiente.\
El audio ya enviado conserva la ganancia anterior y se escuchará con ella, así que el efecto se nota tras la latencia de los búferes: unos pocos de decenas o centenas de milisegundos, según el tamaño elegido en `sound::create`.\
No requiere detener la salida ni es sensible al estado de `sound::play`: puede modificarse mientras suena, y lo que ya suena no se retoca.\
El valor no se limita a la entrada: por encima de `1.0` las muestras se recortan y el sonido distorsiona, los negativos invierten la fase, y valores sin fin o no numéricos producen muestras sin sentido.\
Sobre un Handle Sonido Hilo la función fija el volumen y, a la vez, toma ese valor como destino del desvanecido y cancela el que estuviera en curso.\
Para un cambio suave se usa `sound::fade`; esta llamada es el corte directo.

### - ENG
Signature:
```cpp
bool set_volume(void* snd, float vol);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose gain is to be fixed.
* **Vol**: Gain factor. `0.0` silence, `1.0` untouched signal, larger figures amplify with clipping.

Return:\
`true` if the gain was recorded. `false` if the handle is `nullptr` or invalid.

\
Behavior:\
This function fixes the factor with which `sound::stream` multiplies every sample as it copies it, saturating the result to the format's range: it is gain baked into the data, not a knob working on the speaker.\
Effective gain is the product of the handle's volume and its master volume when it has one; both are applied in the same operation.\
The change rules from the next delivery on.\
Audio already sent keeps the previous gain and will sound with it, so the effect shows up after the buffers' latency: a few tens or hundreds of milliseconds, depending on the size chosen in `sound::create`.\
It needs no stopping of the output and is indifferent to the state of `sound::play`: it can be changed while sounding, and what is already sounding is not retouched.\
The value is not limited on input: above `1.0` samples clip and the sound distorts, negatives invert phase, and infinite or non numeric values produce meaningless samples.\
Over a Handle Threaded Sound the function sets the volume and, at the same time, takes that value as the fading target and cancels whatever was in progress.\
For a smooth change one uses `sound::fade`; this call is the direct cut.
