/* // ESP
	Ejemplo de las marcas de tiempo del modulo de ficheros.
	Muestra: file::timestamp en sus dos firmas, file::to_date, file::to_time y
	file::days_until_2000, con el anclaje de 1970 que compartimos todas las fechas.
	Compilar: g++ timestamps.cpp -o timestamps
	Requiere: #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of the file module time marks.
	Shows: file::timestamp in both signatures, file::to_date, file::to_time and
	file::days_until_2000, with the 1970 anchor all our dates share.
	Build: g++ timestamps.cpp -o timestamps
	Needs: #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: root_this da la carpeta del ejecutable ya con su separador final.
	// ENG: root_this gives the executable's folder already carrying its trailing separator.
	char* base = nullptr;
	UWORD base_size = 0;
	file::root_this(&base, &base_size, false);

	char demo_file[512];
	sprintf(demo_file, "%sst_time_file.txt", base);
	if(file::exist(demo_file)) file::remove(demo_file);

	void* f = file::open(demo_file, file_mode::write | file_mode::create);
	if(!f){ puts("no se pudo crear el fichero / file could not be created"); memory::free(base); return 1; }
	file::apends(f, "fechas");
	file::close(f);

	// ESP: Las tres marcas del sistema son segundos desde el 1 de enero de 1970 (UTC).
	// type fuera de rango no es un fallo: se queda con la de escritura.
	// ENG: The three system marks are seconds since January 1st of 1970 (UTC).
	// A type out of range is not a failure: it falls back to the writed one.
	UWORD creation = file::timestamp(demo_file, time_mode::creation);
	UWORD access   = file::timestamp(demo_file, time_mode::access);
	UWORD writed   = file::timestamp(demo_file, time_mode::writed);
	UWORD odd      = file::timestamp(demo_file, (BYTE)7);
	printf("creacion:    " UWORD_PRINT "\n", creation);
	printf("acceso:      " UWORD_PRINT "\n", access);
	printf("escritura:   " UWORD_PRINT "\n", writed);
	printf("type=7:      " UWORD_PRINT " (igual que escritura)\n", odd);

	// ESP: La firma con ruta abre y cierra por su cuenta: sirve sobre un fichero cerrado.
	// La firma con handle pregunta sobre el vuelo, sin abrir nada otra vez.
	// ENG: The path signature opens and closes on its own: it works over a shut file.
	// The handle signature asks while flying, without opening anything again.
	f = file::open(demo_file, file_mode::read);
	if(f){
		UWORD by_handle = file::timestamp(f, time_mode::creation);
		printf("handle:      " UWORD_PRINT " (igual que la ruta)\n", by_handle);
		file::close(f);
	}

	// ESP: Sobre una ruta inexistente la de ruta responde 0: no hay handle que cerrar.
	// ENG: Over a missing path the path version answers 0: there is no handle left to close.
	printf("ruta sin fichero: " UWORD_PRINT "\n", file::timestamp("sin_ruta_inexistente_9.txt"));

	// ESP: to_date rompe una marca en sus campos: los seis punteros son opcionales.
	// year es unsigned int, el resto son BYTE; la hora sale en UTC, sin zona.
	// ENG: to_date breaks a mark into its fields: the six pointers are optional.
	// year is an unsigned int, the rest are BYTE; the hour comes in UTC, no timezone.
	BYTE sec = 0, min = 0, hour = 0, day = 0, month = 0;
	unsigned int year = 0;
	file::to_date(writed, &sec, &min, &hour, &day, &month, &year);
	printf("escritura leida: %04u-%02u-%02u %02u:%02u:%02u UTC\n",
		year, (unsigned)month, (unsigned)day, (unsigned)hour, (unsigned)min, (unsigned)sec);

	// ESP: Con nullptr en los que sobran solo se rellenan los que se pidieron.
	// ENG: With nullptr on the unneeded ones only the requested get filled.
	BYTE solo_dia = 0;
	file::to_date(writed, nullptr, nullptr, nullptr, &solo_dia, nullptr, nullptr);
	printf("solo el dia: %u\n", (unsigned)solo_dia);

	// ESP: to_time hace el camino contrario: arma la marca a partir de los campos.
	// Aqui se prueba con el anclaje del 2000, un valor que se puede comprobar a mano.
	// ENG: to_time walks the other way: it builds the mark out of the fields.
	// It gets tested here with the year-2000 anchor, a value you can check by hand.
	UWORD y2k = 0;
	file::to_time(&y2k, 0, 0, 0, 1, 1, 2000);
	printf("2000-01-01 00:00:00 -> " UWORD_PRINT " (946684800 esperado)\n", y2k);

	// ESP: La ida y la vuelta cierran el circulo: descifrar y volver a cifrar sale igual.
	// ENG: There and back close the circle: deciphering and ciphering again gives the same.
	UWORD rebuilt = 0;
	file::to_time(&rebuilt, sec, min, hour, day, month, year);
	printf("reconstruida: " UWORD_PRINT " vs original " UWORD_PRINT " -> %s\n",
		rebuilt, writed, rebuilt == writed ? "igual" : "distinta");

	// ESP: days_until_2000 trae segundos desde el anclaje del 2000, no dias pese al nombre.
	// Es siempre el presente: no acepta una marca como argumento.
	// ENG: days_until_2000 brings seconds since the year-2000 anchor, not days despite its name.
	// It is always the present: it takes no mark as an argument.
	UWORD since_2k = file::days_until_2000();
	printf("desde el 2000: " UWORD_PRINT " segundos, " UWORD_PRINT " dias\n",
		since_2k, since_2k / 86400);

	// ESP: La cuenta larga menos el anclaje da lo mismo: las dos escalas son coherentes.
	// ENG: The long count minus the anchor gives the same: both scales stay coherent.
	UWORD writed_since_2k = (writed > y2k) ? (writed - y2k) : 0;
	printf("escritura desde el 2000: " UWORD_PRINT "\n", writed_since_2k);

	if(!memory::free(base)) base = nullptr;
	file::remove(demo_file);
	return 0;
}
