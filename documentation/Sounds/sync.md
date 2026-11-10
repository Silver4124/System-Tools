# sync
### - ESP
Firma:
```cpp
bool sync(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido cuyo turno de envío desea consultarse, o Handle Sonido Hilo.

Retorno:\
`true` cuando al menos uno de los dos búferes de envío está libre: hay sitio para `sound::stream`.\
`false` cuando los dos siguen en servicio y no cabe nada, o si el handle es `nullptr` o inválido.\
Con la salida cerrada responde `true`, porque ningún búfer está sonando, aunque `sound::stream` rechace datos hasta que haya `sound::play`.\
Una desconexión de la salida no deja turnos en prenda: `sound::stream` los recupera al notarla, que es donde está contada la desconexión, así que la consulta vuelve a responder `true` sin necesidad de `sound::stop`.

\
Comportamiento:\
Esta función mira cuánto camino recorrido llevan las entregas hechas y anuncia si queda un turno disponible.\
Es el semáforo del productor: no espera, no envía, no corta nada y responde al instante.\
Consultar también es lo que permite registrar que una entrega terminó.\
Un bucle que solo llama a `sound::stream` sin preguntar acaba atascado: cuando los dos búferes quedan marcados como ocupados, ninguna otra cosa los devuelve, así que la omisión de `sync` produce un audio que se corta aunque el equipo tenga sitio de sobra.\
El ritmo natural con dos turnos es mantenerlos llenos: enviar cuando `sync` responde `true` y esperar un poco cuando responde `false`.\
La espera le corresponde al programa, con la función `sleep` de las funciones por defecto; `sync` nunca bloquea.\
El tamaño del búfer elegido en `sound::create` decide con qué frecuencia se presentará esta pregunta: turnos cortos responden mucho y retrasan poco; turnos largos responden menos y acumulan más audio por delante.\
Sobre un Handle Sonido Hilo la consulta llega al sonido interno, pero ese turno ya lo administra su propio hilo, así que no suele hacer falta preguntarlo desde fuera.

### - ENG
Signature:
```cpp
bool sync(void* snd);
```
Parameters:
* **Snd**: Handle Sound whose sending turn is to be consulted, or Handle Threaded Sound.

Return:\
`true` when at least one of the two sending buffers is free: there is room for `sound::stream`.\
`false` when both are still in service and nothing fits, or if the handle is `nullptr` or invalid.\
With the output closed it answers `true`, since no buffer is sounding, even though `sound::stream` refuses data until there is a `sound::play`.\
An output disconnection leaves no turn pledged: `sound::stream` reclaims the turns when it notices, which is where the disconnection is accounted for, so the query answers `true` again with no need of `sound::stop`.

\
Behavior:\
This function looks at how much ground the made deliveries have covered and announces whether one turn is available.\
It is the producer's traffic light: it does not wait, send, or cut anything, and answers at once.\
Consulting is also what lets a finished delivery be registered.\
A loop that only calls `sound::stream` without asking ends up stuck: once both buffers sit marked as occupied, nothing else hands them back, so skipping `sync` produces audio that cuts out even though the machine has plenty of room.\
The natural rhythm with two turns is keeping them full: send when `sync` answers `true` and wait a little when it answers `false`.\
Waiting belongs to the program, with the `sleep` function of the default functions; `sync` never blocks.\
The buffer size chosen in `sound::create` decides how often this question will come up: short turns answer often and delay little; long turns answer less often and pile more audio ahead.\
Over a Handle Threaded Sound the query reaches the internal sound, but that turn is already administered by its own thread, so asking from outside is rarely needed.
