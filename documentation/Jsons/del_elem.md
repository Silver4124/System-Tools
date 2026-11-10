# del_elem
### - ESP
Firma:
```cpp
void* del_elem(void* jsn, void* offset);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento que se va a eliminar.
  * **Para otros casos**: no se toma en cuenta.

Retorno:\
El valor entregado en **Jsn** cuando el elemento quedó eliminado.\
`nullptr` si el handle es inválido, si la posición queda fuera del arreglo, si el contenedor está vacío, si **Offset** llega vacío en un objeto o si esa etiqueta no existe.

\
Comportamiento:\
Quita un elemento del contenedor.\
En un `json_type::ary`, las posiciones siguientes quedan renumeradas una plaza más abajo.\
En un `json_type::obj`, se elimina la etiqueta junto con su elemento.\
Fuera de los contenedores no quita nada: la llamada se da por buena y devuelve el mismo **Jsn**.

### - ENG
Signature:
```cpp
void* del_elem(void* jsn, void* offset);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the position of the element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the element to be deleted.
  * **For other cases**: it is not taken into account.

Return:\
The value handed in **Jsn** once the element has been removed.\
`nullptr` if the handle is invalid, if the position falls outside the array, if the container is empty, if **Offset** arrives empty on an object, or if that label does not exist.

\
Behavior:\
It takes one element out of the container.\
In a `json_type::ary`, the following positions end up renumbered one slot lower.\
In a `json_type::obj`, the label goes away together with its element.\
Outside the containers nothing is removed: the call counts as good and returns the same **Jsn**.