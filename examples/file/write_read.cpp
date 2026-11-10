/* // ESP
	Ejemplo de escritura y lectura cruda de un fichero.
	Muestra: file::open, file::write, file::read, file::size, file::tell, file::seek,
	file::opened_like, file::clear, file::close, file::exist y file::remove.
	Compilar: g++ write_read.cpp -o write_read
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	El fichero de prueba se crea y se borra en la carpeta donde se ejecuta.
	// ENG
	Example of raw file writing and reading.
	Shows: file::open, file::write, file::read, file::size, file::tell, file::seek,
	file::opened_like, file::clear, file::close, file::exist and file::remove.
	Build: g++ write_read.cpp -o write_read
	Needs: #define ST_IMPORT_FILE before including the header.
	The test file gets created and deleted in the folder where it runs.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DEMO "st_write_read.tmp"

int main(){
	// ESP: Empezar de cero: si algo quedo de una corrida anterior, quitarlo.
	// ENG: Start clean: if something stayed from a previous run, take it out.
	if(file::exist(DEMO)) file::remove(DEMO);
	printf("existe antes de abrir: %d\n", file::exist(DEMO));

	// ESP: create sin write abre en lectura y el sistema suele rechazar el truncado.
	// La combinacion normal de "crear o reemplazar" es write|create.
	// ENG: create without write opens for reading and the system usually refuses the truncate.
	// The usual "create or replace" combination is write|create.
	void* file = file::open(DEMO, file_mode::write | file_mode::create);
	if(!file){ puts("no se pudo crear / could not create"); return 1; }

	// ESP: opened_like devuelve los bits con los que se abrio, no los que se pidieron.
	// ENG: opened_like returns the bits it was opened with, not the ones asked for.
	printf("abierto como: 0x%02X\n", file::opened_like(file));

	// ESP: write solo cuenta bytes: la longitud textual del texto no entra en la cuenta.
	// El modo escritura es obligatorio o la funcion responde 0 sin tocar el disco.
	// ENG: write only counts bytes: the textual length of the text does not enter the count.
	// Write mode is mandatory or the function answers 0 without touching the disk.
	const char* text = "sistema de archivos";
	UWORD wrote = file::write(file, text, 19);
	printf("bytes escritos: " UWORD_PRINT " (19 esperado)\n", wrote);

	// ESP: tell marca donde sigue la escritura y size dice cuanto ocupa el fichero.
	// ENG: tell marks where writing goes on and size says how much the file takes.
	printf("posicion: " UWORD_PRINT ", tamano: " UWORD_PRINT "\n", file::tell(file), file::size(file));

	// ESP: Este handle no puede leer: el modo read no esta puesto.
	// ENG: This handle cannot read: read mode is not set.
	char probe[4];
	printf("lectura con handle de escritura: " UWORD_PRINT "\n", file::read(file, probe, 4));

	// ESP: close devuelve el handle ya destruido: no nulo afirma que se cerro.
	// La direccion no vuelve a usarse, igual que con memory::free.
	// ENG: close returns the handle already destroyed: non null asserts it got closed.
	// The address is not used again, same as with memory::free.
	if(file::close(file)) puts("cerrado / closed");

	// ESP: Reabrir con read|write deja leer y escribir sobre el mismo fichero.
	// ENG: Reopening with read|write allows reading and writing over the same file.
	file = file::open(DEMO, file_mode::read | file_mode::write);
	if(!file){ puts("no se pudo reabrir / could not reopen"); return 1; }
	printf("existe tras abrir: %d\n", file::exist(DEMO));

	char buffer[32] = {0};
	UWORD read_bytes = file::read(file, buffer, sizeof(buffer) - 1);
	buffer[read_bytes] = '\0';
	printf("leido (" UWORD_PRINT "): %s\n", read_bytes, buffer);

	// ESP: seek con end y longitud negativa cuenta desde el final del fichero.
	// El retorno es la posicion donde quedo el cursor, no un exito o fracaso.
	// ENG: seek with end and a negative length counts from the file end.
	// The return is the position the cursor landed on, not success or failure.
	UWORD here = file::seek(file, -5, seek_mode::end);
	printf("seek(-5, end) -> " UWORD_PRINT "\n", here);
	char tail[6] = {0};
	file::read(file, tail, 5);
	printf("ultimos 5 bytes: %s\n", tail);

	// ESP: seek con begin lleva el cursor a un punto exacto para releer.
	// ENG: seek with begin takes the cursor to an exact spot to read again.
	file::seek(file, 0, seek_mode::begin);
	char head[7] = {0};
	file::read(file, head, 6);
	printf("desde el inicio: %s\n", head);

	// ESP: clear trunca el contenido y devuelve cuanto ocupaba antes de truncarse.
	// ENG: clear truncates the content and returns how much it took before the cut.
	UWORD was = file::clear(file);
	printf("claro ocupaba " UWORD_PRINT ", ahora ocupa " UWORD_PRINT "\n", was, file::size(file));

	file::close(file);

	// ESP: remove por ruta devuelve la misma ruta si borro, nullptr si no existia.
	// ENG: remove by path returns the very same path if it deleted, nullptr if it was missing.
	if(file::remove(DEMO)) puts("fichero borrado / file deleted");
	printf("existe al final: %d\n", file::exist(DEMO));

	return 0;
}
