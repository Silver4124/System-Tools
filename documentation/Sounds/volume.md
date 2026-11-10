# volume
### - ESP
Firma:
```cpp
bool volume(void* snd, float* vol);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuya ganancia desea leerse.
* **Vol**: destino `float*` donde se deja el valor leído.

Retorno:\
`true` si **Vol** no es `nullptr` y el handle es utilizable, con el valor ya escrito.\
`false` si **Vol** es `nullptr`, o si el handle es nulo o inválido.\
Cuando el handle resulta inválido y el destino sí era utilizable, **Vol** queda escrito en `0.0`.

\
Comportamiento:\
Esta función devuelve la ganancia propia del handle, la que `sound::set_volume` fijó: el valor con el que `sound::stream` multiplicará las próximas muestras.\
No es la ganancia efectiva: el volumen maestro, cuando existe, se multiplica aparte en el momento del copiado y no se refleja aquí.\
Para saber el producto hay que leer también `sound::master_volume`.\
Sobre un Handle Sonido Hilo se lee el valor vivo del sonido interno, así que durante un desvanecido de `sound::fade` la respuesta es el punto intermedio por el que va pasando la señal, no el destino final.\
Es una consulta que no altera nada ni espera: puede usarse dentro del bucle de producción sin afectar al ritmo de envío.

### - ENG
Signature:
```cpp
bool volume(void* snd, float* vol);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose gain is to be read.
* **Vol**: `float*` destination where the read value is left.

Return:\
`true` if **Vol** is not `nullptr` and the handle is usable, with the value already written.\
`false` if **Vol** is `nullptr`, or if the handle is null or invalid.\
When the handle turns out invalid and the destination was usable, **Vol** is left written at `0.0`.

\
Behavior:\
This function returns the handle's own gain, the one `sound::set_volume` fixed: the value with which `sound::stream` will multiply the next samples.\
It is not the effective gain: the master volume, when present, is multiplied separately at copy time and is not reflected here.\
To know the product, `sound::master_volume` must be read as well.\
Over a Handle Threaded Sound the live value of the internal sound is read, so during a `sound::fade` the answer is the intermediate point the signal is passing through, not the final destination.\
It is a query that changes nothing and waits for nothing: it can be used inside the production loop without affecting the sending rhythm.

## Overloads
### - ESP
Firma:
```cpp
float volume(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuya ganancia desea leerse.

Retorno:\
La ganancia propia del handle, o `0.0` cuando el handle es nulo o inválido.\
No hay salida de error: un handle apagado por silencio y un handle inválido responden igual.

\
Comportamiento:\
Forma breve de la firma base para cuando solo interesa el número y el handle se sabe válido.

### - ENG
Signature:
```cpp
float volume(void* snd);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose gain is to be read.

Return:\
The handle's own gain, or `0.0` when the handle is null or invalid.\
There is no error channel: a handle muted to silence and an invalid handle answer the same.

\
Behavior:\
The brief form of the base signature, for when only the number matters and the handle is known to be valid.
