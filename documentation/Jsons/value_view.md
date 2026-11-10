# value_view
### - ESP
Firma:
```cpp
void* value_view(void* jsn, void* offset, BYTE* buffer, UWORD* buffer_size = nullptr);
```
Parámetros:
* **Jsn**: Handle JSON que se va a consultar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento a buscar.
  * **Para otros casos**: no se toma en cuenta.
* **Buffer**: búfer de bytes donde se escribe el Handle JSON del elemento encontrado.
* **Buffer_size** (opcional): recibe la cantidad de bytes que debería tener **Buffer**.

Retorno:\
El puntero **Buffer**, con el Handle JSON del elemento ya escrito dentro, si el tipo es contenedor (`json_type::ary` u `json_type::obj`).\
Un puntero a la cadena dentro del propio handle, si el tipo es `json_type::str`; no es una copia y no hay que liberarlo.\
El valor empaquetado en un `void*` para el resto de los tipos.\
`nullptr` si el handle es inválido, si **Buffer** llega vacío, si el elemento pedido no existe o si el JSON no guarda nada.

\
Comportamiento:\
Lee el mismo elemento que `json::value`, pero en vez de fabricar un handle lo escribe en los bytes de **Buffer**.\
Ese handle de lectura no es memoria de la librería: `json::detach` lo reconoce y no libera nada, y tampoco hay nada que liberar de él.\
Cuando **Jsn** no es un contenedor, los bytes de **Buffer** no se tocan.\
**Buffer_size** queda escrito aunque la respuesta sea `nullptr`.\
La vista vive de los datos del handle padre: liberado el padre, lo que señala deja de existir.\
Es la alternativa de `json::value` para leer una estructura a fondo.

### - ENG
Signature:
```cpp
void* value_view(void* jsn, void* offset, BYTE* buffer, UWORD* buffer_size = nullptr);
```
Parameters:
* **Jsn**: JSON Handle to be queried.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the position of the element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the element to look for.
  * **For other cases**: it is not taken into account.
* **Buffer**: byte buffer where the JSON Handle of the found element gets written.
* **Buffer_size** (optional): receives the amount of bytes **Buffer** is supposed to have.

Return:\
The **Buffer** pointer, with the element's JSON Handle already written inside it, if the type is a container (`json_type::ary` or `json_type::obj`).\
A pointer to the string inside the handle itself, if the type is `json_type::str`; it is not a copy and there is nothing to free.\
The value packed into a `void*` for the rest of the types.\
`nullptr` if the handle is invalid, if **Buffer** arrives empty, if the element asked for does not exist, or if the JSON keeps nothing.

\
Behavior:\
It reads the same element `json::value` reads, but instead of manufacturing a handle it writes one into the bytes of **Buffer**.\
That read handle is not library memory: `json::detach` recognizes it and frees nothing, and there is nothing of its own to free.\
When **Jsn** is not a container, the bytes of **Buffer** are not touched.\
**Buffer_size** ends up written even when the answer is `nullptr`.\
The view lives off the parent handle's data: once the parent is freed, what it points at stops existing.\
It is the alternative to `json::value` for reading a structure to the bottom.