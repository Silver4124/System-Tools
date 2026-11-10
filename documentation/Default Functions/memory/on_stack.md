# on_stack
### - ESP
Firma:
```cpp
bool on_stack(void* ptr);
```
Parámetros:
* **Ptr**: Puntero a la dirección de memoria del buffer a consultar.

Retorno:
* `true`: En caso de que la dirección de memoria solicitada sí pertenezca al **Stack** de tu programa.
* `false`: En caso de error, o en caso de que la dirección de memoria solicitada no pertenezca al **Stack** de tu programa.

\
Comportamiento:\
Determina si una dirección de memoria pertenece al **Stack** del programa.\
Un `false` puede deberse a un falso negativo: memoria fuera del programa, errores de paginación, o un error de resolución algorítmica de la propia función.

### - ENG
Signature:
```cpp
bool on_stack(void* ptr);
```
Parameters:
* **Ptr**: Pointer to the memory address of the buffer to be queried.

Return:
* `true`: In case the requested memory address does belong to your program's **Stack**.
* `false`: In the event of an error, or if the requested memory address does not belong to your program's **stack**.

\
Behavior:\
It determines whether a memory address belongs to your program's **Stack**.\
A `false` may be a false negative: memory outside your program, paging errors, or an algorithmic resolution error of the function itself.