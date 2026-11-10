/* // ESP
	Ejemplo de las rutas paralelas de la consola: las versiones de write y read que
	no estan declaradas en el encabezado pero si definidas en el modulo.
	Muestra: console::write sin longitud (char*, wchar* y const void*), console::write
	con nullptr, console::read por char**, console::read por void** y console::read
	con nullptr literal.
	Compilar: g++ console_paths.cpp -o console_paths
	Requiere: #define ST_IMPORT_CONSOLE antes de incluir la cabecera.
	Nota: el encabezado solo declara write(cnsl, wchar*, len) y read(cnsl, wchar**, len*);
	las demas firmas viven solo en la definicion, pero al ser una libreria de cabecera
	el programa las ve y las enlaza sin problema.
	// ENG
	Example of the parallel roads of the console: the write and read versions that are
	not declared in the header but are defined in the module.
	Shows: console::write without a length (char*, wchar* and const void*), console::write
	with nullptr, console::read over char**, console::read over void** and console::read
	with a literal nullptr.
	Build: g++ console_paths.cpp -o console_paths
	Needs: #define ST_IMPORT_CONSOLE before including the header.
	Note: the header declares only write(cnsl, wchar*, len) and read(cnsl, wchar**, len*);
	the other signatures live only in the definition, but being a header library
	the program sees them and links them without trouble.
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	void* cnsl = console::get_handle();
	if(!cnsl){ puts("sin consola utilizable / no usable console"); return 1; }

	// ESP: Sin longitud, la version char* mide el texto ella sola buscando el terminador.
	// ENG: Without a length, the char* version measures the text alone by hunting the terminator.
	UWORD a = console::write(cnsl, "write sin longitud, ruta de bytes\r\n");

	// ESP: La version wchar* hace la misma tarea sobre unidades anchas: cuenta wchar_t, no bytes.
	// ENG: The wchar* version does the same deed over wide units: it counts wchar_t, not bytes.
	UWORD b = console::write(cnsl, L"write sin longitud, ruta ancha\r\n");

	// ESP: La version const void* NO es neutra: reinterpreta el puntero como wchar* siempre.
	// Con texto ancho quedan bien; con texto de bytes sale basura, y aqui solo se usa lo ancho.
	// ENG: The const void* version is NOT neutral: it reinterprets the pointer as wchar* always.
	// With wide text it lands fine; with byte text garbage comes out, so only wide stuff goes here.
	const wchar* wide = L"write por const void*, tomado como ancho\r\n";
	UWORD c = console::write(cnsl, (const void*)wide);
	printf("unidades escritas: " UWORD_PRINT ", " UWORD_PRINT ", " UWORD_PRINT "\n", a, b, c);

	// ESP: Con el texto en nullptr la llamada se da por hecha sin escribir nada: responde 0.
	// Sirve para ramificar sin duplicar el codigo, no para avisar de un fallo.
	// ENG: With the text in nullptr the call counts as done without writing anything: it answers 0.
	// It is useful to branch without duplicating the code, not to signal a failure.
	UWORD d = console::write(cnsl, nullptr);
	UWORD e = console::write(cnsl, nullptr, 10);
	printf("write nulo: " UWORD_PRINT " y " UWORD_PRINT "\n", d, e);

	// ESP: read por char** entrega bytes crudos: Len cuenta bytes, no caracteres.
	// Con una codificacion multibyte la cifra sube por encima de lo tecleado.
	// ENG: read over char** hands over raw bytes: Len counts bytes, not characters.
	// Under a multibyte encoding the figure climbs above what was typed.
	puts("escribe una linea / type a line");
	char* bytes = nullptr;
	UWORD nby = 0;
	if(console::read(cnsl, &bytes, &nby) && nby){
		printf("bytes leidos: " UWORD_PRINT " -> [", nby);
		for(UWORD i = 0; i < nby; ++i) putchar(bytes[i]);
		puts("]");
		// ESP: La capacidad real de la memoria es mayor que lo leido: szlen la pregunta.
		// La libreria es la duena: se suelta con memory::free o la reemplaza la siguiente lectura.
		// ENG: The real capacity of the memory is larger than what was read: szlen asks it.
		// The library owns it: it gets released with memory::free or the next reading replaces it.
		printf("capacidad en bytes: " UWORD_PRINT "\n", memory::szlen(bytes));
	}

	// ESP: read por void** toma la ruta ancha y reinterpreta el resultado: la memoria es de wchar_t.
	// Por eso la capacidad se divide entre sizeof(wchar) para contar unidades anchas.
	// ENG: read over void** takes the wide road and reinterprets the result: the memory is wchar_t's.
	// That is why the capacity gets divided by sizeof(wchar) to count wide units.
	void* raw = nullptr;
	UWORD nraw = 0;
	if(console::read(cnsl, &raw, &nraw) && nraw){
		wchar* as_wide = (wchar*)raw;
		printf("unidades anchas: " UWORD_PRINT ", capacidad en wchar: " UWORD_PRINT "\n",
			nraw, memory::szlen(raw) / (UWORD)sizeof(wchar));
		// ESP: De vuelta a bytes para poder imprimirlo con la ruta de caracteres: wcstombs reserva.
		// stacked en false asigna con memory::alloc, asi que el destino se libera con memory::free.
		// ENG: Back to bytes to print it through the character road: wcstombs reserves.
		// stacked as false allocates with memory::alloc, so the destination gets freed with memory::free.
		char* text = nullptr;
		UWORD n = wcstombs(&text, as_wide, nraw, false);
		printf("como texto (" UWORD_PRINT " bytes): ", n);
		for(UWORD i = 0; i < n; ++i) putchar(text[i]);
		puts("");
		if(text) memory::free(text);
	}

	// ESP: Con nullptr literal la eleccion de sobrecarga cae en la version que rechaza la lectura:
	// responde nullptr siempre y no consume nada de la cola. Para descartar hace falta un puntero tipado.
	// ENG: With a literal nullptr the overload pick lands on the version that refuses the reading:
	// it answers nullptr always and takes nothing from the queue. To discard something a typed pointer is needed.
	UWORD probe = 999;
	void* r = console::read(cnsl, nullptr, &probe);
	printf("read con nullptr literal: ret=%p, probe sigue en " UWORD_PRINT "\n", r, probe);

	// ESP: Los punteros de texto quedan en manos del modulo: nullptr los deja la proxima lectura.
	// ENG: The text pointers stay in the module's hands: the next reading leaves them as nullptr.
	return 0;
}
