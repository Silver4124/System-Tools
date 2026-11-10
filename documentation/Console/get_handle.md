# get_handle
### - ESP
Firma:
```cpp
void* get_handle();
```
Retorno:\
**Handle Consola** del programa, la misma dirección en cada llamada.\
`nullptr` si la consola resulta inalcanzable desde el proceso, cuando alguna de las funciones de consola necesarias del entorno no están disponibles.\
Que la respuesta no sea `nullptr` no garantiza que haya una consola delante, ya que un proceso lanzado sin consola obtiene un handle válido cuyas salidas están vacías, y serán las demás funciones del módulo las que reporten el fallo una a una.

\
Comportamiento:\
Esta función es la puerta de entrada única del módulo. La primera vez que se la invoca construye el handle y lo deja instalado; de ahí en adelante se limita a devolver su dirección, así que pedirla en cada llamada cuesta lo mismo que comparar un entero y no reserva memoria que haya que liberar.\
El handle no se crea por ventana: al ser uno solo para todo el proceso, describe la consola con la que el proceso nació y no apunta a una terminal ajena.\
Al construirse, la función toma nota del estado en que la consola se encuentra, y ese retrato es lo que `console::restorecolor` y `console::restoreunicode` devolverán más tarde.\
Ninguna función del módulo libera el handle, y entregarlo a `memory::free` lo estropearía al ser memoria de la propia librería, no una reserva devuelta por un `memory::alloc` o `new`.\
Al ser un handle único por programa, las funciones e hilos del proceso operan todos con la misma dirección, y `namespace window` la acepta como padre, así que conviene guardar la copia del puntero y repartirla por el programa.\
El handle no se puede pasar a otro proceso: la dirección solo describe la consola del proceso que la obtuvo.

### - ENG
Signature:
```cpp
void* get_handle();
```
Return:\
The program's **Console Handle**, the same address on every call.\
`nullptr` if the console turns out unreachable from the process, because some of the console functions the environment needs are unavailable.\
A non-`nullptr` answer does not guarantee there is a console in front: a process launched without one gets a valid handle whose outputs are empty, and it is the rest of the module's functions that report the failure one by one.

\
Behavior:\
This function serves as the sole entry point for the module. When invoked for the first time, it constructs the handle and installs it; From then on, it simply returns its address, so requesting it on each call costs the same as comparing an integer and doesn't allocate memory that needs to be freed.\
The handle isn't created per window; since there's only one for the entire process, it describes the console with which the process started and doesn't point to an external terminal.\
When constructed, the function takes note of the console's state, and this description is what `console::restorecolor` and `console::restoreunicode` will later return.\
No function in the module frees the handle, and assigning it to `memory::free` would corrupt it, as it's memory belonging to the library itself, not an allocation returned by `memory::alloc` or `new`.\
Since there's only one handle per program, the process's functions and threads all work with the same address, and `namespace window` accepts it as a father, so it's worth keeping a copy of the pointer and passing it around the program.\
The handle can't be handed to another process: the address only describes the console of the process that obtained it.