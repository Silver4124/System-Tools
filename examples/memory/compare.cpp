/* // ESP
	Ejemplo de las funciones de copia y comparacion binaria de la libreria.
	Muestra: memory::slloc, memory::clloc, memory::mlloc, memory::cnrlloc y
	memory::cmplloc, con sus convenciones de retorno poco intuitivas.
	Compilar: g++ compare.cpp -o compare
	No hace falta definir ninguna macro ST_IMPORT_*: memory es una funcion base.
	// ENG
	Example of the library binary copy and compare functions.
	Shows: memory::slloc, memory::clloc, memory::mlloc, memory::cnrlloc and
	memory::cmplloc, with their counter-intuitive return conventions.
	Build: g++ compare.cpp -o compare
	No ST_IMPORT_* macro is needed: memory is a base function.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	BYTE source[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	BYTE dest[8];

	// ESP: slloc rellena bytes, es la variante de memset que no devuelve basura.
	// ENG: slloc fills bytes, it is the memset variant that does not hand back garbage.
	memory::slloc(dest, 0, 8);
	printf("dest antes de copiar: %d %d\n", dest[0], dest[7]);

	// ESP: clloc es memcpy: requiere buffers que no se solapen.
	// ENG: clloc is memcpy: it needs buffers that do not overlap.
	if(memory::clloc(dest, source, 8)) puts("copia hecha / copy done");
	printf("dest[0] = %d, dest[7] = %d\n", dest[0], dest[7]);

	// ESP: Con Dest e Src en la misma direccion clloc no copia nada y devuelve Dest.
	// Es un no-op, no un fallo, pero el promesa RESTRICT hace que el compilador lo marque.
	// ENG: With Dest and Src on the same address clloc copies nothing and returns Dest.
	// It is a no-op, not a failure, but the RESTRICT promise makes the compiler flag it.

	// ESP: mlloc es memmove: es la unica segura dentro del mismo buffer.
	// El solapamiento parcial con clloc es comportamiento indefinido.
	// ENG: mlloc is memmove: the only safe one inside the same buffer.
	// Partial overlap with clloc is undefined behaviour.
	BYTE slide[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	// ESP: Desplaza los seis primeros bytes hacia direcciones mayores.
	// ENG: Shifts the first six bytes towards higher addresses.
	memory::mlloc(slide + 2, slide, 6);
	printf("slide tras mlloc: %d %d %d %d\n", slide[0], slide[1], slide[2], slide[3]);

	// ESP: cnrlloc responde el puntero del buffer menor cuando difieren.
	// ENG: cnrlloc answers the pointer of the lesser buffer when they differ.
	BYTE lesser[8] = {1, 2, 3, 4, 5, 6, 7, 7};
	// ESP: unsigned int y UWORD son tipos distintos en 64 bits: fija el tipo de la medida.
	// ENG: unsigned int and UWORD are different types on 64 bits: pin the measure type.
	unsigned int len = 8;
	void* blame = memory::cnrlloc(source, lesser, len);
	// ESP: Comparar el retorno con cada puntero dice quien perdio.
	// ENG: Matching the return against each pointer says who lost.
	if(blame == source) puts("source es el menor / source is the lesser");
	else if(blame == lesser) puts("lesser es el menor / lesser is the lesser");
	else puts("contenidos identicos (XOR de direcciones) / identical content (address XOR)");

	// ESP: cmplloc es la version booleana: nullptr significa "distintos o no comparable".
	// ENG: cmplloc is the boolean version: nullptr means "different or not comparable".
	if(memory::cmplloc(source, lesser, len)) puts("cmplloc: iguales / cmplloc: equal");
	else puts("cmplloc: distintos / cmplloc: different");

	// ESP: El caso inverso devuelve un valor no nulo, el XOR de las dos direcciones.
	// No es una direccion util: solo sirve como verdad.
	// ENG: The opposite case returns a non null value, the XOR of both addresses.
	// It is not a usable address: it only works as truth.
	BYTE twin[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	void* equal_token = memory::cmplloc(source, twin, len);
	printf("cmplloc sobre iguales -> %p (distinto de nullptr = iguales)\n", equal_token);

	// ESP: Tamano 0 o un puntero nulo tambien producen nullptr: nunca es un "iguales".
	// ENG: Size 0 or a null pointer also give nullptr: it never means "equal".
	printf("cmplloc con tamano 0 -> %p\n", memory::cmplloc(source, twin, 0u));

	return 0;
}
