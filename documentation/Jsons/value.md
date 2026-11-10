# value
### - ESP
Firma:
```cpp
void* value(void* jsn, void* offset);
```
Parámetros:
* **Jsn**: Handle JSON que se va a consultar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento a buscar.
  * **Para otros casos**: no se toma en cuenta.

Retorno:\
Un Handle JSON nuevo para manipular el elemento, si el tipo es contenedor (`json_type::ary` u `json_type::obj`).\
Un puntero a la cadena dentro del propio handle, si el tipo es `json_type::str`; no es una copia y no hay que liberarlo.\
El valor empaquetado en un `void*` para el resto de los tipos.\
`nullptr` si el handle es inválido, si el elemento pedido no existe o si el JSON no guarda nada.

\
Comportamiento:\
Cada llamada fabrica un Handle JSON nuevo, que hay que regular con `json::detach`, así que no conviene usarla de forma reiterada.\
Para explorar la estructura a fondo está `json::value_view`, que no fabrica handles.

### - ENG
Signature:
```cpp
void* value(void* jsn, void* offset);
```
Parameters:
* **Jsn**: JSON Handle to be queried.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the position of the element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the element to look for.
  * **For other cases**: it is not taken into account.

Return:\
A new JSON Handle to work the element with, if the type is a container (`json_type::ary` or `json_type::obj`).\
A pointer to the string inside the handle itself, if the type is `json_type::str`; it is not a copy and there is nothing to free.\
The value packed into a `void*` for the rest of the types.\
`nullptr` if the handle is invalid, if the element asked for does not exist, or if the JSON keeps nothing.

\
Behavior:\
Every call manufactures a new JSON Handle, which has to be settled with `json::detach`, so repeated use is not advisable.\
For exploring the structure to the bottom there is `json::value_view`, which manufactures no handles.