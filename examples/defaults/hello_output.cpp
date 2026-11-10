/* // ESP
	Ejemplo de salida formateada sin ninguna dependencia externa.
	Muestra: printf, puts, putchar, sprintf y las macros de formato
	WORD_PRINT, UWORD_PRINT, OWORD_PRINT, XWORD_PRINT y CWORD_PRINT.
	No hace falta definir ninguna macro ST_IMPORT_*: son funciones base.
	Compilar: g++ hello_output.cpp -o hello_output   (o `make` desde la raiz)
	La libreria reenvia el formato a la runtime C del sistema, asi que aqui
solo se usan especificadores comunes a Windows y Linux.
	// ENG
	Formatted output example with no external dependency.
	Shows: printf, puts, putchar, sprintf and the format macros
	WORD_PRINT, UWORD_PRINT, OWORD_PRINT, XWORD_PRINT and CWORD_PRINT.
	No ST_IMPORT_* macro is needed: these are base functions.
	Build: g++ hello_output.cpp -o hello_output   (or `make` from the root)
	The library forwards the format to the system C runtime, so only
	specifiers common to Windows and Linux are used here.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: Un printf normal, sin incluir ninguna cabecera de C.
	// ENG: A plain printf, without including any C header.
	printf("System Tools %s\n", "hello_output");

	// ESP: WORD y UWORD miden lo que la palabra del procesador, 32 o 64 bits.
	// ENG: WORD and UWORD are as wide as the processor word, 32 or 64 bits.
	WORD  signed_value   = -1000000;
	UWORD unsigned_value = 1000000;

	// ESP: Las macros de formato se concatenan como cadenas literales.
	// ENG: The format macros concatenate as string literals.
	printf("WORD   = " WORD_PRINT "\n",  signed_value);
	printf("UWORD  = " UWORD_PRINT "\n", unsigned_value);

	// ESP: Las bases octal y hexadecimal tienen su propia macro.
	// ENG: Octal and hexadecimal bases have their own macro.
	printf("octal  = " OWORD_PRINT "\n", unsigned_value);
	printf("hexa   = " XWORD_PRINT "\n", unsigned_value);

	// ESP: CWORD_PRINT anade ancho o precision delante del especificador.
	// ENG: CWORD_PRINT prepends width or precision to the specifier.
	printf("relleno= " CWORD_PRINT("12","d") "\n", signed_value);
	printf("precision= " CWORD_PRINT(".3","f") "\n", 3.14159);

	// ESP: Un % antes de %% es literal; %% imprime el propio signo de porcentaje.
	// ENG: A % before %% is literal; %% prints the percent sign itself.
	printf("por ciento: 50%%\n");

	// ESP: sprintf escribe en un buffer propio, sin asignar memoria.
	// ENG: sprintf writes into your own buffer, allocating nothing.
	char line[64];
	int written = sprintf(line, "%s -> %d", "sprintf", 42);
	printf("sprintf devolvio %d caracteres: %s\n", written, line);

	// ESP: puts agrega el salto de linea; putchar no, y trabaja con un solo caracter.
	// ENG: puts appends the newline; putchar does not, and handles one character.
	puts("puts anade su propio salto");
	putchar('p'); putchar('u'); putchar('t'); putchar('c'); putchar('h'); putchar('a'); putchar('r');
	putchar('\n');

	return 0;
}
