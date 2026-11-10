/* // ESP
	Ejemplo de las operaciones que trabajan solo con rutas de texto.
	Muestra: file::exist, file::copy, file::move, file::clear y file::remove
	sobre una carpeta de prueba y un par de ficheros sueltos, todos autogenerados.
	Compilar: g++ copy_move.cpp -o copy_move
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of the operations that work over text paths only.
	Shows: file::exist, file::copy, file::move, file::clear and file::remove
	over a test folder and a couple of loose files, all of them self-generated.
	Build: g++ copy_move.cpp -o copy_move
	Needs: #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define SRC_DIR   "st_cm_src"
#define SRC_A     SRC_DIR "/a.txt"
#define SRC_B     SRC_DIR "/b.txt"
#define CP_DIR    "st_cm_copy"
#define CP_B      CP_DIR "/b.txt"
#define MV_DIR    "st_cm_moved"
#define ONE       "st_cm_one.txt"
#define TWO       "st_cm_two.txt"
#define LONE      "st_cm_lone.txt"
#define LONE_MV   "st_cm_lone_renamed.txt"

// ESP: Helper propio: deja un fichero con el texto indicado y lo cierra.
// ENG: Own helper: leaves a file holding the given text and closes it.
static void put_text(const char* path, const char* text){
	void* f = file::open(path, file_mode::write | file_mode::create);
	if(!f){ printf("no se pudo escribir %s / writing failed for %s\n", path, path); return; }
	UWORD n = 0; while(text[n]) ++n;
	file::write(f, text, n);
	file::close(f);
}

// ESP: Helper propio: dice cuantos elementos hay dentro de una carpeta o cuántos bytes tiene un fichero.
// size pregunta lo mismo sobre dos clases de handle distintas.
// ENG: Own helper: counts the elements inside a folder, or the bytes of a file.
// size asks the same question over two different kinds of handle.
static UWORD content_of(const char* path){
	void* h = file::open(path, file_mode::read);
	if(!h) h = file::folder(path, file_mode::read);
	if(!h) return 0;
	UWORD n = file::size(h);
	file::close(h);
	return n;
}

// ESP: Helper propio: lee los primeros bytes de un fichero y los muestra.
// ENG: Own helper: reads a file's first bytes and shows them.
static void show_text(const char* path, UWORD bytes){
	void* f = file::open(path, file_mode::read);
	if(!f){ printf("%s: inlegible / unreadable\n", path); return; }
	char buff[64];
	if(bytes > sizeof(buff) - 1) bytes = sizeof(buff) - 1;
	UWORD got = file::read(f, buff, bytes);
	file::close(f);
	buff[got] = '\0';
	printf("%s: [%s]\n", path, buff);
}

int main(){
	// ESP: Escenario limpio: el ejemplo es repetible aunque algo sobrara de otra pasada.
	// ENG: Clean stage: the example stays repeatable even if something survived another run.
	if(file::exist(CP_DIR)){ file::clear(CP_DIR); file::remove(CP_DIR); }
	if(file::exist(MV_DIR)){ file::clear(MV_DIR); file::remove(MV_DIR); }
	if(file::exist(SRC_DIR)){ file::clear(SRC_DIR); file::remove(SRC_DIR); }
	file::remove(ONE); file::remove(TWO); file::remove(LONE); file::remove(LONE_MV);

	// ESP: Material: una carpeta con dos ficheros y tres ficheros sueltos.
	// ENG: Material: one folder holding two files plus three loose files.
	void* d = file::folder(SRC_DIR, file_mode::create | file_mode::write);
	if(!d){ puts("no se pudo crear la carpeta / folder could not be created"); return 1; }
	file::close(d);
	put_text(SRC_A, "contenido de a");
	put_text(SRC_B, "contenido de b");
	put_text(ONE, "suelto uno");
	put_text(TWO, "segundo suelto");
	put_text(LONE, "para renombrar");

	// ESP: exist vale igual para ficheros y para carpetas: no distingue las dos clases.
	// Con nullptr o una ruta inexistente responde false, nunca falla.
	// ENG: exist is equally good for files and folders: it does not tell the two kinds apart.
	// With nullptr or a missing path it answers false, it never fails.
	printf("exist fichero:   %d\n", (int)file::exist(ONE));
	printf("exist carpeta:   %d\n", (int)file::exist(SRC_DIR));
	printf("exist inexistente: %d\n", (int)file::exist("st_cm_nada.txt"));
	printf("exist nullptr:   %d\n", (int)file::exist(nullptr));

	// ESP: copy sobre una carpeta es recursivo: crea la destino y pasa el contenido entrada a entrada.
	// El retorno es la ruta destino cuando sale bien, nullptr cuando algo del camino se rompe.
	// ENG: copy over a folder is recursive: it makes the destination and carries each entry inside.
	// The return is the destination path when it works, nullptr when the road breaks somewhere.
	const char* copied = file::copy(SRC_DIR, CP_DIR);
	printf("carpeta copiada a: %s\n", copied ? copied : "nullptr");
	printf("entradas copiadas: " UWORD_PRINT "\n", content_of(CP_DIR));

	// ESP: Un destino ya ocupado se respeta con overwrite en false: la llamada se niega.
	// ENG: An already taken destination gets respected with overwrite as false: the call refuses.
	const char* refused = file::copy(ONE, CP_B, false);
	printf("copiar sin pisar: %s\n", refused ? "copio" : "rechazado (nullptr)");
	show_text(CP_B, 20);

	// ESP: Con overwrite en true el contenido viejo se pierde y el retorno vuelve a ser la ruta destino.
	// ENG: With overwrite as true the old content is lost and the return is the destination path again.
	const char* forced = file::copy(ONE, CP_B, true);
	printf("copiar pisando: %s\n", forced ? "copio" : "rechazado (nullptr)");
	show_text(CP_B, 20);

	// ESP: Con un origen que no existe no hay intento: nullptr inmediato.
	// copy tampoco crea carpetas intermedias: el destino debe poder abrirse.
	// ENG: With a source that is not there there is no attempt: nullptr right away.
	// copy makes no middle folders either: the destination must be openable.
	printf("origen ausente: %s\n", file::copy("st_cm_nada.txt", TWO) ? "copio" : "nullptr");
	printf("sin carpeta madre: %s\n", file::copy(ONE, "st_cm_sin_carpeta/x.txt") ? "copio" : "nullptr");

	// ESP: move cambia el nombre, la ruta, o las dos cosas a la vez: el origen deja de existir.
	// Lleva el mismo overwrite que copy; aqui basta con el valor por defecto.
	// ENG: move changes the name, the route, or both at once: the source stops existing.
	// It carries the same overwrite copy has; the default value is enough here.
	const char* renamed = file::move(LONE, LONE_MV);
	printf("renombrado a: %s\n", renamed ? renamed : "nullptr");
	printf("origen %d, nuevo %d\n", (int)file::exist(LONE), (int)file::exist(LONE_MV));

	// ESP: move también traslada una carpeta entera de sitio, con su contenido dentro.
	// ENG: move shifts a whole folder to another place too, with its content inside.
	const char* shifted = file::move(CP_DIR, MV_DIR);
	printf("carpeta movida: %s, vieja %d, nueva %d\n",
		shifted ? "si" : "no", (int)file::exist(CP_DIR), (int)file::exist(MV_DIR));

	// ESP: clear responde lo que había antes de vaciar: bytes en un fichero, entradas en una carpeta.
	// La ruta sigue existiendo despues: clear quita el contenido, no el continente.
	// ENG: clear answers what was there before emptying: bytes for a file, entries for a folder.
	// The path still exists afterwards: clear takes the content away, not the container.
	UWORD bytes_out = file::clear(TWO);
	UWORD kids_out = file::clear(MV_DIR);
	printf("clear fichero: " UWORD_PRINT " bytes, quedan " UWORD_PRINT ", existe %d\n",
		bytes_out, content_of(TWO), (int)file::exist(TWO));
	printf("clear carpeta: " UWORD_PRINT " entradas, quedan " UWORD_PRINT ", existe %d\n",
		kids_out, content_of(MV_DIR), (int)file::exist(MV_DIR));

	// ESP: remove quita el continente: responde la ruta solo cuando lo logró.
	// Sobre una carpeta con contenido la conducta cambia de plataforma:
	// en Windows recorre y borra lo de adentro; en Linux el rmdir falla si algo queda.
	// Por eso se vacia antes con clear en todas partes.
	// ENG: remove takes the container away: it answers the path only when it managed.
	// Over a folder with content the behavior changes by platform:
	// Windows walks and deletes what sits inside; Linux rmdir fails if anything is left.
	// That is why it gets emptied first with clear everywhere.
	const char* file_gone = file::remove(TWO);
	printf("remove fichero: %s, existe %d\n",
		file_gone ? file_gone : "nullptr", (int)file::exist(TWO));
	// ESP: Repetir el remove sobre la ruta ya borrada responde nullptr: no es un fallo mudo.
	// ENG: Repeating remove over the already deleted path answers nullptr: it is not a silent failure.
	printf("remove repetido: %s\n", file::remove(TWO) ? "quitado" : "nullptr");
	printf("remove carpeta vacia: %s\n", file::remove(MV_DIR) ? "quitada" : "nullptr");
	printf("remove invento: %s\n", file::remove("st_cm_nada.txt") ? "quitado" : "nullptr");

	// ESP: Despedida: el ejemplo se lleva lo que trajo.
	// Queda SRC_DIR: con sus dos ficheros borrados, la carpeta sigue en pie hasta remove.
	// ENG: Farewell: the example takes away what it brought.
	// SRC_DIR is left: with both files deleted, the folder stands until remove.
	file::remove(ONE);
	file::remove(LONE_MV);
	file::remove(SRC_A);
	file::remove(SRC_B);
	printf("carpeta origen sigue: %d\n", (int)file::exist(SRC_DIR));
	file::remove(SRC_DIR);
	printf("carpeta origen al final: %d\n", (int)file::exist(SRC_DIR));
	return 0;
}
