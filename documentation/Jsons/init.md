# init
### - ESP
Firma:
```cpp
void* init(BYTE type);
```
Parámetros:
* **Type**: tipo de dato que va a tener el JSON.

Retorno:\
Handle del JSON creado.\
`nullptr` si la reserva falla o si **Type** es `json_type::unk`.

\
Comportamiento:\
Arma un dato JSON nuevo con el tipo pedido y su valor en blanco.\
Un contenedor, `json_type::ary` o `json_type::obj`, nace sin elementos; un `json_type::nll` nace sin nada de almacenamiento.\
El handle vive en el **Heap**, así que hay que pasarle `json::detach` cuando deje de hacer falta.\
Los tipos que se pueden pedir están en [[[Enum json_type]]](Enum%20json_type.md).

### - ENG
Signature:
```cpp
void* init(BYTE type);
```
Parameters:
* **Type**: data type the JSON is to have.

Return:\
Handle of the JSON created.\
`nullptr` if the allocation fails or if **Type** is `json_type::unk`.

\
Behavior:\
It builds a new JSON datum of the type asked for, with its value left blank.\
A container, `json_type::ary` or `json_type::obj`, is born with no elements; a `json_type::nll` is born with no storage at all.\
The handle lives in the **Heap**, so it has to be handed to `json::detach` once it is no longer needed.\
The types that can be asked for are in [[[Enum json_type]]](Enum%20json_type.md).