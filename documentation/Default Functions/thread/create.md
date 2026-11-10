# create
### - ESP
Firma:
```cpp
void* create(void*(*func)(void*), void* arg = nullptr);
```
Parámetros:
* **Func**: Puntero a función que se desee ejecutar en paralelo.
* **Arg** (`nullptr` por defecto): Puntero a pasar como argumento.

Retorno:\
Puntero que representa el manejador (*handle*) del hilo creado, en caso de éxito.\
`nullptr` si ocurre una falla durante la asignación/creación por parte del sistema operativo, o si el puntero entregado a **Func** es `nullptr`.

\
Comportamiento:\
Al invocarse, inicia de inmediato la ejecución de **Func** en un hilo independiente, pasándole **Arg** como su único parámetro de entrada.\
Posee variaciones ocultas: no exige convertir el puntero de la función estrictamente al tipo `void*(*)(void*)`.\
El handle retornado gestiona después el hilo: `thread::join` lo espera y `thread::kill` lo da por terminado, liberando sus recursos.

### - ENG
Signature:
```cpp
void* create(void*(*func)(void*), void* arg = nullptr);
```
Parameters:
* **Func**: Function pointer to the target routine that will be executed in parallel.
* **Arg** (`nullptr` by default): Pointer to be passed as argument.

Return:\
Pointer representing the thread handle of the newly created thread on success.\
`nullptr` if an error occurs during system allocation/creation, or if the pointer passed to **Func** is `nullptr`.

\
Behavior:\
On invocation, it immediately starts running **Func** in an independent thread context, passing **Arg** as its sole input parameter.\
It has hidden variations: the function pointer does not have to be converted strictly to the type `void*(*)(void*)`.\
The returned handle manages the thread afterwards: `thread::join` waits for it and `thread::kill` terminates it, releasing its resources.