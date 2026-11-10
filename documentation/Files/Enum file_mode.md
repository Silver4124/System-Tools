# Enum file_mode
### - ESP
Este enumerador indica los permisos de acceso y creación utilizados al momento de abrir un fichero mediante las funciones del módulo `ST_IMPORT_FILE`.\
Las etiquetas se combinan con el operador binario OR (`|`) para otorgar varios permisos a la vez en la misma apertura.
| Etiqueta del Enumerador | Valor | Permiso Asignado |
| :---- | ---- | :---- |
| `file_mode::none` | `0x00` | Sin permisos |
| `file_mode::read` | `0x01` | Lectura |
| `file_mode::write` | `0x02` | Escritura |
| `file_mode::create` | `0x08` | Creación; si el fichero ya existe, entonces borrará todo su contenido. |

### - ENG
This enumerator specifies the access and creation permissions used when opening a file via the functions of the `ST_IMPORT_FILE` module.\
The tags combine with the binary OR operator (`|`) to grant several permissions at once in the same opening.
| Enumerator Tag | Value | Assigned Permission |
| :---- | ---- | :---- |
| `file_mode::none` | `0x00` | Without permits |
| `file_mode::read` | `0x01` | Read permission |
| `file_mode::write` | `0x02` | Write permission |
| `file_mode::create` | `0x08` | Creation permission; if the file already exists, then it will delete all its contents. |