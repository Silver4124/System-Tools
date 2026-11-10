/* // ESP
	Ejemplo del borrado por handle: la version de file::remove que no esta declarada
	en el encabezado pero si definida en el modulo.
	Muestra: file::remove(void*) sobre un fichero abierto y sobre una carpeta, el
	consumo del handle, file::inside como fuente de handles y el vaciado previo de
	la carpeta antes de borrarla.
	Compilar: g++ remove_handle.cpp -o remove_handle
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	El encabezado solo declara remove(const char*); la firma por handle vive en la
	definicion y, al ser libreria de cabecera, el programa la ve y la enlaza.
	Lo creado se genera y se borra en la carpeta donde se ejecuta.
	// ENG
	Example of deleting through the handle: the file::remove version that is not
	declared in the header but is defined in the module.
	Shows: file::remove(void*) over an open file and over a folder, the handle being
	consumed, file::inside as a source of handles, and the emptying done before
	deleting the folder itself.
	Build: g++ remove_handle.cpp -o remove_handle
	Needs: #define ST_IMPORT_FILE before including the header.
	The header declares only remove(const char*); the handle signature lives in the
	definition and, being a header library, the program sees it and links it.
	What gets created is generated and deleted in the folder where it runs.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DIR_A  "st_remove_dir"
#define DIR_B  "st_remove_dir/inner"
#define FILE_A "st_remove_dir/one.txt"
#define FILE_B "st_remove_dir/two.txt"

int main(){
	// ESP: Empezar de cero: el borrado por ruta de una carpeta es recursivo y limpia la corrida previa.
	// ENG: Start clean: the path-based delete of a folder is recursive and wipes the previous run.
	if(file::exist(DIR_A)) file::remove(DIR_A);

	// ESP: folder con create arma la carpeta y entrega su handle; la segunda carpeta anidada igual.
	// ENG: folder with create builds the directory and hands its handle over; the nested second one the same.
	void* dir = file::folder(DIR_A, file_mode::create | file_mode::write);
	if(!dir){ puts("sin carpeta / no folder"); return 1; }
	if(!file::folder(DIR_B, file_mode::create | file_mode::write)) puts("fallo la subcarpeta / subfolder failed");

	// ESP: Dos ficheros dentro: el handle queda abierto en write|create y aqui no se cierra.
	// El borrado por handle se queda el cierre, asi que cerrarlo antes seria trabajo duplicado.
	// ENG: Two files inside: the handle stays open on write|create and does not get closed here.
	// The handle delete takes the closing over, so closing beforehand would be duplicated work.
	void* one = file::open(FILE_A, file_mode::write | file_mode::create);
	void* two = file::open(FILE_B, file_mode::write | file_mode::create);
	if(!one || !two){ puts("sin ficheros / no files"); return 1; }
	file::write(one, "uno", 3);
	file::write(two, "dos", 3);
	printf("creados: %d y %d\n", file::exist(FILE_A), file::exist(FILE_B));

	// ESP: remove(void*) borra desde la ruta interna del handle: nada de reconstruir el camino con path.
	// Retorno: la misma direccion entregada cuando el disco acepto el borrado.
	// ENG: remove(void*) deletes from the handle's internal path: no rebuilding the road through path.
	// Return: the very same address delivered once the disk accepted the deletion.
	void* gone = file::remove(one);
	// ESP: El print va aparte: dentro de printf el orden de argumentos no esta fijado, y consultarlo
	// todo junto dejaria el "existe" tomado antes del borrado.
	// ENG: The print goes apart: inside printf the argument order is not fixed, and asking it all
	// together would take the "exists" reading before the deletion.
	printf("remove(one) -> %p, igual: %d\n", gone, (int)(gone == one));
	printf("existe tras remove(one): %d\n", file::exist(FILE_A));

	// ESP: El handle queda consumido: la propia remove libera esa memoria, asi que ninguna consulta vuelve a el.
	// Ni size, ni opened_like, ni close: preguntar por un handle muerto es leer memoria ya devuelta al monton.
	// Lo unico comprobable despues del borrado vive en el disco, y el hermano del lado sigue intacto.
	// ENG: The handle is consumed: remove itself frees that memory, so no query goes back to it.
	// Neither size, nor opened_like, nor close: asking a dead handle is reading memory already returned to the heap.
	// What is left to check after the deletion lives on the disk, and the sibling on the side stays intact.
	printf("el otro fichero sigue en el disco: %d\n", file::exist(FILE_B));

	// ESP: Antes de borrar el segundo desde su hijo, cerrar el handle con el que se cree:
	// mientras otra handle propia tenga el fichero abierto, Windows rechaza el borrado.
	// ENG: Before deleting the second one from its child, close the handle it was created with:
	// while another one's own handle keeps the file open, Windows refuses the deletion.
	file::close(two);

	// ESP: El segundo fichero se borra desde el handle que entrega inside: cada indice es un hijo abierto.
	// El conteo lo da size sobre la carpeta: cuenta entradas, no bytes, y deja fuera . y ..
	// ENG: The second file gets deleted from the handle inside delivers: each index is an opened child.
	// The count comes from size over the folder: it counts entries, not bytes, and leaves . and .. out.
	void* list = file::folder(DIR_A, file_mode::read);
	if(list){
		UWORD count = file::size(list);
		printf("entradas de la carpeta: " UWORD_PRINT "\n", count);
		for(UWORD i = 0; i < count; ++i){
			void* child = file::inside(list, i);
			if(!child) continue;
			if(file::is_folder(child)){
				// ESP: Un handle de hijo compartido no es el dueno de la ruta: aqui se cierra sin borrar.
				// ENG: A shared child handle is not the owner of the path: it gets closed here without deleting.
				file::close(child);
				continue;
			}
			char fname[64];
			file::name(child, fname, sizeof(fname) - 1, true);
			void* deleted = file::remove(child);
			printf("borrado [%s] desde inside: %p\n", fname, deleted);
		}
		file::close(list);
	}

	// ESP: Con el interior limpio, la carpeta tambien se borra por su propio handle.
	// Una carpeta con algo dentro se rechaza: el vaciado previo es condicion.
	// Y el "existe" se pregunta aparte, para que no se tome antes del borrado.
	// ENG: With the inside clean, the folder gets deleted through its own handle too.
	// A folder holding something gets refused: emptying it beforehand is the condition.
	// And the "exists" question goes apart, so it is not taken before the deletion.
	printf("remove(dir) -> %p\n", file::remove(dir));
	printf("existe tras remove(dir): %d\n", file::exist(DIR_A));
	printf("nada queda: %d %d %d\n", file::exist(DIR_A), file::exist(DIR_B), file::exist(FILE_B));

	// ESP: Del handle de la carpeta ya no queda nada que consultar: el segundo remove seria leer memoria muerta.
	// La doble liberacion se rechaza por el camino largo: la ruta ya borrada responde nullptr sin tocar nada.
	// ENG: Of the folder handle nothing is left to query: a second remove would be reading dead memory.
	// The double release gets refused through the long road: the already deleted path answers nullptr without touching anything.
	printf("remove por ruta repetido: %p\n", file::remove(DIR_A));
	return 0;
}
