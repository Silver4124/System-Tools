/* // ESP
	Ejemplo de carpetas: creacion, recorrido y vaciado.
	Muestra: file::folder, file::inside, file::is_folder, file::size, file::parent,
	file::clear y file::remove sobre una carpeta de prueba.
	Compilar: g++ folder_walk.cpp -o folder_walk
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	La carpeta de prueba se crea y se borra dentro de la carpeta donde se ejecuta.
	// ENG
	Example of folders: creation, walking and emptying.
	Shows: file::folder, file::inside, file::is_folder, file::size, file::parent,
	file::clear and file::remove over a test folder.
	Build: g++ folder_walk.cpp -o folder_walk
	Needs: #define ST_IMPORT_FILE before including the header.
	The test folder gets created and deleted inside the folder where it runs.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DEMO_DIR   "st_folder_walk"
#define DEMO_SUB   DEMO_DIR "/caja"
#define DEMO_ONE   DEMO_DIR "/alpha.txt"
#define DEMO_TWO   DEMO_DIR "/beta.dat"

static void make_file(const char* path, const char* text){
	void* f = file::open(path, file_mode::write | file_mode::create);
	if(!f){ puts("no se pudo crear el fichero de prueba / test file could not be created"); return; }
	UWORD n = 0; while(text[n]) ++n;
	file::write(f, text, n);
	file::close(f);
}

int main(){
	// ESP: Dejar el escenario limpio antes de empezar.
	// ENG: Leave the stage clean before starting.
	if(file::exist(DEMO_DIR)){ file::clear(DEMO_DIR); file::remove(DEMO_DIR); }

	// ESP: folder con el bit create construye el directorio si falta.
	// La carpeta se maneja con el mismo tipo de handle que un fichero.
	// ENG: folder with the create bit builds the directory when it is missing.
	// A folder is handled with the same handle kind a file uses.
	void* dir = file::folder(DEMO_DIR, file_mode::create | file_mode::write);
	if(!dir){ puts("no se pudo crear la carpeta / folder could not be created"); return 1; }
	printf("es carpeta: %d\n", file::is_folder(dir));
	file::close(dir);

	// ESP: folder con create sobre una ruta ya existente no borra lo que adentro este.
	// ENG: folder with create over a path that already exists does not wipe what sits inside.
	file::folder(DEMO_SUB, file_mode::create);
	make_file(DEMO_ONE, "treinta y un bytes de prueba");
	make_file(DEMO_TWO, "corto");

	// ESP: size sobre una carpeta cuenta entradas, no bytes: . y .. quedan afuera.
	// ENG: size over a folder counts entries, not bytes: . and .. stay out.
	dir = file::folder(DEMO_DIR, file_mode::read);
	if(!dir){ puts("no se pudo abrir la carpeta / folder could not be opened"); return 1; }
	UWORD count = file::size(dir);
	printf("entradas: " UWORD_PRINT "\n", count);

	for(UWORD idx = 0; idx < count; ++idx){
		// ESP: inside abre la entrada idx como handle nuevo: hay que cerrarlo uno por uno.
		// El orden lo da el sistema de archivos, no una lista ordenada.
		// ENG: inside opens entry idx as a brand new handle: it must be closed one by one.
		// The order comes from the file system, not from a sorted list.
		void* child = file::inside(dir, idx);
		if(!child) continue;
		char fname[64];
		file::name(child, fname, sizeof(fname) - 1, true);
		if(file::is_folder(child))
			printf("  [" UWORD_PRINT "] carpeta  %s con " UWORD_PRINT " entradas\n",
				idx, fname, file::size(child));
		else
			printf("  [" UWORD_PRINT "] fichero  %s de " UWORD_PRINT " bytes\n",
				idx, fname, file::size(child));

		// ESP: Un indice fuera de rango responde nullptr: el conteo no es una promesa estable.
		// ENG: An index out of range answers nullptr: the count is not a steady promise.
		file::close(child);
	}
	void* beyond = file::inside(dir, count);
	printf("inside(" UWORD_PRINT ") -> %p\n", count, beyond);

	// ESP: parent entrega un handle nuevo de la carpeta madre, no una vista del mismo.
	// La ruta absoluta sale por path con el modelo de longitud textual.
	// ENG: parent hands a brand new handle of the mother folder, not a view of the same one.
	// The absolute path comes out through path with the textual length model.
	void* up = file::parent(dir);
	if(up){
		char* root = nullptr; UWORD root_len = 0;
		file::path(up, &root, &root_len, false);
		printf("carpeta madre (" UWORD_PRINT " caracteres): %s\n", root_len, root ? root : "");
		if(!memory::free(root)) root = nullptr;
		file::close(up);
	}

	// ESP: clear sobre carpeta borra cada entrada y devuelve cuantas habia.
	// La carpeta misma queda en pie y vacia.
	// ENG: clear over a folder deletes every entry and returns how many were there.
	// The folder itself stays standing and empty.
	UWORD removed = file::clear(dir);
	printf("entradas borradas: " UWORD_PRINT ", quedan " UWORD_PRINT "\n", removed, file::size(dir));
	file::close(dir);

	// ESP: clear deja la carpeta vacia y remove la quita: en Linux remove no borra contenido ajeno.
	// En Windows esa misma llamada si recorre y borra lo de adentro, pero no cuentes con ello.
	// ENG: clear leaves the folder empty and remove takes it out: on Linux remove deletes no foreign content.
	// On Windows that very same call does walk and delete what sits inside, but do not count on it.
	if(file::remove(DEMO_DIR)) puts("carpeta borrada / folder deleted");
	printf("existe al final: %d\n", file::exist(DEMO_DIR));

	return 0;
}
