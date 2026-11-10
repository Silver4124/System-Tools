/* // ESP
	Ejemplo de las funciones de registro: escritura terminada y lectura por campos.
	Muestra: file::apends, file::apendc, file::apendws, file::gets, file::getc y file::getws.
	Compilar: g++ append_gets.cpp -o append_gets
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	El fichero de prueba se crea y se borra en la carpeta donde se ejecuta.
	// ENG
	Example of the record functions: terminated writing and field-by-field reading.
	Shows: file::apends, file::apendc, file::apendws, file::gets, file::getc and file::getws.
	Build: g++ append_gets.cpp -o append_gets
	Needs: #define ST_IMPORT_FILE before including the header.
	The test file gets created and deleted in the folder where it runs.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DEMO "st_append_gets.tmp"

int main(){
	if(file::exist(DEMO)) file::remove(DEMO);
	void* file = file::open(DEMO, file_mode::write | file_mode::create);
	if(!file){ puts("no se pudo crear / could not create"); return 1; }

	// ESP: apends escribe el texto y ademas un terminador: el retorno cuenta los dos.
	// ENG: apends writes the text and one terminator on top: the return counts both.
	UWORD a = file::apends(file, "primera linea");
	UWORD b = file::apends(file, "segunda");
	printf("apends: " UWORD_PRINT " + " UWORD_PRINT " -> posicion " UWORD_PRINT "\n",
		a, b, file::tell(file));

	// ESP: apendws hace lo mismo con texto ancho, codificando cada caracter a UTF-8.
	// ENG: apendws does the same with wide text, encoding each character into UTF-8.
	UWORD c = file::apendws(file, L"tercera");
	printf("apendws: " UWORD_PRINT " -> posicion " UWORD_PRINT "\n", c, file::tell(file));

	// ESP: apendc suelta un solo punto de codigo: aqui el euro, tres bytes UTF-8.
	// Con 0 como caracter no escribe nada: el terminador no se suelta con apendc.
	// ENG: apendc releases one code point: here the euro sign, three UTF-8 bytes.
	// With 0 as the character it writes nothing: the terminator does not go out through apendc.
	UWORD d = file::apendc(file, 0x20AC);
	d += file::apendc(file, '\n');
	printf("apendc: " UWORD_PRINT " -> fichero de " UWORD_PRINT " bytes\n", d, file::size(file));

	file::close(file);

	// ESP: gets exige un puntero a puntero: la funcion reserva con new[] y el programador borra con delete[].
	// ENG: gets asks for a pointer to pointer: the function reserves with new[] and the programmer wipes with delete[].
	file = file::open(DEMO, file_mode::read | file_mode::write);
	if(!file){ puts("no se pudo reabrir / could not reopen"); return 1; }

	char* field = nullptr;
	UWORD got = file::gets(file, &field);
	// ESP: El retorno trae el terminador dentro: la longitud textual es una menos.
	// La lectura deja el cursor justo despues del terminador, lista la siguiente llamada.
	// ENG: The return carries the terminator inside: the textual length is one less.
	// The reading leaves the cursor right after the terminator, so the next call is ready.
	printf("gets(" UWORD_PRINT " bytes, " UWORD_PRINT " caracteres): %s\n",
		got, (got ? got - 1 : 0), field ? field : "");
	printf("posicion tras gets: " UWORD_PRINT "\n", file::tell(file));

	got = file::gets(file, &field);
	printf("gets(" UWORD_PRINT " bytes, " UWORD_PRINT " caracteres): %s\n",
		got, (got ? got - 1 : 0), field ? field : "");

	// ESP: getws gasta el terminador igual que gets, pero su retorno son bytes de la ruta UTF-8.
	// Con caracteres de mas de un byte los dos numeros dejan de coincidir.
	// ENG: getws spends the terminator just like gets, but its return is bytes of the UTF-8 track.
	// With characters over one byte the two figures stop matching.
	wchar* wfield = nullptr;
	UWORD wbytes = file::getws(file, &wfield);
	UWORD wlen = 0; while(wfield[wlen]) ++wlen;
	// ESP: printf no sabe de wchar: para mostrarlo hay que volver a bytes con wcstombs.
	// stacked en false reserva con memory::alloc, asi que se libera con memory::free.
	// ENG: printf knows nothing of wchar: showing it means going back to bytes through wcstombs.
	// stacked as false reserves with memory::alloc, so it gets released with memory::free.
	char* wide_as_text = nullptr;
	UWORD wide_bytes = wcstombs(&wide_as_text, wfield, wlen, false);
	printf("getws: " UWORD_PRINT " bytes leidos, " UWORD_PRINT " unidades, " UWORD_PRINT " bytes vuelta: %s\n",
		wbytes, wlen, wide_bytes, wide_as_text ? wide_as_text : "");
	printf("posicion tras getws: " UWORD_PRINT "\n", file::tell(file));
	if(!memory::free(wide_as_text)) puts("texto liberado / text released");

	// ESP: getc decodifica un punto de codigo UTF-8 y devuelve cuantos bytes consumio.
	// ENG: getc decodes one UTF-8 code point and returns how many bytes it spent.
	int chr = 0;
	UWORD spent = file::getc(file, &chr);
	printf("getc: U+%04X en " UWORD_PRINT " bytes\n", chr, spent);
	spent = file::getc(file, &chr);
	printf("getc: U+%04X en " UWORD_PRINT " bytes\n", chr, spent);

	// ESP: Al final del fichero getc responde 0 y no toca el destino.
	// ENG: At the file end getc answers 0 and leaves the destination alone.
	spent = file::getc(file, &chr);
	printf("getc sobre el fin: " UWORD_PRINT "\n", spent);

	// ESP: La memoria de gets y getws es del programador: el modulo no la recupera al cerrar.
	// ENG: The memory of gets and getws belongs to the programmer: the module does not reclaim it on close.
	delete[] field;
	delete[] wfield;
	file::close(file);
	file::remove(DEMO);

	return 0;
}
