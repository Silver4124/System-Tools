# 🔠 Macros
### - ESP
System Tools proporciona macros de todo tipo, algunas para detectar caracteristicas del entorno, otras como herramientas semanticas, pero todas al final del dia poseen algun nivel de funcionalidad y papel dentro y fuera de la librería.\
A continuación, se va a proporcionar información, ejemplos y casos de uso sobre cada una de estas macros.\
En todos los ejemplos de este documento se asume la directiva `using namespace System_Tools;` al inicio de la unidad de traducción; sin ella, las funciones se cualificaran (`System_Tools::printf`, `System_Tools::memory::alloc`, `System_Tools::file::write`) automaticamente. Si no se incluyen cabeceras de C, la librería publica además las cualificaciones equivalentes `using System_Tools::printf;`, `using System_Tools::memory::alloc;`, etc. en el espacio de nombres global; esa publicación se decide leyendo los guardias `_INC_STDIO`/`_STDIO_H` y `_INC_STDLIB`/`_STDLIB_H` de las cabeceras ya incluidas, que la librería nunca escribe, y se apaga definiendo `ST_NO_GLOBAL_FUNCTIONS` antes de incluir `System_Tools.h`.
### - ENG
System Tools provides macros of all kinds: some to detect environment characteristics and others as semantic tools, but all ultimately play a functional role both inside and outside the library.\
Below you will find information, code examples, and use cases for each of these macros.\
Every example in this document assumes `using namespace System_Tools;` at the top of the translation unit; without it, the functions must be qualified (`System_Tools::printf`, `System_Tools::memory::alloc`, `System_Tools::file::write`) automatically. When no C headers are included, the library additionally publishes the equivalent `using System_Tools::printf;`, `using System_Tools::memory::alloc;`, etc. qualifications into the global namespace; that publication is decided by reading the `_INC_STDIO`/`_STDIO_H` and `_INC_STDLIB`/`_STDLIB_H` guards of the headers already included, which the library never writes, and it switches off by defining `ST_NO_GLOBAL_FUNCTIONS` before `System_Tools.h`.

## WORD_SIZE
### - ESP
Proporciona un entero sin signo que indica el tamaño de la palabra del procesador, utilizado comunmente para saber los bits de la plataforma a la que se desea compilar y el tamaño en bytes del tipo de dato `WORD` y `UWORD`.
* **En plataformas de 32 bits**: Esta macro se definirá como la constante `4`.
* **En plataformas de 64 bits**: Esta macro se definirá como la constante `8`.

Al definirse como una constante númerica y al estar relacionada con `WORD` y `UWORD`, su uso más comun es para la difurcación de compilación:
```cpp
#if WORD_SIZE == 8
puts("Hola desde 64 bits!");
#else
puts("Hola desde 32 bits!");
#endif
```
### - ENG
Provides an unsigned integer representing the processor word size, commonly used to determine the bitness of the target compilation platform and the byte size of the `WORD` and `UWORD` data types.
* **On 32-bit platforms**: Defined as the constant `4`.
* **On 64-bit platforms**: Defined as the constant `8`.

Since it evaluates to a numeric constant and is directly tied to WORD and UWORD, its most common use is for conditional compilation branching:
```cpp
#if WORD_SIZE == 8
puts("Hello from 64-bit!");
#else
puts("Hello from 32-bit!");
#endif
```

## WORD_PRINT
### - ESP
Proporciona una cadena de texto que indica como se debe formatear un `WORD` dentro de una función de C, con una definición dependiente del tamaño de la palabra del procesador.
* **En plataformas de 32 bits**: Esta macro se definirá como la cadena constante `"%d"`, ya que `WORD` equivale a `int`.
* **En plataformas de 64 bits**: Esta macro se definirá como la cadena constante `"%lld"`, ya que `WORD` equivale a `long long`.
```cpp
WORD num = 42;
printf("num = " WORD_PRINT "\n", num);
```
### - ENG
Provides a string literal specifying how a `WORD` should be formatted inside a C function, with a definition that depends on the processor word size.
* **On 32-bit platforms**: Defined as the constant string `"%d"`, since `WORD` is an alias of `int`.
* **On 64-bit platforms**: Defined as the constant string `"%lld"`, since `WORD` is an alias of `long long`.
```cpp
WORD num = 42;
printf("num = " WORD_PRINT "\n", num);
```

## UWORD_PRINT
### - ESP
Proporciona una cadena de texto que indica como se debe formatear un `UWORD` dentro de una función de C, con una definición dependiente del tamaño de la palabra del procesador.
* **En plataformas de 32 bits**: Esta macro se definirá como la cadena constante `"%u"`, ya que `UWORD` equivale a `unsigned int`.
* **En plataformas de 64 bits**: Esta macro se definirá como la cadena constante `"%llu"`, ya que `UWORD` equivale a `unsigned long long`.
```cpp
UWORD num = 42;
printf("num = " UWORD_PRINT "\n", num);
```
### - ENG
Provides a string literal specifying how a `UWORD` should be formatted inside a C function, with a definition that depends on the processor word size.
* **On 32-bit platforms**: Defined as the constant string `"%u"`, since `UWORD` is an alias of `unsigned int`.
* **On 64-bit platforms**: Defined as the constant string `"%llu"`, since `UWORD` is an alias of `unsigned long long`.
```cpp
UWORD num = 42;
printf("num = " UWORD_PRINT "\n", num);
```

## OWORD_PRINT
### - ESP
Proporciona una cadena de texto que indica como se debe formatear un `WORD` o `UWORD` dentro de una función de C en forma de un número en base octal, con una definición dependiente del tamaño de la palabra del procesador.
* **En plataformas de 32 bits**: Esta macro se definirá como la cadena constante `"%o"`.
* **En plataformas de 64 bits**: Esta macro se definirá como la cadena constante `"%llo"`.
```cpp
UWORD onum = 42;
printf("onum = " OWORD_PRINT "\n", onum);
```
### - ENG
Provides a string literal specifying how a `WORD` or `UWORD` should be formatted as an octal number inside a C function, with a definition that depends on the processor word size.
* **On 32-bit platforms**: Defined as the constant string `"%o"`.
* **On 64-bit platforms**: Defined as the constant string `"%llo"`.
```cpp
UWORD onum = 42;
printf("onum = " OWORD_PRINT "\n", onum);
```

## XWORD_PRINT
### - ESP
Proporciona una cadena de texto que indica como se debe formatear un `WORD` o `UWORD` dentro de una función de C en forma de un número en base hexadecimal en mayuscula, con una definición dependiente del tamaño de la palabra del procesador.
* **En plataformas de 32 bits**: Esta macro se definirá como la cadena constante `"%X"`.
* **En plataformas de 64 bits**: Esta macro se definirá como la cadena constante `"%llX"`.
```cpp
UWORD xnum = 42;
printf("xnum = " XWORD_PRINT "\n", xnum);
```
### - ENG
Provides a string literal specifying how a `WORD` or `UWORD` should be formatted as an uppercase hexadecimal number inside a C function, with a definition that depends on the processor word size.
* **On 32-bit platforms**: Defined as the constant string `"%X"`.
* **On 64-bit platforms**: Defined as the constant string `"%llX"`.
```cpp
UWORD xnum = 42;
printf("xnum = " XWORD_PRINT "\n", xnum);
```
## CWORD_PRINT
### - ESP
Proporciona una cadena de texto en función de dos Parámetros, que indica como se debe formatear un `WORD` o `UWORD` dentro de una función de C, con una definición dependiente del tamaño de la palabra del procesador.
#### Parámetros:
* **Syntax**: Proporciona a la cadena de texto resultante el formato, siguiendo el estandar de funciones formateadas de C.
* **Sign**: Proporciona a la cadena de texto resultante un único caracter que ayude a indicar el tipo numérico a formatear, siguiendo el estandar de funciones formateadas de C.
#### Definición:
* **En plataformas de 32 bits**: Esta macro se definirá como la cadena constante `"%" syntax sign`.
* **En plataformas de 64 bits**: Esta macro se definirá como la cadena constante `"%" syntax "ll" sign`.
```cpp
WORD num = 1234;
printf("num = " CWORD_PRINT("","d") "\n", num);
printf("3 caracteres de num: " CWORD_PRINT(".3","d") "\n", num);
```
#### Porque importa el ancho del especificador:
Las funciones variádicas no llevan metadatos de tipos.\
El callee solo sabe cuántos bytes leer según el carácter de conversión, y esa lectura debe coincidir con lo que el caller empujó.\
Usar un especificador de un ancho distinto al del `WORD`/`UWORD` real produce dos fallos según la plataforma:
* **32 bits**: `"%lld"` hace que la runtime consuma 8 bytes de un argumento de 4, desalineando **todos** los argumentos posteriores (valores basura y, con `%s` o `%p`, desreferencia de punteros inválidos).
* **64 bits**: `"%d"` consume un único espacio de 8 bytes, así que la lista no se desalinea, pero solo lee los 32 bits bajos del `WORD`: valores con magnitud igual o superior a 2³¹ salen truncados y con el signo del dword bajo, y una dirección impresa con `"%u"` pierde su mitad alta.
Estas macros existen para que el ancho lo decida el preprocesador y no el programador, por lo que se recomienda no escribir `"%d"` o `"%lld"` a mano junto a valores de tipo `WORD`/`UWORD`.
#### Compatibilidad de la plataforma destino:
El formato lo interpreta la runtime C del sistema, no System Tools. En Windows la resolución intenta primero `msvcrt.dll` y solo después la UCRT (`__stdio_common_v*printf`), así que en crt antiguas el prefijo `ll` puede no reconocerse y se requiere el equivalente `I64`. Revisa la runtime mínima de tu plataforma destino antes de depender de `"%lld"` con la promesa de compatibilidad con Windows NT 3.1.
### - ENG
Provides a string literal based on two parameters, specifying how a `WORD` or `UWORD` should be formatted inside a C function, with a definition that depends on the processor word size.
#### Parameters:
* **Syntax**: Supplies format specifiers to the resulting string literal, adhering to standard C formatting conventions.
* **Sign**: Supplies a single character to specify the numeric type to format, adhering to standard C formatting conventions.
#### Definition:
* **On 32-bit platforms**: Defined as the constant string `"%" syntax sign`.
* **On 64-bit platforms**: Defined as the constant string `"%" syntax "ll" sign`.
```cpp
WORD num = 1234;
printf("num = " CWORD_PRINT("","d") "\n", num);
printf("3 characters of num: " CWORD_PRINT(".3","d") "\n", num);
```
#### Why the specifier width matters:
Variadic calls carry no type metadata.\
The callee only knows how many bytes to read from the conversion character, and that read must match what the caller pushed.\
Using a specifier whose width differs from the actual `WORD`/`UWORD` produces two different failures depending on the platform:
* **32 bits**: `"%lld"` makes the runtime consume 8 bytes out of a 4-byte argument, misaligning **every** following argument (garbage values, and with `%s` or `%p`, dereferencing of invalid pointers).
* **64 bits**: `"%d"` consumes a single 8-byte slot, so the argument list stays aligned, but only the low 32 bits of the `WORD` are read: magnitudes at or beyond 2³¹ come out truncated and signed by the low dword, and a pointer printed with `"%u"` loses its high half.
These macros exist so the preprocessor and not the programmer decides the width, so it is recommended not to write `"%d"` or `"%lld"` by hand next to `WORD`/`UWORD` values.
#### Target platform compatibility:
The format is interpreted by the system C runtime, not by System Tools. On Windows symbol resolution tries `msvcrt.dll` first and only then the UCRT (`__stdio_common_v*printf`), so on legacy crt the `ll` length prefix may go unrecognized and its `I64` equivalent is required. Verify the minimum runtime of your target platform before relying on `"%lld"` together with the Windows NT 3.1 compatibility promise.

## IS_LITTLE_ENDIAN
### - ESP
Proporciona una constante booleana que indica si el sistema donde se desea compilar esta en orden de alineamiento de Little Endian.\
El antonimo de esta macro es la macro `IS_BIG_ENDIAN`.
* **En plataformas Little Endian**: Esta macro se definirá como la constante `1`.
* **En plataformas Big Endian**: Esta macro se definirá como la constante `0`.
```cpp
#if IS_LITTLE_ENDIAN
puts("Hola desde Little Endian!");
#else
puts("Hola desde Big Endian!");
#endif
```
### - ENG
Provides a boolean constant indicating whether the target system uses Little-Endian byte ordering.\
The counterpart to this macro is `IS_BIG_ENDIAN`.
* **On Little-Endian platforms**: Defined as the constant `1`.
* **On Big-Endian platforms**: Defined as the constant `0`.
```cpp
#if IS_LITTLE_ENDIAN
puts("Hello from Little Endian!");
#else
puts("Hello from Big Endian!");
#endif
```

## IS_BIG_ENDIAN
### - ESP
Proporciona una constante booleana que indica si el sistema donde se desea compilar esta en orden de alineamiento de Big Endian.\
El antonimo de esta macro es la macro `IS_LITTLE_ENDIAN`.
* **En plataformas Little Endian**: Esta macro se definirá como la constante `0`.
* **En plataformas Big Endian**: Esta macro se definirá como la constante `1`.
```cpp
#if IS_BIG_ENDIAN
puts("Hola desde Big Endian!");
#else
puts("Hola desde Little Endian!");
#endif
```
### - ENG
Provides a boolean constant indicating whether the target system uses Big-Endian byte ordering.\
The counterpart to this macro is IS_LITTLE_ENDIAN.
* **On Little-Endian platforms**: Defined as the constant `0`.
* **On Big-Endian platforms**: Defined as the constant `1`.
```cpp
#if IS_BIG_ENDIAN
puts("Hello from Big Endian!");
#else
puts("Hello from Little Endian!");
#endif
```

## ENDIAN_MULT
### - ESP
Proporciona una constante númerica con signo que indica si el sistema donde se desea compilar esta en orden de alineamiento de Little Endian o Big Endian.\
Su definición está estrechamente relacionada con las macros `IS_LITTLE_ENDIAN` y `IS_BIG_ENDIAN`.
* **En plataformas Little Endian**: Esta macro se definirá como la constante `1`.
* **En plataformas Big Endian**: Esta macro se definirá como la constante `-1`.
```cpp
unsigned int num = 0x12345678;
const BYTE* ptr = (const BYTE*)&num;
BYTE lsb = ptr[(3 * (1 - ENDIAN_MULT)) / 2];
printf("El byte LSB de 0x%X es: 0x%02X\n", num, lsb);
```
### - ENG
Provides a signed numeric constant indicating whether the target system uses Little-Endian or Big-Endian byte ordering.\
Its definition is closely tied to the `IS_LITTLE_ENDIAN` and `IS_BIG_ENDIAN` macros.
* **On Little-Endian platforms**: Defined as the constant `1`.
* **On Big-Endian platforms**: Defined as the constant `-1`.
```cpp
unsigned int num = 0x12345678;
const BYTE* ptr = (const BYTE*)&num;
BYTE lsb = ptr[(3 * (1 - ENDIAN_MULT)) / 2];
printf("The LSB byte of 0x%X is: 0x%02X\n", num, lsb);
```

## VA_START
### - ESP
Proporciona una variable de nombre a elección en función de sus Parámetros, con el objetivo de operar con un Variadic Argument. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Prev_arg**: Nombre del parametro previo al Variadic Argument, dentro de la función.
* **Va_name**: Nombre que se le desee dar a la variable resultande a la operación.
```cpp
void print_numbers(UWORD amount, ...){
    VA_START(amount, args); // Crea una variable llamada 'args'.
    putchar('[');
    while(amount){
        --amount;
        printf("%d", VA_NEXT(args, int));
        if(amount) printf(", ");
    }
    putchar(']');
    VA_END(args);
}
```
### - ENG
Initializes a variadic argument handler with a user-defined name based on the function's parameters. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Prev_arg**: The name of the parameter immediately preceding the variadic argument list in the function signature.
* **Va_name**: The identifier assigned to the resulting variadic argument variable.
```cpp
void print_numbers(UWORD amount, ...){
    VA_START(amount, args); // Creates a variable named 'args'.
    putchar('[');
    while(amount){
        --amount;
        printf("%d", VA_NEXT(args, int));
        if(amount) printf(", ");
    }
    putchar(']');
    VA_END(args);
}
```

## VA_NEXT
### - ESP
Proporciona uno de los Parámetros resueltos de un Variadic Argument, y luego avanza en un argumento el Variadic Argument resuleto; tanto el Variadic Argument resuleto a usar como tambien el tipo que se retorna dependen de los Parámetros utilizados. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Va_name**: Nombre de la variable que almacena la resolución de un Variadic Argument.
* **Type**: Tipo de dato a retornar.
```cpp
double sum_all(UWORD count, ...){
    VA_START(count, args);
    double total = 0;
    while(count){
        --count;
        total += VA_NEXT(args, double); // Obtiene un 'double' del Variadic Argument, lo usa para sumarlo con 'total', y pasa al siguiente argumento.
    }
    VA_END(args);
    return total;
}
```
### - ENG
Retrieves the current argument from a resolved variadic argument list and advances the internal reader pointer to the next element. Both the variadic list handler and the returned type depend on the provided parameters. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Va_name**: The identifier of the variadic argument list handler.
* **Type**: The data type to cast and return.
```cpp
double sum_all(UWORD count, ...){
    VA_START(count, args);
    double total = 0;
    while(count){
        --count;
        total += VA_NEXT(args, double); // Retrieves a 'double' from the variadic list, adds it to 'total', and advances to the next argument.
    }
    VA_END(args);
    return total;
}
```

## VA_PEEK
### - ESP
Proporciona, de forma indexada numericamente, uno de los Parámetros resueltos de un Variadic Argument, pero a diferencia de la macro `VA_NEXT`, este no realiza un procedimiento de avance al Variadic Argument resuelto. tanto el Variadic Argument resuleto a usar como tambien cual argumento se va a leer y el tipo que se retorna dependen de los Parámetros utilizados. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Va_name**: Nombre de la variable que almacena la resolución de un Variadic Argument.
* **Idx**: Número sin signo (Sea de variable o constante) que indica que argumento se desea obtener desde la posición argumental relativa de la variable empleada para el argumento `Va_name`.
* **Type**: Tipo de dato a retornar.
```cpp
bool validate_and_process(UWORD total_args, ...){
    VA_START(total_args, args);
    if(total_args < 2){
        VA_END(args);
        return false;
    }
    BYTE* second_ptr = VA_PEEK(args, 1, BYTE*); // Obtiene el segundo argumento.
    if(!second_ptr){
        VA_END(args);
        return false;
    }
    while(total_args){
        --total_args;
        BYTE* ptr = VA_NEXT(args, BYTE*);
        bool cnd = ptr < second_ptr;
        if(cnd) cnd = second_ptr - ptr > 64;
        else    cnd = ptr - second_ptr > 64;
        if(cnd){
            VA_END(args);
            return false;
        }
    }
    VA_END(args);
    return true;
}
```
### - ENG
Inspects an argument from a resolved variadic argument list at a specified zero-based index without advancing the internal reader pointer. The variadic handler, target index, and returned type depend on the supplied parameters. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Va_name**: The identifier of the variadic argument list handler.
* **Idx**: An unsigned integer (variable or constant) specifying the relative index of the argument to inspect.
* **Type**: The data type to cast and return.
```cpp
bool validate_and_process(UWORD total_args, ...){
    VA_START(total_args, args);
    if(total_args < 2){
        VA_END(args);
        return false;
    }
    BYTE* second_ptr = VA_PEEK(args, 1, BYTE*); // Inspects the second argument.
    if(!second_ptr){
        VA_END(args);
        return false;
    }
    while(total_args){
        --total_args;
        BYTE* ptr = VA_NEXT(args, BYTE*);
        bool cnd = ptr < second_ptr;
        if(cnd) cnd = second_ptr - ptr > 64;
        else    cnd = ptr - second_ptr > 64;
        if(cnd){
            VA_END(args);
            return false;
        }
    }
    VA_END(args);
    return true;
}
```

## VA_RESET
### - ESP
Proporciona la capacidad de reiniciar una resolución de un Variadic Argument hacia el inicio. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Prev_arg**: Nombre del parametro previo al Variadic Argument, dentro de la función.
* **Va_name**: Nombre de la variable, que almacena la resolución de un Variadic Argument, que se desea reiniciar.
```cpp  
UWORD process_multi_pass(UWORD count, ...){
    VA_START(count, args);
    UWORD total_length = 0;
    for(UWORD i = 0; i < count; ++i){
        const char* text = VA_NEXT(args, const char*);
        while(*text){ ++total_length; ++text; }
        total_length += 2;
    }
    VA_RESET(count, args); // Reinicia la variable 'args', para que vuelva al inicio del Variadic Argument que resuelve.
    ++total_length;
    char* buffer = (char*)memory::alloc(total_length);
    total_length = 0;
    for(UWORD i = 0; i < count; ++i){
        const char* text = VA_NEXT(args, const char*);
        total_length += (UWORD)sprintf(buffer + total_length, "[%s]", text);
    }
    buffer[total_length] = '\0'; // 'sprintf' ya escribe el terminador; esta línea es opcional.
    printf("%s", buffer);
    memory::free(buffer);
    VA_END(args);
    return total_length;
}  
```
### - ENG
Resets a variadic argument list handler back to its initial position. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Prev_arg**: The name of the parameter immediately preceding the variadic argument list in the function signature.
* **Va_name**: The identifier of the variadic argument list handler to reset.
```cpp
UWORD process_multi_pass(UWORD count, ...){
    VA_START(count, args);
    UWORD total_length = 0;
    for(UWORD i = 0; i < count; ++i){
        const char* text = VA_NEXT(args, const char*);
        while(*text){ ++total_length; ++text; }
        total_length += 2;
    }
    VA_RESET(count, args); // Resets the 'args' variable back to the beginning of the variadic argument list.
    ++total_length;
    char* buffer = (char*)memory::alloc(total_length);
    total_length = 0;
    for(UWORD i = 0; i < count; ++i){
        const char* text = VA_NEXT(args, const char*);
        total_length += (UWORD)sprintf(buffer + total_length, "[%s]", text);
    }
    buffer[total_length] = '\0'; // 'sprintf' already writes the terminator; this line is optional.
    printf("%s", buffer);
    memory::free(buffer);
    VA_END(args);
    return total_length;
}
```

## VA_COPY
### - ESP
Proporciona la capacidad de crear un duplicado de una resolución de un Variadic Argument, tanto a que Variadic Argument pertenece como tambien de nivel de desplazamiento por el mismo. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Va_name**: Nombre de la variable, que almacena la resolución de un Variadic Argument, que se desea duplicar.
* **Va_copy_name**: Nombre que se le desee dar a la variable resultande a la operación, que representa la copia creada.
```cpp
void log_and_dispatch(void* file, const char* prefix, UWORD arg_count, ...){
    VA_START(arg_count, args_original);
    if(!file || !prefix || !arg_count){
        VA_END(args_original);
        return;
    }
    VA_COPY(args_original, args_copy); // Crea una copia de 'args_original'.
    UWORD prefix_len = 0; if(prefix) while(prefix[prefix_len]) ++prefix_len;
    char* buffer = (char*)memory::alloc(prefix_len + (WORD_SIZE << 3) + 4);
    for(UWORD i = 0; i < arg_count; ++i){
        WORD val = VA_NEXT(args_copy, WORD);
        UWORD size = sprintf(buffer, "%s: " WORD_PRINT "\n", prefix, val);
        file::write(file, buffer, size);
    }
    memory::free(buffer);
    VA_END(args_copy); // Liberar la copia.
    // La lista original permanece intacta y lista para procesarse
    for(UWORD i = 0; i < arg_count; ++i){
        WORD val = VA_NEXT(args_original, WORD);
        printf("%s: " WORD_PRINT "\n", prefix, val);
    }
    VA_END(args_original);
}
```
### - ENG
Duplicates a variadic argument handler, preserving both its target argument list and current read offset. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Va_name**: The identifier of the variadic argument list handler to duplicate.
* **Va_copy_name**: The identifier assigned to the newly created duplicate variable.
```cpp
void log_and_dispatch(void* file, const char* prefix, UWORD arg_count, ...){
    VA_START(arg_count, args_original);
    if(!file || !prefix || !arg_count){
        VA_END(args_original);
        return;
    }
    VA_COPY(args_original, args_copy); // Creates a copy of 'args_original'.
    UWORD prefix_len = 0; if(prefix) while(prefix[prefix_len]) ++prefix_len;
    char* buffer = (char*)memory::alloc(prefix_len + (WORD_SIZE << 3) + 4);
    for(UWORD i = 0; i < arg_count; ++i){
        WORD val = VA_NEXT(args_copy, WORD);
        UWORD size = sprintf(buffer, "%s: " WORD_PRINT "\n", prefix, val);
        file::write(file, buffer, size);
    }
    memory::free(buffer);
    VA_END(args_copy); // Releases the copy handler.
    // The original list remains intact and ready for processing
    for(UWORD i = 0; i < arg_count; ++i){
        WORD val = VA_NEXT(args_original, WORD);
        printf("%s: " WORD_PRINT "\n", prefix, val);
    }
    VA_END(args_original);
}
```

## VA_END
### - ESP
Proporciona la capacidad de dar por terminado el ciclo de vida de una variable de resolución de un Variadic Argument. Su definición puede variar dependiendo del sistema, pero su funcionalidad siempre será la misma.
#### Parámetros:
* **Va_name**: Nombre de la variable, que almacena la resolución de un Variadic Argument, que se desea dar por eliminada.
```cpp
void no_operation(UWORD arg_count, ...){
    VA_START(arg_count, args);
    VA_END(args); // Elimina a 'args', liberando la memoria que solía ocupar.
}
```
### - ENG
Terminates the lifecycle of a variadic argument list handler and cleans up any resources associated with it. Its underlying implementation may vary across operating systems, but its behavior remains consistent.
#### Parameters:
* **Va_name**: The identifier of the variadic argument handler to dispose.
```cpp
void no_operation(UWORD arg_count, ...){
    VA_START(arg_count, args);
    VA_END(args); // Cleans up 'args', releasing any resources it held.
}
```

## RESTRICT
### - ESP
Proporciona la palabra reservada de lenguaje `restrict` de C, implementando todas sus funcionalidades subsecuentes. Usada para indicar al compilador que un puntero es el único medio para acceder al objeto al que apunta durante su tiempo de vida.
```cpp
void sumar_vectores(int* RESTRICT a, int* RESTRICT b, int* RESTRICT resultado, UWORD n){
    for(UWORD i = 0; i < n; ++i) resultado[i] = a[i] + b[i];
}
```
### - ENG
Provides the C language keyword `restrict`, enabling its pointer-aliasing optimization capabilities. Used to inform the compiler that a given pointer is the sole means of accessing the underlying object throughout its lifetime.
```cpp
void add_vectors(int* RESTRICT a, int* RESTRICT b, int* RESTRICT result, UWORD n){
    for(UWORD i = 0; i < n; ++i) result[i] = a[i] + b[i];
}
```

## FORCEINLINE
### - ESP
Proporciona la palabra reservada de lenguaje `__forceinline` (o `__attribute__((always_inline))`, según el compilador) de C, que en esta librería es `inline ALWAYSINLINE`.\
Obliga al compilador a incrustar el código de una función directamente en el lugar donde es llamada, ignorando sus criterios heurísticos.\
A diferencia de la palabra reservada de lenguaje `inline`, que el compilador puede ignorar si la considera ineficiente, `FORCEINLINE` deja casi sin margen al optimizador para ignorarla. \
En algunos casos, donde esta directiva no exista en tu compilador, será definida como la palabra reservada de lenguaje `inline` en su lugar.
```cpp
struct Vector3{
    float x;
    float y;
    float z;
};

FORCEINLINE float get_quadratic_length(const Vector3* v){
    return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

void normalize_vectors(Vector3* vectors, UWORD n){
    for(UWORD i = 0; i < n; i++){
        float len_sq = get_quadratic_length(vectors + i);
        if(len_sq > 0.0f){
            float inv_len = 1.0f / sqrtf(len_sq);
            vectors[i].x *= inv_len;
            vectors[i].y *= inv_len;
            vectors[i].z *= inv_len;
        }
    }
}
```
### - ENG
Provides the compiler-specific directive `__forceinline` (or `__attribute__((always_inline))`, depending on the toolchain), which is `inline ALWAYSINLINE` in this library.\
Used to force the compiler to inline a function's body directly at each call site, bypassing standard heuristic decisions.\
Unlike the standard `inline` keyword, which the compiler may freely ignore if deemed inefficient, `FORCEINLINE` instructs the compiler to override optimizer heuristics.\
If the underlying compiler does not support explicit forced inlining, it falls back to the standard `inline` keyword.
```cpp
struct Vector3{
    float x;
    float y;
    float z;
};

FORCEINLINE float get_quadratic_length(const Vector3* v){
    return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

void normalize_vectors(Vector3* vectors, UWORD n){
    for(UWORD i = 0; i < n; i++){
        float len_sq = get_quadratic_length(vectors + i);
        if(len_sq > 0.0f){
            float inv_len = 1.0f / sqrtf(len_sq);
            vectors[i].x *= inv_len;
            vectors[i].y *= inv_len;
            vectors[i].z *= inv_len;
        }
    }
}
```

## ALWAYSINLINE
### - ESP
Proporciona el atributo del compilador que pide incrustar una funcion, sin tocar el enlace de la definicion: `__forceinline` en MSVC y `__attribute__((always_inline))` en GCC y Clang, y cadena vacia donde ninguno de los dos existe.\
A diferencia de `FORCEINLINE`, esta macro nunca escribe `inline` por su cuenta, asi que va delante de una definicion que ya lo lleva, que es lo que hace `FUNCDECL` con todo el cuerpo de la libreria.\
Sin un compilador que la soporte, la macro se define vacia y la funcion queda solo con el `inline` que ya traia.
```cpp
ALWAYSINLINE int square(int v){
	return v * v;
}
```
### - ENG
Provides the compiler attribute that requests inlining without touching the linkage of the definition: `__forceinline` on MSVC, `__attribute__((always_inline))` on GCC and Clang, and an empty string where neither exists.\
Unlike `FORCEINLINE`, this macro never writes `inline` on its own, so it goes in front of a definition that already carries it, which is what `FUNCDECL` does with the whole body of the library.\
Without a compiler that supports it the macro is defined empty and the function is left with only the `inline` it already had.
```cpp
ALWAYSINLINE int square(int v){
	return v * v;
}
```

## NOINLINE
### - ESP
Proporciona el atributo del compilador que prohibe incrustar una funcion: `__declspec(noinline)` en MSVC y `__attribute__((noinline))` en GCC y Clang.\
Se necesita cuando el codigo recorre su propio marco de pila, como hace el andamiaje `VA_START` sobre Windows: si el optimizador disuelve la funcion variadica dentro del llamador, la pila que aquella recorre deja de ser la suya y la lectura devuelve basura. `printf`, `sprintf`, `scanf` y `sscanf` la llevan por eso.\
Es la contrapartida de `ALWAYSINLINE`, y donde el compilador no la conoce la macro se define vacia.
```cpp
FUNCDECL(NOINLINE int my_scan, const char* format, ...){
	VA_START(format, args);
	// ...
}
```
### - ENG
Provides the compiler attribute that forbids inlining a function: `__declspec(noinline)` on MSVC, `__attribute__((noinline))` on GCC and Clang.\
It is needed when the code walks its own stack frame, the way the `VA_START` scaffolding does on Windows: if the optimizer dissolves the variadic function into the caller, the stack it walks stops being its own and the read comes back garbage. `printf`, `sprintf`, `scanf` and `sscanf` carry it for that reason.\
It is the counterpart of `ALWAYSINLINE`, and where the compiler does not know it the macro is defined empty.
```cpp
FUNCDECL(NOINLINE int my_scan, const char* format, ...){
	VA_START(format, args);
	// ...
}
```
