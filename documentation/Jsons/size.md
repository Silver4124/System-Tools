# size
### - ESP
Firma:
```cpp
UWORD size(void* jsn);
```
Parámetros:
* **Jsn**: Handle JSON que se va a medir.

Retorno:\
La cantidad de elementos que tiene adentro, si el tipo es contenedor (`json_type::ary` u `json_type::obj`).\
La cantidad de caracteres, sin contar el terminador nulo (`'\0'`), si el tipo es `json_type::str`.\
El tamaño en bytes del valor guardado en el resto de los tipos: un `WORD` en los enteros, `4` en `json_type::flt`, `0` en `json_type::bll`.\
`0` si el handle es inválido o si el JSON no guarda nada.

\
Comportamiento:\
El `0` no aclara el motivo: un arreglo vacío, un booleano y un handle inválido responden igual.

### - ENG
Signature:
```cpp
UWORD size(void* jsn);
```
Parameters:
* **Jsn**: JSON Handle to be measured.

Return:\
The amount of elements it holds inside, if the type is a container (`json_type::ary` or `json_type::obj`).\
The amount of characters, not counting the null terminator (`'\0'`), if the type is `json_type::str`.\
The size in bytes of the stored value for the rest of the types: one `WORD` for the integers, `4` for `json_type::flt`, `0` for `json_type::bll`.\
`0` if the handle is invalid or if the JSON keeps nothing.

\
Behavior:\
The `0` does not explain itself: an empty array, a boolean, and an invalid handle all answer the same.