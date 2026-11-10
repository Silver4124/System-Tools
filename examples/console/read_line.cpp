/* // ESP
	Ejemplo de lectura por lineas de la terminal.
	Muestra: console::read con texto ancho, la propiedad del buffer, su capacidad
	y su liberacion, mas el regreso al texto multibyte para printf.
	Compilar: g++ read_line.cpp -o read_line
	Requiere: #define ST_IMPORT_CONSOLE antes de incluir la cabecera.
	Nota: la funcion bloquea hasta que la terminal tiene una linea completa, o sea hasta Enter.
	// ENG
	Example of line-by-line terminal reading.
	Shows: console::read over wide text, the buffer ownership, its capacity and its release,
	plus the way back to multibyte text for printf.
	Build: g++ read_line.cpp -o read_line
	Needs: #define ST_IMPORT_CONSOLE before including the header.
	Note: the function blocks until the terminal holds a whole line, that is, until Enter.
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	if(!console::get_handle()){ puts("sin consola utilizable / no usable console"); return 1; }

	// ESP: El puntero debe ser nullptr o una direccion de memory::alloc: read lo realoca por su cuenta.
	// Una direccion del stack estaria destruida en la segunda llamada.
	// ENG: The pointer must be nullptr or a memory::alloc address: read reallocates it on its own.
	// A stack address would be destroyed by the second call.
	wchar* line = nullptr;
	UWORD len = 0;

	puts("escribe dos lineas (la segunda vacia termina)");
	for(int turn = 0; turn < 2; ++turn){
		// ESP: El retorno es el handle: nullptr solo avisa que la consulta no valio la pena.
		// ENG: The return is the handle: nullptr only warns the query was not worth it.
		void* r = console::read(nullptr, &line, &len);
		if(!r){ puts("handle invalido / invalid handle"); return 1; }

		// ESP: Len 0 y texto nulo es "leí vacío", distinto de "el handle no vale".
		// ENG: Len 0 with null text is "I read nothing", different from "the handle is worthless".
		if(!len && !line){ puts("lectura vacia / empty reading"); break; }

		// ESP: write no agrega el salto: aqui se devuelve lo leido tal cual, salto incluido.
		// ENG: write adds no line break: here what was read goes back as is, newline included.
		console::write(nullptr, line, len);

		// ESP: La memoria es del modulo: szlen da la capacidad y len la longitud textual.
		// La capacidad siempre trae lugar de sobra para el terminador.
		// ENG: The memory belongs to the module: szlen gives capacity and len the textual length.
		// The capacity always carries spare room for the terminator.
		printf("unidades leidas: " UWORD_PRINT ", capacidad en wchar: " UWORD_PRINT "\n",
			len, memory::szlen(line) / (UWORD)sizeof(wchar));

		// ESP: El salto de linea forma parte del texto: quitarlo es trabajo del programador.
		// ENG: The line break is part of the text: taking it off is the programmer's job.
		while(len && (line[len - 1] == L'\n' || line[len - 1] == L'\r')) --len;

		// ESP: De vuelta a bytes para printf: size cuenta unidades de wchar, el retorno da bytes.
		// stacked en false reserva con memory::alloc, asi que se libera con memory::free.
		// ENG: Back to bytes for printf: size counts wchar units, the return gives bytes.
		// stacked as false reserves with memory::alloc, so it gets released with memory::free.
		char* text = nullptr;
		UWORD bytes = wcstombs(&text, line, len, false);
		printf("como bytes UTF-8 (" UWORD_PRINT "): %s\n", bytes, text ? text : "");
		if(!memory::free(text)) puts("bytes liberados / bytes released");

		// ESP: Cada llamada reemplaza lo anterior y nunca añade: la linea vieja deja de servir.
		// ENG: Each call replaces what came before and never appends: the old line stops working.
	}

	// ESP: El buffer queda en manos del programador al salir del bucle: el modulo no lo borra.
	// ENG: The buffer lands on the programmer's hands when the loop ends: the module does not wipe it.
	if(line) memory::free(line);
	line = nullptr;

	// ESP: Con un puntero de texto aparte se descarta lo leido y solo importa cuanto se consumo.
	// Los caracteres salen de la cola igual: leer no sirve como sondeo.
	// Ojo: nullptr literal no vale, elegiria la sobrecarga que rechaza la lectura sin intentarla.
	// ENG: With a separate text pointer the reading is thrown away and only the consumed amount matters.
	// The characters still leave the queue: reading is no good as a poll.
	// Beware: a literal nullptr is no use, it would pick the overload that refuses the reading without attempting it.
	wchar* dropped_text = nullptr;
	UWORD dropped = 0;
	if(console::read(nullptr, &dropped_text, &dropped))
		printf("linea descartada: " UWORD_PRINT " unidades / line dropped: " UWORD_PRINT " units\n", dropped, dropped);
	if(dropped_text) memory::free(dropped_text);

	return 0;
}
