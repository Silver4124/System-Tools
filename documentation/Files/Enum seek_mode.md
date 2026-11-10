# Enum seek_mode
### - ESP
Este enumerador indica el punto de referencia relativo desde el cual se calculará el desplazamiento del puntero de lectura y escritura dentro de un fichero al usar la función `file::seek`.
| Etiqueta del Enumerador | Valor | Orientación |
| :---- | ---- | :---- |
| `seek_mode::begin` | `0` | Desde el inicio |
| `seek_mode::current` | `1` | Desde la posición actual del puntero de E/S del fichero |
| `seek_mode::end` | `2` | Desde el final |

### - ENG
This enumerator specifies the relative origin from which the reading and writing pointer offset will be calculated inside a file when using `file::seek`.
| Enumerator Tag | Value | Orientation |
| :---- | ---- | :---- |
| `seek_mode::begin` | `0` | From the beginning |
| `seek_mode::current` | `1` | From the current file I/O pointer position |
| `seek_mode::end` | `2` | From the end |