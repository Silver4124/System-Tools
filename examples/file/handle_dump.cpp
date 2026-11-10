/* // ESP
	Ejemplo del volcado de handles a disco y su recuperación.
	Muestra: file::apendo y file::geto con handles de fichero, y lo que pasa
	cuando el objeto no es volcable o cuando el registro ya no cabe en el fichero.
	Compilar: g++ handle_dump.cpp -o handle_dump
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of dumping handles to disk and getting them back.
	Shows: file::apendo and file::geto over file handles, and what happens
	when the object is not dumpable or when the record no longer fits the file.
	Build: g++ handle_dump.cpp -o handle_dump
	Needs: #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DUMP_FILE "st_hd_dump.bin"
#define NOTE_A    "st_hd_note_a.txt"
#define NOTE_B    "st_hd_note_b.txt"
#define BOX       "st_hd_box"
#define BOX_FILE  BOX "/st_hd_inside.txt"

// ESP: Helper propio: deja un fichero de texto con el contenido indicado.
// ENG: Own helper: leaves a text file holding the given content.
static void put_text(const char* path, const char* text){
	void* f = file::open(path, file_mode::write | file_mode::create);
	if(!f){ printf("no se pudo escribir %s / writing failed for %s\n", path, path); return; }
	UWORD n = 0; while(text[n]) ++n;
	file::write(f, text, n);
	file::close(f);
}

// ESP: Helper propio: dice cuantos caracteres tiene una cadena terminada en nulo.
// ENG: Own helper: counts the characters of a null-terminated string.
static UWORD slen(const char* s){ UWORD n = 0; if(s) while(s[n]) ++n; return n; }

int main(){
	if(file::exist(DUMP_FILE)) file::remove(DUMP_FILE);
	if(file::exist(BOX)){ file::clear(BOX); file::remove(BOX); }
	file::remove(NOTE_A); file::remove(NOTE_B);

	// ESP: Material de prueba: dos ficheros, una carpeta y un fichero dentro de ella.
	// ENG: Test material: two files, one folder, and a file inside that folder.
	put_text(NOTE_A, "primera nota volcada");
	put_text(NOTE_B, "segunda nota volcada con mas bytes");
	void* box = file::folder(BOX, file_mode::create | file_mode::write);
	if(box){ file::close(box); }
	put_text(BOX_FILE, "dentro de la caja");

	// ESP: Los tres handles quedan abiertos: apendo lee su modo y su ruta absoluta del handle vivo.
	// ENG: The three handles stay open: apendo reads their mode and absolute path from the live handle.
	void* a = file::open(NOTE_A, file_mode::read);
	void* b = file::open(NOTE_B, file_mode::read | file_mode::write);
	void* c = file::open(BOX_FILE, file_mode::read);
	void* k = file::folder(BOX, file_mode::read);
	if(!a || !b || !c){
		puts("faltan handles de prueba / test handles are missing");
		if(a) file::close(a);
		if(b) file::close(b);
		if(c) file::close(c);
		if(k) file::close(k);
		return 1;
	}

	// ESP: El volcado se abre con lectura y escritura: los permisos de escritura no alcanzan para leer la cabecera.
	// ENG: The dump gets opened with read and write: write permissions alone do not reach to read the header back.
	void* dump = file::open(DUMP_FILE, file_mode::read | file_mode::write | file_mode::create);
	if(!dump){ puts("no se pudo abrir el volcado / dump could not be opened"); return 1; }

	// ESP: apendo escribe la firma del objeto, sus campos y devuelve los bytes metidos.
	// Con un handle de fichero la firma son los caracteres FILE mas el tamano de palabra
	// y el orden de bytes, para que el registro se pueda leer en otra maquina.
	// ENG: apendo writes the object signature, its fields, and returns the bytes put inside.
	// For a file handle the signature is the characters FILE plus the word size
	// and the byte order, so the record can be read on another machine.
	UWORD wrote_a = file::apendo(dump, a);
	UWORD wrote_b = file::apendo(dump, b);
	UWORD wrote_c = file::apendo(dump, c);
	UWORD wrote_k = file::apendo(dump, k);
	printf("registro a: " UWORD_PRINT " bytes\n", wrote_a);
	printf("registro b: " UWORD_PRINT " bytes\n", wrote_b);
	printf("registro c: " UWORD_PRINT " bytes\n", wrote_c);
	printf("registro caja: " UWORD_PRINT " bytes\n", wrote_k);

	// ESP: Un puntero nulo no es un objeto volcable: la llamada responde 0 y no escribe nada.
	// Un puntero sin firma reconocida corre la misma suerte: 0 bytes, fichero intacto.
	// ENG: A null pointer is not a dumpable object: the call answers 0 and writes nothing.
	// A pointer without a recognized signature meets the same fate: 0 bytes, file untouched.
	UWORD wrote_null = file::apendo(dump, nullptr);
	UWORD stranger = 0x11223344;
	UWORD wrote_stranger = file::apendo(dump, &stranger);
	UWORD dump_size = file::size(dump);
	printf("apendo(nullptr): " UWORD_PRINT ", apendo(sin firma): " UWORD_PRINT "\n",
		wrote_null, wrote_stranger);
	printf("tamano del volcado: " UWORD_PRINT " (= " UWORD_PRINT ")\n",
		dump_size, wrote_a + wrote_b + wrote_c + wrote_k);

	// ESP: La cabecera del primer registro se puede leer a mano: seis caracteres ASCII.
	// El byte de tamano es '@' en palabra de 64 y ' ' en 32; el de orden, '>' o '<'.
	// ENG: The first record's header can be read by hand: six ASCII characters.
	// The size byte is '@' on a 64-bit word and ' ' on 32; the order one is '>' or '<'.
	char header[7] = {0};
	file::seek(dump, 0, seek_mode::begin);
	file::read(dump, header, 6);
	printf("cabecera: [%s] tamano='%c' orden='%c'\n", header, header[4], header[5]);

	file::close(dump);
	// ESP: Los handles originales siguen vivos hasta cerrarlos aqui: geto trabaja sobre registros, no sobre ellos.
	// ENG: The original handles stay alive until they get closed here: geto works on records, not on them.
	file::close(a);
	file::close(b);
	file::close(c);
	file::close(k);

	// ESP: geto abre el siguiente registro y entrega cuantos bytes consumio.
	// El puntero de salida es un void** que la funcion rellena con el handle nuevo.
	// ENG: geto opens the next record and delivers how many bytes it consumed.
	// The output pointer is a void** that the function fills with the brand new handle.
	dump = file::open(DUMP_FILE, file_mode::read);
	if(!dump){ puts("no se pudo releer el volcado / dump could not be reread"); return 1; }

	void* back = nullptr;
	UWORD read_a = file::geto(dump, &back);
	printf("\nlectura 1: " UWORD_PRINT " bytes, handle %p\n", read_a, back);
	if(back){
		// ESP: El handle recuperado es un handle normal: admite size, read, path y opened_like.
		// ENG: The recovered handle is an ordinary handle: it takes size, read, path and opened_like.
		printf("  modo recuperado: 0x%02X (leera %d)\n",
			(unsigned)file::opened_like(back), (int)(file::opened_like(back) & file_mode::read));
		char* route = nullptr; UWORD route_size = 0;
		file::path(back, &route, &route_size, false);
		printf("  ruta guardada (" UWORD_PRINT "): %s\n", route_size, route ? route : "");
		if(!memory::free(route)) route = nullptr;
		char text[64];
		UWORD got = file::read(back, text, slen("primera nota volcada"));
		text[got] = '\0';
		printf("  contenido: [%s]\n", text);
	}

	// ESP: La segunda lectura usa el mismo void**: geto cierra el handle que habia antes.
	// Por eso la direccion de la lectura uno ya no es util: no la vuelvas a tocar.
	// ENG: The second reading uses the very same void**: geto closes the handle that was there before.
	// That is why reading one's address is no longer usable: do not touch it again.
	UWORD read_b = file::geto(dump, &back);
	printf("lectura 2: " UWORD_PRINT " bytes, handle %p\n", read_b, back);
	if(back){
		printf("  modo recuperado: 0x%02X (escribira %d)\n",
			(unsigned)file::opened_like(back), (int)(file::opened_like(back) & file_mode::write));
		UWORD capacidad = file::size(back);
		printf("  tamano recuperado: " UWORD_PRINT "\n", capacidad);
	}

	// ESP: El tercer registro guarda una ruta dentro de una carpeta: la recupera igual.
	// ENG: The third record holds a route inside a folder: it gets recovered the same way.
	UWORD read_c = file::geto(dump, &back);
	printf("lectura 3: " UWORD_PRINT " bytes, handle %p\n", read_c, back);
	if(back){
		char text[64];
		UWORD got = file::read(back, text, slen("dentro de la caja"));
		text[got] = '\0';
		printf("  contenido: [%s]\n", text);
	}

	// ESP: El cuarto registro salio de una carpeta: la reconstruccion usa file::open, no file::folder.
	// El registro no guarda ese dato, asi que en Windows la apertura sale rechazada y el puntero queda nulo.
	// ENG: The fourth record came from a folder: the rebuild goes through file::open, not file::folder.
	// The record keeps no note of that, so on Windows the opening gets refused and the pointer stays null.
	UWORD read_k = file::geto(dump, &back);
	printf("lectura 4 (caja): " UWORD_PRINT " bytes, handle %p\n", read_k, back);
	// ESP: Si se cierra a mano el handle recuperado, hay que apagar la variable despues:
	// geto lee la firma de lo que la variable apunte, y una direccion ya liberada es memoria muerta.
	// ENG: Closing the recovered handle by hand means the variable gets switched off right after:
	// geto reads the signature of whatever the variable points to, and a freed address is dead memory.
	if(back){ file::close(back); back = nullptr; }

	// ESP: Sin registros pendientes geto responde 0 y deja el puntero en nullptr.
	// Antes de leer, si el puntero traia un handle vivo, geto lo cierra: la direccion vieja se apaga.
	// ENG: With no record left geto answers 0 and leaves the pointer at nullptr.
	// Before reading, if the pointer carried a live handle, geto closes it: the old address goes dark.
	UWORD read_none = file::geto(dump, &back);
	printf("lectura sin resto: " UWORD_PRINT ", puntero %p\n", read_none, back);
	(void)read_none;

	file::close(dump);

	// ESP: Recogida: el volcado y el material de prueba se van.
	// ENG: Cleanup: the dump and the test material leave.
	file::remove(DUMP_FILE);
	file::remove(NOTE_A);
	file::remove(NOTE_B);
	file::remove(BOX_FILE);
	file::remove(BOX);
	return 0;
}
