# type
### - ESP
Firma:
```cpp
BYTE type(void* jsn);
```
Parámetros:
* **Jsn**: Handle JSON a evaluar.

Retorno:\
La etiqueta del tipo de dato del elemento, tomada de [[[Enum json_type]]](Enum%20json_type.md).\
`json_type::unk` si el handle es inválido, si no guarda JSON, si el tipo no está entre los soportados, o si hubo un error de análisis.

\
Comportamiento:\
El `unk` no aclara el motivo: un handle inválido y un JSON mal formado responden igual.

### - ENG
Signature:
```cpp
BYTE type(void* jsn);
```
Parameters:
* **Jsn**: JSON Handle to evaluate.

Return:\
The data type tag of the element, taken from [[[Enum json_type]]](Enum%20json_type.md).\
`json_type::unk` if the handle is invalid, if it holds no JSON, if the type is not among the supported ones, or if there was a parsing error.

\
Behavior:\
The `unk` does not explain itself: an invalid handle and a badly formed JSON answer the same.
