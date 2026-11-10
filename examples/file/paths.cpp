/* // ESP
	Ejemplo del nombre, la extension y la ruta de un fichero, mas las utilidades de ruta.
	Muestra: file::name, file::extension, file::path, file::normalize y file::root_this
	en sus dos familias de firma: la que reserva y la que escribe sobre el buffer propio.
	Compilar: g++ paths.cpp -o paths
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of a file's name, extension and path, plus the path utilities.
	Shows: file::name, file::extension, file::path, file::normalize and file::root_this
	in their two signature families: the reserving one and the own-buffer one.
	Build: g++ paths.cpp -o paths
	Needs: #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: root_this entrega la carpeta del ejecutable en absoluto, con su separador al final.
	// La longitud cuenta ese separador, asi que las rutas se pegan sin anadir nada.
	// No depende del directorio de trabajo: vale igual lanzado desde fuera de su carpeta.
	// ENG: root_this hands the executable's folder in absolute form, with its trailing separator.
	// The length counts that separator, so paths get glued without adding anything.
	// It does not depend on the working directory: it holds even launched from outside its folder.
	char* base = nullptr;
	UWORD base_size = 0;
	file::root_this(&base, &base_size, false);
	printf("carpeta del programa (" UWORD_PRINT "): %s\n", base_size, base);

	char demo_dir[512], demo_file[512];
	sprintf(demo_dir, "%sst_paths_dir", base);
	sprintf(demo_file, "%sst_paths_dir/st_paths_file.txt", base);
	if(file::exist(demo_dir)){ file::clear(demo_dir); file::remove(demo_dir); }
	file::folder(demo_dir, file_mode::create);

	void* f = file::open(demo_file, file_mode::write | file_mode::create);
	if(!f){ puts("no se pudo crear el fichero de prueba / test file could not be created"); return 1; }
	file::apends(f, "ruta");
	file::close(f);

	f = file::open(demo_file, file_mode::read);
	if(!f){ puts("no se pudo reabrir / could not reopen"); file::remove(demo_file); return 1; }

	// ESP: La familia con char** pide el puntero vacío o una direccion de memory::alloc:
	// la funcion realoca por dentro y el tamano sale aparte, sin contar el terminador.
	// ENG: The char** family asks for an empty pointer or a memory::alloc address:
	// the function reallocates from inside and the size comes out apart, terminator uncounted.
	char* heap = nullptr;
	UWORD size = 0;
	file::name(f, &heap, &size, false);
	printf("nombre: %s (" UWORD_PRINT " caracteres)\n", heap, size);
	// ESP: La misma variable sirve para la siguiente llamada: lo anterior se reemplaza.
	// ENG: The very same variable serves the next call: what was there gets replaced.
	file::name(f, &heap, &size, true);
	printf("nombre con extension: %s (" UWORD_PRINT ")\n", heap, size);
	if(!memory::free(heap)) heap = nullptr;

	file::extension(f, &heap, &size, false);
	printf("extension: %s (" UWORD_PRINT ")\n", heap, size);
	file::extension(f, &heap, &size, true);
	printf("extension con punto: %s (" UWORD_PRINT ")\n", heap, size);
	if(!memory::free(heap)) heap = nullptr;

	file::path(f, &heap, &size);
	printf("ruta absoluta (" UWORD_PRINT "):\n  %s\n", size, heap);
	if(!memory::free(heap)) heap = nullptr;

	// ESP: La familia con char[] escribe sobre el buffer propio y recorta al tamano dado.
	// El tamano prometido es el de caracteres utilizables: el terminador va aparte.
	// ENG: The char[] family writes over the own buffer and trims to the given size.
	// The promised size is of usable characters: the terminator goes apart.
	char small[12];
	file::name(f, small, sizeof(small) - 1, true);
	printf("nombre recortado a " UWORD_PRINT ": %s\n", (UWORD)(sizeof(small) - 1), small);
	// ESP: Con tamano 0 no se copia nada y queda solo el terminador.
	// ENG: With size 0 nothing gets copied and only the terminator is left.
	file::name(f, small, 0, true);
	printf("nombre con tamano 0: '%s'\n", small);
	file::extension(f, small, sizeof(small) - 1, true);
	printf("extension en buffer propio: %s\n", small);

	// ESP: normalize reescribe la ruta en el sitio y devuelve el mismo puntero.
	// Traduce el separador ajeno al propio y colapsa los repetidos, sin salir del buffer.
	// "carpeta//fichero.txt" acaba en "carpeta\\fichero.txt": una sola barra, y la del sistema.
	// Solo acorta o iguala la medida, nunca la alarga: el buffer de entrada siempre aguanta.
	// ENG: normalize rewrites the path in place and returns the very same pointer.
	// It turns the foreign separator into the native one and collapses the repeats, without leaving the buffer.
	// "folder//file.txt" ends as "folder\\file.txt": a single bar, and the bar of the system.
	// It only shortens or matches the measure, never grows it: the input buffer always holds.
	char messy[256];
	sprintf(messy, "%sst_paths_dir//st_paths_file.txt", base);
	printf("antes: %s\n", messy);
	UWORD messy_size = 0;
	char* fixed = file::normalize(messy, &messy_size);
	printf("despues (" UWORD_PRINT "): %s\n", messy_size, fixed);

	// ESP: Abriendo lo que devolvió se comprueba que la ruta normalizada sigue siendo util.
	// ENG: Opening what it gave back checks that the normalized path is still usable.
	void* g = file::open(fixed, file_mode::read);
	printf("abrir la ruta normalizada: %s\n", g ? "ok" : "fall");
	if(g) file::close(g);

	// ESP: Con separadores mezclados todos caen del lado propio: la mezcla sale con las barras del sistema.
	// ENG: With separators mixed they all fall on the native side: the mixture comes out in system bars.
	sprintf(messy, "ruta/ejemplo\\sub/mixed.txt");
	fixed = file::normalize(messy, nullptr);
	printf("ruta mixta: %s\n", fixed);

	file::close(f);

	// ESP: Lo que reserva la familia char** es memoria del modulo: se devuelve con memory::free.
	// ENG: What the char** family reserves is module memory: it goes back through memory::free.
	if(!memory::free(base)) base = nullptr;

	file::remove(demo_file);
	file::remove(demo_dir);

	return 0;
}
