# add_elem
### - ESP
Firma:
```cpp
void* add_elem(void* jsn, void* offset, BYTE type);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición de destino, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento nuevo.
  * **Para otros casos**: no se toma en cuenta.
* **Type**: tipo de dato del elemento que se va a añadir.

Retorno:\
El valor entregado en **Jsn** cuando el elemento quedó añadido.\
`nullptr` si el handle es inválido, si **Offset** llega vacío en un objeto o si esa etiqueta ya existe.

\
Comportamiento:\
Añade un elemento del tipo **Type**, con su valor en blanco.\
En un `json_type::ary`, el elemento se coloca en la posición **Offset** y los que había después corren un lugar; una posición más allá del final lo deja al final.\
En un `json_type::obj`, la etiqueta tiene que ser nueva y el elemento se agrega al final de la lista.\
Un `json_type::nll` ocupa su lugar sin guardar contenido.\
Fuera de los contenedores no añade nada: la llamada se da por buena y devuelve el mismo **Jsn**.\
Los tipos que se pueden pedir están en [[[Enum json_type]]](Enum%20json_type.md).

### - ENG
Signature:
```cpp
void* add_elem(void* jsn, void* offset, BYTE type);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the destination position, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the new element.
  * **For other cases**: it is not taken into account.
* **Type**: data type of the element to be added.

Return:\
The value handed in **Jsn** once the element has been added.\
`nullptr` if the handle is invalid, if **Offset** arrives empty on an object, or if that label already exists.

\
Behavior:\
It adds an element of type **Type**, with its value left blank.\
In a `json_type::ary`, the element takes position **Offset** and the ones behind it shift one place; a position past the end leaves it at the end.\
In a `json_type::obj`, the label has to be new and the element goes at the end of the list.\
A `json_type::nll` takes its place without holding any content.\
Outside the containers nothing is added: the call counts as good and returns the same **Jsn**.\
The types that can be asked for are in [[[Enum json_type]]](Enum%20json_type.md).