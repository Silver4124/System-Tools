/* // ESP
	Ejemplo del handle hilo con el fichero en manos del programa: firma base, bucle y maestro.
	Muestra: sound::file_stream por handle, sound::set_volume, sound::master_volume, sound::set_pitch,
	sound::get_spectrum, sound::playing y sound::destroy, junto a file::open y file::close.
	Compilar: g++ threaded_file.cpp -o threaded_file
	Requiere: #define ST_IMPORT_SOUND y #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of the threaded handle with the file kept by the program: base signature, loop and master.
	Shows: sound::file_stream by handle, sound::set_volume, sound::master_volume, sound::set_pitch,
	sound::get_spectrum, sound::playing and sound::destroy, next to file::open and file::close.
	Build: g++ threaded_file.cpp -o threaded_file
	Needs: #define ST_IMPORT_SOUND and #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_SOUND
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define WAV_PATH   "st_threaded_file.wav"
#define WAV_RATE   8000u
#define WAV_FRAMES 3200u
#define DATA_SIZE  (WAV_FRAMES * 2u)
#define WAV_SIZE   (44u + DATA_SIZE)

// ESP: Helper propio: suelta dos bytes al revés de peso, que es como el WAV apunta sus enteros.
// ENG: Own helper: drops the two bytes reversed by weight, which is how WAV points its integers.
static void put16(BYTE* at, unsigned int value){
	at[0] = (BYTE)(value & 0xFF);
	at[1] = (BYTE)((value >> 8) & 0xFF);
}

// ESP: Helper propio: igual con cuatro bytes.
// ENG: Own helper: the same with four bytes.
static void put32(BYTE* at, unsigned long value){
	at[0] = (BYTE)(value & 0xFF);
	at[1] = (BYTE)((value >> 8) & 0xFF);
	at[2] = (BYTE)((value >> 16) & 0xFF);
	at[3] = (BYTE)((value >> 24) & 0xFF);
}

// ESP: Helper propio: copia cuatro letras seguidas dentro del bufer.
// ENG: Own helper: copies four letters in a row into the buffer.
static void put_tag(BYTE* at, const char* tag){
	for(UWORD i = 0; i < 4; ++i) at[i] = (BYTE)tag[i];
}

// ESP: Oscilador de dos muestras atras sobre 220 Hz y 8000 de muestreo: 36 tramas por ciclo.
// ENG: Two-samples-behind oscillator over 220 Hz and 8000 of sampling: 36 frames per cycle.
static float next_sample(){
	static float behind = 0.1719292f;
	static float further = 0.0f;
	float now = 1.9702202f * behind - further;
	further = behind;
	behind = now;
	return now;
}

// ESP: Helper propio: arma los 44 bytes de cabecera y despues las muestras, todo en un bloque.
// El modulo pide el fichero abierto solo en lectura: si trae escritura la firma rechaza el handle.
// ENG: Own helper: lays the 44 header bytes and then the samples, all inside one block.
// The module asks the file opened in reading only: if it carries writing the signature refuses the handle.
static BYTE* build_wav(){
	BYTE* wav = (BYTE*)memory::alloc(WAV_SIZE);
	if(!wav) return nullptr;
	memory::slloc(wav, 0, WAV_SIZE);
	put_tag(wav, "RIFF");
	put32(wav + 4, (unsigned long)(WAV_SIZE - 8));
	put_tag(wav + 8, "WAVE");
	put_tag(wav + 12, "fmt ");
	put32(wav + 16, 16);
	put16(wav + 20, 1);
	put16(wav + 22, 1);
	put32(wav + 24, WAV_RATE);
	put32(wav + 28, WAV_RATE * 2);
	put16(wav + 32, 2);
	put16(wav + 34, 16);
	put_tag(wav + 36, "data");
	put32(wav + 40, DATA_SIZE);
	for(UWORD i = 0; i < WAV_FRAMES; ++i)
		put16(wav + 44 + (i * 2), (unsigned int)(unsigned short)(short)(next_sample() * 0.4f * 32767.0f));
	return wav;
}

int main(){
	BYTE* wav = build_wav();
	if(!wav){ puts("sin WAV / no WAV"); return 1; }
	void* maker = file::open(WAV_PATH, file_mode::write | file_mode::create);
	if(!maker){ puts("no se pudo escribir el WAV / the WAV could not be written"); memory::free(wav); return 1; }
	file::write(maker, wav, WAV_SIZE);
	file::close(maker);
	memory::free(wav);

	// ESP: La firma base pide el Handle Fichero abierto en lectura: aqui el programa lo abre y lo cierra.
	// Looping en true: el hilo rebobina el archivo cada vez que lo agota, asi que nunca se despide solo.
	// CanIcloser en false: el modulo no toca el fichero al terminar y la cierre le toca al programa.
	// ENG: The base signature asks the Handle File opened in reading: here the program opens it and shuts it.
	// Looping at true: the thread rewinds the file every time it drains it, so it never says goodbye alone.
	// CanIcloser at false: the module never touches the file on the end and the closing falls to the program.
	void* file = file::open(WAV_PATH, file_mode::read);
	if(!file){ puts("sin fichero / no file"); return 1; }
	void* th = sound::file_stream(file, true, 1.0f, false);
	if(!th){ puts("sin sonido hilo / no threaded sound"); file::close(file); file::remove(WAV_PATH); return 1; }

	// ESP: El fichero sigue abierto y consultable: el programa ve cuanto mide por fuera del hilo.
	// ENG: The file stays open and queryable: the program sees how much it measures from outside the thread.
	printf("tamano del fichero: " UWORD_PRINT ", duracion %g s\n",
		file::size(file), sound::get_duration(th));

	// ESP: Un maestro enlazado al handle hilo sobrevive a la reconstrucción que provoca el cambio de tono.
	// La direccion es del programa: el modulo la lee en cada entrega y nunca la libera.
	// ENG: A master linked to the threaded handle survives the rebuilding a pitch change provokes.
	// The address is the program property: the module reads it on every delivery and never frees it.
	float master = 1.0f;
	sound::set_master_volume(th, &master);

	// ESP: El espectro mide lo que sale por la salida, ganancia incluida: es la prueba del volumen.
	// La media de un tono puro ronda el 0.63 de su amplitud pico, no el pico mismo.
	// ENG: The spectrum measures what leaves the output, gain included: that is the volume proof.
	// The mean of a pure tone hovers around 0.63 of its peak amplitude, not the peak itself.
	float* window = nullptr;
	UWORD samples = 0;
	float average = 0.0f;
	sleep(200);
	if(sound::get_spectrum(th, &window, &samples, &average))
		printf("espectro: " UWORD_PRINT " valores, media %g\n", samples, average);

	// ESP: Bajar el volumen a cero no silencia lo que ya viajo: los turnos pendientes gastan su retardo.
	// Por eso la media nueva se pide despues de esperar un rato mas de lo que caben dos buferes.
	// ENG: Dropping the volume to zero does not silence what already traveled: the queued turns spend their delay.
	// That is why the new mean gets asked after waiting a bit more than two buffers hold.
	sound::set_volume(th, 0.0f);
	sleep(600);
	float quiet = 0.0f;
	if(sound::get_spectrum(th, &window, &samples, &quiet))
		printf("media con volumen 0: %g\n", quiet);
	sound::set_volume(th, 1.0f);

	// ESP: Con bucle la posicion sube y cae: al agotarse el archivo el hilo vuelve al byte 44.
	// ENG: With a loop the position climbs and drops: once the file drains the thread goes back to byte 44.
	sleep(400);
	printf("posicion en bucle: %g\n", sound::get_position(th));
	sleep(800);
	printf("posicion tras el rebobinado: %g\n", sound::get_position(th));

	// ESP: Cambiar el tono sobre un handle hilo obliga al hilo a cerrar y rehacer la salida.
	// Ese corte deja un instante de silencio y suelta los turnos en camino.
	// La consulta puede caer justo en medio de la reconstruccion y responder sin maestro.
	// ENG: Changing the tone over a threaded handle forces the thread to close and redo the output.
	// That cut leaves one instant of silence and releases the turns on the way.
	// The query can land right in the middle of the rebuild and answer without a master.
	sound::set_pitch(th, 1.25f);
	printf("tono leido: %g, maestro intacto: %d\n",
		sound::pitch(th), (int)(sound::master_volume(th) == &master));
	sleep(200);

	// ESP: destroy pone el aviso de autodestruccion, espera al hilo y despues suelta el sonido interno.
	// Con canIcloser en false el fichero sigue abierto: aqui queda el cierre del programa.
	// ENG: destroy sets the autodestruction flag, waits for the thread and only then releases the internal sound.
	// With canIcloser at false the file stays open: the closing here is left to the program.
	printf("sonando antes de destroy: %d\n", (int)sound::playing(th));
	sound::destroy(th);
	printf("fichero vivo tras destroy: " UWORD_PRINT " bytes\n", file::size(file));
	file::close(file);

	if(window) delete[] window;
	printf("borrado: %s\n", file::remove(WAV_PATH) ? "si" : "no");
	puts("fin / done");
	return 0;
}
