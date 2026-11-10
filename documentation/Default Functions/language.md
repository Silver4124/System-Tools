# language
### - ESP
Firma:
```cpp
UWORD language();
```
Retorno:\
Número entero sin signo del tamaño de la palabra del procesador.\
Empaqueta un cuarteto de carácteres en formato ASCII, sin el carácter de terminación nula (`'\0'`).\
El cuarteto es el ID unico de idioma que está usando el usuario.

\
Comportamiento:\
Idioma y región llegan dentro de un entero, así que se comparan con una `&` y una `==`, sin buffers que mantener ni liberar.\
El empaquetado, visto como un número de 64 bits, es el siguiente:
```
0x00000000RRRRLLLL
```
Donde:
* **Cada `L`**: el espacio hexadecimal de los carácteres del idioma.
* **Cada `R`**: el espacio hexadecimal de los carácteres de la región.
* **Cada `0`**: el espacio hexadecimal libre y sin usar; podría no estar presente si el tamaño de la palabra del procesador es de 32 bits.

Cada letra corresponde a un byte completo (dos dígitos hexadecimales), así que a nivel de bits:
```
bits  31.....24 23.....16 15......8 7.......0
      region[1] region[0] lang[1]   lang[0]
```
Para obtener unicamente el idioma:
```cpp
UWORD LangID = language();
UWORD lang = LangID & 0xFFFF;            // Los dos bytes bajos: 'l0' | ('l1' << 8)
const BYTE lang_first = (BYTE)(lang);    // Primer caracter del idioma
const BYTE lang_second = (BYTE)(lang >> 8); // Segundo caracter del idioma
```
Para obtener unicamente la región:
```cpp
UWORD LangID = language();
UWORD region = (LangID >> 16) & 0xFFFF;  // Los dos bytes altos: 'r0' | ('r1' << 8)
const BYTE region_first = (BYTE)(region);
const BYTE region_second = (BYTE)(region >> 8);
// Equivalentemente, extrayendo cada byte por separado:
UWORD region_alt_calc = ((LangID >> 16) & 0xFF) | ((LangID >> 24) << 8);
// Ambos calculos llevan al mismo resultado, independientemente de como lo obtuviste:
// region == region_alt_calc
```
Si ninguna función o variable de sistema pudo proveer el idioma, la función devuelve el cuarteto de error `"ERR!"`, empaquetado como `0x21525245`.\
Compáralo explícitamente si necesitas distinguir "sin datos" de un idioma real.\
Un caracter ausente dentro de un cuarteto parcial se rellena con `'?'`.

Si deseas operar con el valor retornado como una cadena o buffer de texto ASCII:
```cpp
UWORD LangID = language();
printf("lang = %.4s\n", &LangID);
```
Este atajo depende de que la máquina sea little-endian, como ocurre con x86 y ARM.\
En una big-endian los cuatro bytes salen invertidos y hay que extraerlos con las operaciones de arriba.\
`IS_LITTLE_ENDIAN` permite comprobar el orden de los bytes en tiempo de compilación.\
Además, recuerda que el cuarteto no está terminado en `'\0'`: usa siempre una precisión explícita (`.4`) o un buffer propio con su terminador.

### - ENG
Signature:
```cpp
UWORD language();
```
Return:\
Unsigned integer of the processor word size.\
It packages a quartet of characters in ASCII format, without the null termination character (`'\0'`).\
The quartet is the unique language ID that the user is using.

\
Behavior:\
Language and region arrive inside an integer, so they are compared with an `&` and a `==`, with no buffers to keep or to free.\
The packing, seen as a 64-bit number, is the following:
```
0x00000000RRRRLLLL
```
Wheres:
* **Each `L`**: the hexadecimal space of the language characters.
* **Each `R`**: the hexadecimal space of the region characters.
* **Each `0`**: the free, unused hexadecimal space; it may be absent if the processor word size is 32 bits.

Each letter is a whole byte (two hexadecimal digits), so at bit level:
```
bits  31.....24 23.....16 15......8 7.......0
      region[1] region[0] lang[1]   lang[0]
```
If you only want the language:
```cpp
UWORD LangID = language();
UWORD lang = LangID & 0xFFFF;               // The two low bytes: 'l0' | ('l1' << 8)
const BYTE lang_first = (BYTE)(lang);       // First character of the language
const BYTE lang_second = (BYTE)(lang >> 8); // Second character of the language
```
If you only want the region:
```cpp
UWORD LangID = language();
UWORD region = (LangID >> 16) & 0xFFFF;     // The two high bytes: 'r0' | ('r1' << 8)
const BYTE region_first = (BYTE)(region);
const BYTE region_second = (BYTE)(region >> 8);
// Equivalently, extracting each byte separately:
UWORD region_alt_calc = ((LangID >> 16) & 0xFF) | ((LangID >> 24) << 8);
// Both calculations lead to the same result, regardless of how you obtained it:
// region == region_alt_calc
```
If no system function or variable could provide the language, the function returns the error quartet `"ERR!"`, packed as `0x21525245`.\
Compare against it explicitly if you need to tell "no data" from a real language.\
A missing character inside a partial quartet is filled with `'?'`.

To treat the returned value as an ASCII text string or buffer:
```cpp
UWORD LangID = language();
printf("lang = %.4s\n", &LangID);
```
This shortcut depends on the machine being little-endian, as x86 and ARM are.\
On a big-endian the four bytes come out inverted and have to be extracted with the operations above.\
`IS_LITTLE_ENDIAN` lets you check the byte order at compile time.\
Also remember the quartet is not `'\0'`-terminated: always use an explicit precision (`.4`) or your own terminated buffer.