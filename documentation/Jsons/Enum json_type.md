# Enum json_type
### - ESP
Este enumerador da los códigos de tipo con los que el módulo clasifica los elementos de un JSON.\
Junto con los tipos está la bandera `json_type::uns`, que marca el signo, y aparte queda `json_type::unk`, el código del tipo desconocido o del error.

| Etiqueta/s del enumerador | Valor | Tipo de dato | Elemento/s semánticos asignados |
| :---- | :---- | :---- | :---- |
| `json_type::nll` | `0x00` | Nulo | `null` |
| `json_type::bll` | `0x01` | `false` | `false` |
| `json_type::bll \| json_type::uns` | `0x11` | `true` | `true` |
| `json_type::num` | `0x02` | `WORD` | Cualquier número entero con signo negativo |
| `json_type::num \| json_type::uns` | `0x12` | `UWORD` | Cualquier número entero sin signo negativo |
| `json_type::flt` | `0x03` | `float` | Cualquier número en coma flotante o decimal |
| `json_type::str` | `0x04` | Cadena de texto ASCII | Cualquier texto encerrado entre `"` |
| `json_type::ary` | `0x05` | Arreglo, o Lista de elementos | `[]`, `[elem1]`, `[elem1, elem2]`, `[elem1, elem2, elem3]`, ... |
| `json_type::obj` | `0x06` | Objeto, o lista etiquetada de elementos | `{}`, `{"name1": elem1}`, `{"name1": elem1, "name2": elem2}`, `{"name1": elem1, "name2": elem2, "name3": elem3}`, ... |
| `json_type::uns` | `0x10` | Bandera de especificación de signo | |
| `json_type::unk` | `0xFF` | Especificador de error de tipo | |

### - ENG
This enum gives the type codes the module uses to classify the elements of a JSON.\
Along with the types sits the `json_type::uns` flag, which marks the sign, and `json_type::unk` stands apart, the code for the unknown type or the error.

| Enumerator Tag/s | Value | Type | Assigned Semantic Element/s |
| :---- | :---- | :---- | :---- |
| `json_type::nll` | `0x00` | Null | `null` |
| `json_type::bll` | `0x01` | `false` | `false` |
| `json_type::bll \| json_type::uns` | `0x11` | `true` | `true` |
| `json_type::num` | `0x02` | `WORD` | Any integer with negative sign |
| `json_type::num \| json_type::uns` | `0x12` | `UWORD` | Any integer without negative sign |
| `json_type::flt` | `0x03` | `float` | Any decimal or float number |
| `json_type::str` | `0x04` | ASCII text string | Any text between `"` |
| `json_type::ary` | `0x05` | Array, or Elements list | `[]`, `[elem1]`, `[elem1, elem2]`, `[elem1, elem2, elem3]`, ... |
| `json_type::obj` | `0x06` | Object, or tagged elements list | `{}`, `{"name1": elem1}`, `{"name1": elem1, "name2": elem2}`, `{"name1": elem1, "name2": elem2, "name3": elem3}`, ... |
| `json_type::uns` | `0x10` | Sign specifier flag | |
| `json_type::unk` | `0xFF` | Type error specifier | |