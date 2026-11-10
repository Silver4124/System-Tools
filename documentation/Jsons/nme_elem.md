# nme_elem
### - ESP
Firma:
```cpp
const char* nme_elem(void* jsn, UWORD offset);
```
Parámetros:
* **Jsn**: Handle JSON que se va a consultar.
* **Offset**: número entero sin signo con la posición del elemento, contada desde `0`.

Retorno:\
Cadena de texto o búfer de texto ASCII lista para operaciones de solo lectura.\
`nullptr` cuando el handle no es un `json_type::obj`, cuando el objeto no tiene elementos o cuando **Offset** queda fuera de sus posiciones.

\
Comportamiento:\
Entrega el nombre con el que está guardado el elemento que ocupa la posición **Offset** de un objeto.\
Solo responde sobre `json_type::obj`, así que cualquier otro tipo, incluido `json_type::ary`, es rechazado sin mirar el índice.\
La cadena no es una copia: apunta a la memoria del propio handle, por lo que no debe liberarse, y una modificación del objeto puede desplazarla.

### - ENG
Signature:
```cpp
const char* nme_elem(void* jsn, UWORD offset);
```
Parameters:
* **Jsn**: JSON Handle to be queried.
* **Offset**: unsigned integer with the position of the element, counted from `0`.

Return:\
Text string or ASCII text buffer ready for read-only operations.\
`nullptr` when the handle is not a `json_type::obj`, when the object holds no elements, or when **Offset** falls outside its positions.

\
Behavior:\
It delivers the name under which the element sitting at position **Offset** of an object was saved.\
It only answers over `json_type::obj`, so any other type, `json_type::ary` included, is rejected without looking at the index.\
The string is not a copy: it points at the handle's own memory, so it must not be freed, and a change to the object may shift it.