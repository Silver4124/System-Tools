# swp_elem
### - ESP
Firma:
```cpp
void* swp_elem(void* jsn, void* offset_a, void* offset_b);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Offset_a**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del primer elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del primer elemento.
  * **Para otros casos**: no se toma en cuenta.
* **Offset_b**:
  * **Para `json_type::ary`**: número entero sin signo con la posición del segundo elemento, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del segundo elemento.
  * **Para otros casos**: no se toma en cuenta.

Retorno:\
El valor entregado en **Jsn** una vez hecho el intercambio.\
Con las dos posiciones coincidiendo, el handle se devuelve tal cual y nada se intercambia (en un objeto cuenta también como coincidencia escribir la misma etiqueta).\
`nullptr` si el handle es inválido, si alguna de las dos posiciones queda fuera del arreglo, si el contenedor está vacío, si alguna etiqueta llega vacía en un objeto o si alguna de las dos etiquetas no existe.

\
Comportamiento:\
Cambia de lugar dos elementos del contenedor, y solo esos dos: las demás posiciones no se mueven.\
En un `json_type::obj`, las etiquetas se quedan donde están y son los valores los que pasan de una a otra.\
Fuera de los contenedores no intercambia nada: la llamada se da por buena y devuelve el mismo **Jsn**.

### - ENG
Signature:
```cpp
void* swp_elem(void* jsn, void* offset_a, void* offset_b);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Offset_a**:
  * **For `json_type::ary`**: unsigned integer with the position of the first element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the first element.
  * **For other cases**: it is not taken into account.
* **Offset_b**:
  * **For `json_type::ary`**: unsigned integer with the position of the second element, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the second element.
  * **For other cases**: it is not taken into account.

Return:\
The value handed in **Jsn** once the swap is done.\
With the two positions matching, the handle comes back as it is and nothing gets swapped (in an object, the two labels written the same counts as a match too).\
`nullptr` if the handle is invalid, if either position falls outside the array, if the container is empty, if a label arrives empty on an object, or if either of the two labels does not exist.

\
Behavior:\
It swaps two elements of the container, and only those two: the rest of the positions do not move.\
In a `json_type::obj`, the labels stay where they are and it is the values that pass from one to the other.\
Outside the containers nothing is swapped: the call counts as good and returns the same **Jsn**.