# master_volume
### - ESP
Firma:
```cpp
bool master_volume(void* snd, float** mst_vol);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo del que desea conocerse su maestro.
* **Mst_vol**: destino donde se deja la dirección del `float` enlazado.

Retorno:\
`true` si **Mst_vol** no es `nullptr` y el handle es utilizable.\
`false` si **Mst_vol** es `nullptr` o si el handle es nulo o inválido.\
El destino recibe la dirección enlazada, o `nullptr` cuando el handle no tiene maestro.\
Sobre un handle inválido el destino queda en `nullptr`, así que un `nullptr` por sí solo no distingue ambos casos: hay que mirar el retorno.

\
Comportamiento:\
Esta función devuelve la dirección del maestro, no su cifra: para conocer el factor que se está multiplicando hay que leer el `float` al que apunta.\
Una dirección `nullptr` significa que la ganancia de la salida es únicamente el volumen del handle.\
Es la pieza que permite montar un bus de mezcla: se lee la dirección de un handle y se entrega a `sound::set_master_volume` en los demás, y a partir de ahí todos comparten un único deslizador.\
La dirección se devuelve tal cual, sin comprobar que el valor siga siendo vivo: si el programa liberó ese `float`, la consulta devuelve una dirección inservible y las entregas siguientes la leerán.\
Sobre un Handle Sonido Hilo se lee el enlace del sonido interno, el mismo que su hilo conserva cuando reconstruye la salida por un cambio de tono.\
Un **Handle Sonido** creado con `sound::create` nunca inventa su maestro: o se le enlaza uno, o queda en `nullptr`.\
Un handle recuperado desde el disco con `file::geto`, en cambio, recibe un `float` recién creado por la propia biblioteca cuando el registro lo traía, y ese valor queda en propiedad del handle que lo recuperó: se libera al destruirlo.\
Si esa dirección se enlaza a otros handles con `sound::set_master_volume`, la propiedad sigue siendo del primero: al destruirlo, los handles enlazados quedan apuntando un valor ya devuelto.

### - ENG
Signature:
```cpp
bool master_volume(void* snd, float** mst_vol);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose master is to be known.
* **Mst_vol**: Destination where the address of the linked `float` is left.

Return:\
`true` if **Mst_vol** is not `nullptr` and the handle is usable.\
`false` if **Mst_vol** is `nullptr` or if the handle is null or invalid.\
The destination receives the linked address, or `nullptr` when the handle has no master.\
Over an invalid handle the destination is left at `nullptr`, so a `nullptr` on its own tells the two cases apart by nothing: the return has to be checked.

\
Behavior:\
This function returns the master's address, not its figure: the factor being multiplied requires reading the `float` it points at.\
A `nullptr` address means the output's gain is only the handle's volume.\
It is the piece that lets a mixing bus be assembled: the address is read from one handle and handed to `sound::set_master_volume` on the rest, and from there all of them share a single slider.\
The address comes back as it is, with no check that the value is still alive: if the program freed that `float`, the query returns an unusable address and the following deliveries will read it.\
Over a Handle Threaded Sound the link read is the internal sound's, the same one its thread keeps when rebuilding the output after a pitch change.\
A **Handle Sound** created with `sound::create` never invents its master: either one is linked to it, or it stays `nullptr`.\
A handle recovered from disk with `file::geto`, however, receives a `float` freshly created by the library itself when the record carried one, and that value belongs to the recovering handle: it is freed when the handle is destroyed.\
If the address is linked to other handles with `sound::set_master_volume`, ownership stays with the first one: once it is destroyed, the linked handles end up pointing at an already returned value.

## Overloads
### - ESP
Firma:
```cpp
float* master_volume(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo del que desea conocerse su maestro.

Retorno:\
La dirección enlazada, o `nullptr` tanto si el handle no tiene maestro como si el handle es nulo o inválido.

\
Comportamiento:\
Forma breve para pasar el maestro de un handle a otro sin intermediarios: lo que devuelve se entrega directamente a `sound::set_master_volume`.\
Como no deja ver el retorno de la firma base, sobre un handle inválido se comporta igual que un handle sin maestro.

### - ENG
Signature:
```cpp
float* master_volume(void* snd);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose master is to be known.

Return:\
The linked address, or `nullptr` both when the handle has no master and when the handle is null or invalid.

\
Behavior:\
The brief form for passing a master from one handle to another with no intermediaries: whatever it returns goes straight into `sound::set_master_volume`.\
Since it hides the base signature's return, over an invalid handle it behaves exactly like a handle without a master.
