# detach
### - ESP
Firma:
```cpp
void* detach(void* jsn);
```
Parámetros:
* **Jsn**: Handle JSON que se va a liberar.

Retorno:\
El valor entregado en **Jsn** una vez liberado.\
`nullptr` si el handle es inválido.

\
Comportamiento:\
Destruye el handle y lo que el JSON guarda con él, sacándolo del **Heap** del programa.\
Hecho esto, cualquier otro puntero que apuntara a ese handle pasa a ser una dirección inválida.\
El handle que se obtuvo de `json::value_view` se reconoce como handle de lectura y se devuelve tal cual, sin liberar nada.

### - ENG
Signature:
```cpp
void* detach(void* jsn);
```
Parameters:
* **Jsn**: JSON Handle to be freed.

Return:\
The value handed in **Jsn** once freed.\
`nullptr` if the handle is invalid.

\
Behavior:\
It destroys the handle and what the JSON keeps with it, taking it out of the program's **Heap**.\
After that, any other pointer aimed at that handle is an invalid address.\
A handle gotten from `json::value_view` is recognized as a read handle and handed back as it is, freeing nothing.