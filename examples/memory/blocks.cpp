/* // ESP
	Ejemplo del manejo de memoria propio de la libreria.
	Muestra: memory::alloc, memory::realloc, memory::szlen, memory::free y
	memory::on_stack, con la convencion invertida de free.
	Compilar: g++ blocks.cpp -o blocks
	No hace falta definir ninguna macro ST_IMPORT_*: memory es una funcion base.
	// ENG
	Example of the library own memory handling.
	Shows: memory::alloc, memory::realloc, memory::szlen, memory::free and
	memory::on_stack, including free inverted convention.
	Build: g++ blocks.cpp -o blocks
	No ST_IMPORT_* macro is needed: memory is a base function.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: alloc responde nullptr cuando se pide tamano 0, no solo cuando falla.
	// ENG: alloc answers nullptr when size 0 is asked, not only when it fails.
	BYTE* data = (BYTE*)memory::alloc(64);
	if(!data){ puts("reserva rechazada / allocation refused"); return 1; }

	// ESP: En Windows el bloque llega sin limpiar; en Linux viene en cero.
	// slloc pone los 64 bytes a cero en las dos plataformas.
	// ENG: On Windows the block arrives uncleaned; on Linux it comes zeroed.
	// slloc zeroes these 64 bytes on both platforms.
	memory::slloc(data, 0, 64);
	printf("data[0] = %d\n", data[0]);

	// ESP: szlen pregunta la capacidad utilizable del bloque, no lo que se pidio.
	// Suele salir algo mayor que el tamano entregado a alloc.
	// ENG: szlen asks the usable capacity of the block, not what was requested.
	// It usually comes out a bit larger than the size handed to alloc.
	UWORD capacity = memory::szlen(data);
	printf("capacidad del bloque: " UWORD_PRINT "\n", capacity);

	// ESP: Un puntero del Stack nunca tiene tamano consultable: szlen responde 0.
	// ENG: A Stack pointer never has a queryable size: szlen answers 0.
	BYTE stack_block[16];
	printf("szlen sobre memoria del Stack: " UWORD_PRINT "\n", memory::szlen(stack_block));

	// ESP: on_stack distingue quien es quien sin consultar tamano.
	// ENG: on_stack tells them apart without asking for a size.
	printf("data en el stack: %d, stack_block en el stack: %d\n",
		memory::on_stack(data), memory::on_stack(stack_block));

	// ESP: realloc con nullptr se comporta como alloc, y con tamano 0 como free.
	// ENG: realloc with nullptr behaves like alloc, and with size 0 like free.
	BYTE* moved = (BYTE*)memory::realloc(data, 256);
	// ESP: Si devuelve nullptr el bloque viejo sigue vivo y es el unico valido.
	// ENG: If it returns nullptr the old block is still alive and the only valid one.
	if(!moved){ puts("realloc rechazado / realloc refused"); memory::free(data); return 1; }
	data = moved;
	printf("tras realloc: capacidad " UWORD_PRINT "\n", memory::szlen(data));

	// ESP: Escribir donde corresponde: el bloque ya aguanta 256 bytes.
	// La capacidad real suele sobrar espacio extra, pero se usa lo pedido.
	// ENG: Writing where it belongs: the block now holds 256 bytes.
	// Real capacity usually gives extra room, but only the asked size is used.
	for(UWORD i = 0; i < 256; ++i) data[i] = (BYTE)i;
	printf("data[255] = %d\n", data[255]);

	// ESP: free devuelve nullptr cuando SI libero: la comprobacion es invertida.
	// ENG: free returns nullptr when it DID release: the check is inverted.
	if(!memory::free(data)) puts("bloque liberado / block released");
	else puts("el bloque sigue en memoria / block is still in memory");
	data = nullptr;

	// ESP: Pedir 0 bytes es un fallo silencioso, no una reserva vacia.
	// ENG: Asking 0 bytes is a silent failure, not an empty reservation.
	void* nothing = memory::alloc(0);
	printf("alloc(0) -> %p\n", nothing);

	return 0;
}
