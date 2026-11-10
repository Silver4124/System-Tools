# mve_elem
### - ESP
Firma:
```cpp
void* mve_elem(void* jsn, void* offset, void* new_offset);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento.
  * **Para otros casos**: no se toma en cuenta.
* **New_offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición de destino.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta que se le va a poner al elemento.
  * **Para otros casos**: no se toma en cuenta.

Retorno:\
El valor entregado en **Jsn** una vez hecho el movimiento.\
Con las dos posiciones coincidiendo, el handle se devuelve tal cual y nada se mueve (en un objeto cuenta también como coincidencia escribir la misma etiqueta).\
`nullptr` si el handle es inválido, si alguna de las dos posiciones queda fuera del arreglo, si el contenedor está vacío, si alguna etiqueta llega vacía en un objeto, si la etiqueta de origen no existe o si la etiqueta de destino ya está tomada.

\
Comportamiento:\
Reubica un elemento del contenedor, y la forma depende del tipo.\
En un `json_type::ary`, el elemento que viaja es el de la posición menor, que va a parar a la posición mayor, y los del rango entre ambas corren un lugar.\
En un `json_type::obj`, el elemento conserva su lugar y solo cambia la etiqueta.\
Fuera de los contenedores no mueve nada: la llamada se da por buena y devuelve el mismo **Jsn**.

### - ENG
Signature:
```cpp
void* mve_elem(void* jsn, void* offset, void* new_offset);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the position of the element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the element.
  * **For other cases**: it is not taken into account.
* **New_offset**:
  * **For `json_type::ary`**: unsigned integer with the destination position.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label the element is to be given.
  * **For other cases**: it is not taken into account.

Return:\
The value handed in **Jsn** once the move is done.\
With the two positions matching, the handle comes back as it is and nothing gets moved (in an object, the two labels written the same counts as a match too).\
`nullptr` if the handle is invalid, if either position falls outside the array, if the container is empty, if a label arrives empty on an object, if the source label does not exist, or if the destination label is already taken.

\
Behavior:\
It relocates one element of the container, and the way it does it depends on the type.\
In a `json_type::ary`, the element that travels is the one at the lower position, which ends up at the higher position, and the ones in the range between the two shift one place.\
In a `json_type::obj`, the element keeps its place and only the label changes.\
Outside the containers nothing is moved: the call counts as good and returns the same **Jsn**.