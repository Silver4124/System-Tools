/* // ESP
	Contrajemplo: file::normalize aplicado sobre una literal de texto.
	Compilar: g++ normalize_string_literal.cpp -o normalize_string_literal
	Fallo: al ejecutar, segmentation fault (139 en MSYS bash, 0xC0000005 en cmd).
	El aviso del compilado es otro: ISO C++ forbids converting a string constant to 'char*'.
	Causa: normalize recibe char* sin const y escribe encima del mismo buffer; una literal vive
	en una seccion de solo lectura, asi que la primera escritura sobre ella tumba el proceso.
	Salida: copiar la ruta a un buffer escribible, con memory::alloc o con un arreglo de char, y normalizar ahi.
	Nota: al caer de golpe es probable que no se vea ni la linea impresa antes del fallo: la
	salida del printf propio va bufferizada y el corte se la lleva. Este fichero esta escrito para fallar.
	// ENG
	Counterexample: file::normalize applied over a text literal.
	Build: g++ normalize_string_literal.cpp -o normalize_string_literal
	Failure: when running, segmentation fault (139 in MSYS bash, 0xC0000005 in cmd).
	The warning at the compiling is a different one: ISO C++ forbids converting a string constant to 'char*'.
	Cause: normalize takes a char* with no const and writes over the very same buffer; a literal lives
	in a read only section, so the first write over it drops the process.
	The way out: copy the path into a writable buffer, with memory::alloc or with an array of char, and normalize there.
	Note: falling down at once means the line printed before the failure probably never shows: the
	own printf output is buffered and the cut takes it away. This file is written to fail.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	printf("antes de normalize\n");

	// ESP: aqui esta el fallo: la literal es const char* y normalize escribe dentro de ella.
	// ENG: here is the failure: the literal is const char* and normalize writes inside it.
	char* out = file::normalize("C:/dir/file.txt");

	// ESP: La forma correcta, en el bloque de abajo: un buffer propio, con copia previa. Apague
	// la linea del fallo antes de probarla, porque las dos escriben sobre la misma out.
	// ENG: The right shape, in the block below: a buffer of your own, with a prior copy. Turn the
	// failing line off before testing it, since both write over the very same out.
#if 0
	char buf[64];
	memory::clloc(buf, "C:/dir/file.txt", 16);
	out = file::normalize(buf);
#endif

	printf("nunca se ve: [%s]\n", out);
	return 0;
}
