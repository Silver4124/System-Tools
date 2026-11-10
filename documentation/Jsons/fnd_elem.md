# fnd_elem
### - ESP
Firma:
```cpp
UWORD fnd_elem(void* jsn, void* offset);
```
Parámetros:
* **Jsn**: Handle JSON que se va a consultar.
* **Offset**:
  * **Para `json_type::ary`**: número entero sin signo con la posición a comprobar, contada desde `0`.
  * **Para `json_type::obj`**: cadena de texto o búfer de texto ASCII con la etiqueta del elemento.
  * **Para otros casos**: no se toma en cuenta.

Retorno:\
La posición del elemento encontrado, contada desde `1`.\
`0` si el handle es inválido, si la posición pedida cae fuera del arreglo, si el contenedor está vacío, si **Offset** llega vacío en un objeto o si esa etiqueta no existe.

\
Comportamiento:\
En un `json_type::obj` recorre las etiquetas y dice en cuál lugar está la buscada.\
En un `json_type::ary` no busca: comprueba la posición entregada y la devuelve aumentada en uno.\
La respuesta va en base `1`, así que para usarla como **Offset** en las demás funciones hay que restarle uno.\
Un handle que no sea contenedor responde `0`, igual que un elemento que no está.

### - ENG
Signature:
```cpp
UWORD fnd_elem(void* jsn, void* offset);
```
Parameters:
* **Jsn**: JSON Handle to be queried.
* **Offset**:
  * **For `json_type::ary`**: unsigned integer with the position to be checked, counted from `0`.
  * **For `json_type::obj`**: text string or ASCII text buffer with the label of the element.
  * **For other cases**: it is not taken into account.

Return:\
The position of the element found, counted from `1`.\
`0` if the handle is invalid, if the asked position falls outside the array, if the container is empty, if **Offset** arrives empty on an object, or if that label does not exist.

\
Behavior:\
In a `json_type::obj` it walks the labels and says which place holds the one being looked for.\
In a `json_type::ary` it searches nothing: it checks the position handed over and returns it plus one.\
The answer comes in base `1`, so using it as **Offset** in the rest of the functions means subtracting one.\
A handle that is not a container answers `0`, the same as an element that is not there.