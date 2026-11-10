# 📁 Files
```cpp
#define ST_IMPORT_FILE
#include "System_Tools.h"
```
### - ESP
Este apartado reúne las funciones del `namespace file` de System Tools, las que se ocupan del sistema de archivos y de los ficheros en disco.\
El trabajo se hace sobre una handle opaca que crean `file::open` y `file::folder` y que devuelve `file::close`; la misma clase de handle vale para un fichero y para una carpeta, y `file::is_folder` decide cuál de los dos es.\
Las rutas entran y salen como cadenas de bytes con los separadores de la plataforma en la que se ejecuta el programa: `file::path` y `file::root_this` las devuelven absolutas, `file::normalize` las deja uniformes y `file::name` y `file::extension` sacan sus partes.\
La E/S es de bytes contados, así que `file::read` y `file::write` no añaden codificación ni terminadores; para cadenas y objetos están `file::apends`, `file::apendws`, `file::apendc`, `file::apendo`, `file::gets`, `file::getws`, `file::getc` y `file::geto`.\
Las longitudes de texto de la librería no cuentan el terminador: `file::name`, `file::extension`, `file::path`, `file::root_this` y `file::normalize` devuelven y reciben cantidad de caracteres de texto, así que el búfer del programador necesita un byte más que la cifra entregada.\
Los fallos se anuncian solo con el valor devuelto, normalmente `nullptr`, `0` o `0xFF`; no hay un código de error aparte que consultar.\
Las marcas de apertura están en [[[Enum file_mode]]](Enum%20file_mode.md), los orígenes de desplazamiento en [[[Enum seek_mode]]](Enum%20seek_mode.md) y los tipos de marca de tiempo en [[[Enum time_mode]]](Enum%20time_mode.md).\
Para leer esta sección se asume que se conocen los conceptos fundamentales de los sistemas de archivos y el manejo de descriptores y rutas.

A continuación se enlistan las funciones y estructuras disponibles en este módulo:
* [`Enum file_mode`](Enum%20file_mode.md).
* [`Enum seek_mode`](Enum%20seek_mode.md).
* [`Enum time_mode`](Enum%20time_mode.md).
* [`exist`](exist.md).
* [`opened_like`](opened_like.md).
* [`open`](open.md).
* [`copy`](copy.md).
* [`move`](move.md).
* [`parent`](parent.md).
* [`close`](close.md).
* [`remove`](remove.md).
* [`timestamp`](timestamp.md).
* [`read`](read.md).
* [`write`](write.md).
* [`size`](size.md).
* [`tell`](tell.md).
* [`seek`](seek.md).
* [`clear`](clear.md).
* [`folder`](folder.md).
* [`inside`](inside.md).
* [`is_folder`](is_folder.md).
* [`name`](name.md).
* [`extension`](extension.md).
* [`path`](path.md).
* [`root_this`](root_this.md).
* [`normalize`](normalize.md).
* [`apends`](apends.md).
* [`apendws`](apendws.md).
* [`apendc`](apendc.md).
* [`apendo`](apendo.md).
* [`gets`](gets.md).
* [`getws`](getws.md).
* [`getc`](getc.md).
* [`geto`](geto.md).
* [`to_date`](to_date.md).
* [`to_time`](to_time.md).
* [`days_until_2000`](days_until_2000.md).
### - ENG
This section gathers the functions of the `namespace file` of System Tools, the ones taking care of the file system and of the files on disk.\
The work happens over an opaque handle that `file::open` and `file::folder` create and that `file::close` gives back; the very same kind of handle serves for a file and for a folder, and `file::is_folder` tells which of the two it is.\
Paths come in and go out as byte strings carrying the separators of the platform the program runs on: `file::path` and `file::root_this` return them absolute, `file::normalize` makes them uniform and `file::name` and `file::extension` pull their parts out.\
I/O is counted in bytes, so `file::read` and `file::write` add no encoding and no terminators; strings and objects are handled by `file::apends`, `file::apendws`, `file::apendc`, `file::apendo`, `file::gets`, `file::getws`, `file::getc` and `file::geto`.\
Text lengths in the library do not count the terminator: `file::name`, `file::extension`, `file::path`, `file::root_this` and `file::normalize` return and receive amounts of text characters, so the programmer's buffer needs one byte more than the figure delivered.\
Failures are announced only through the return value, usually `nullptr`, `0` or `0xFF`; there is no separate error code to look up.\
The opening flags live in [[[Enum file_mode]]](Enum%20file_mode.md), the seeking origins in [[[Enum seek_mode]]](Enum%20seek_mode.md) and the timestamp kinds in [[[Enum time_mode]]](Enum%20time_mode.md).\
Reading this section assumes familiarity with the fundamental concepts of file systems and with the handling of descriptors and paths.

Below is the list of available functions and structures in this module:
* [`Enum file_mode`](Enum%20file_mode.md).
* [`Enum seek_mode`](Enum%20seek_mode.md).
* [`Enum time_mode`](Enum%20time_mode.md).
* [`exist`](exist.md).
* [`opened_like`](opened_like.md).
* [`open`](open.md).
* [`copy`](copy.md).
* [`move`](move.md).
* [`parent`](parent.md).
* [`close`](close.md).
* [`remove`](remove.md).
* [`timestamp`](timestamp.md).
* [`read`](read.md).
* [`write`](write.md).
* [`size`](size.md).
* [`tell`](tell.md).
* [`seek`](seek.md).
* [`clear`](clear.md).
* [`folder`](folder.md).
* [`inside`](inside.md).
* [`is_folder`](is_folder.md).
* [`name`](name.md).
* [`extension`](extension.md).
* [`path`](path.md).
* [`root_this`](root_this.md).
* [`normalize`](normalize.md).
* [`apends`](apends.md).
* [`apendws`](apendws.md).
* [`apendc`](apendc.md).
* [`apendo`](apendo.md).
* [`gets`](gets.md).
* [`getws`](getws.md).
* [`getc`](getc.md).
* [`geto`](geto.md).
* [`to_date`](to_date.md).
* [`to_time`](to_time.md).
* [`days_until_2000`](days_until_2000.md).