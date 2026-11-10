# Enum time_mode
### - ESP
Este enumerador indica qué marca de tiempo específica del sistema de archivos se desea consultar sobre un fichero determinado.
| Etiqueta del Enumerador | Valor | Marca de tiempo |
| :---- | ---- | :---- |
| `time_mode::creation` | `0` | Desde su creación |
| `time_mode::access` | `1` | Desde su última apertura |
| `time_mode::writed` | `2` | Desde su última escritura |

### - ENG
This enumerator specifies which specific file system timestamp you want to query for a given file.
| Enumerator Tag | Value | Timestamp |
| :---- | ---- | :---- |
| `time_mode::creation` | `0` | Since its creation |
| `time_mode::access` | `1` | Since its last opening |
| `time_mode::writed` | `2` | Since its last writing |