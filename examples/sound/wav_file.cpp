/* // ESP
	Ejemplo de un WAV fabricado por el programa y reproducido por el hilo del modulo.
	Muestra: sound::file_stream, sound::get_duration, sound::get_position, sound::get_spectrum,
	sound::fade, sound::seek, sound::reload, sound::playing y sound::destroy sobre un Handle Sonido Hilo.
	Compilar: g++ wav_file.cpp -o wav_file
	Requiere: #define ST_IMPORT_SOUND y #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of a WAV made by the program and played back by the module thread.
	Shows: sound::file_stream, sound::get_duration, sound::get_position, sound::get_spectrum,
	sound::fade, sound::seek, sound::reload, sound::playing and sound::destroy over a Handle Threaded Sound.
	Build: g++ wav_file.cpp -o wav_file
	Needs: #define ST_IMPORT_SOUND and #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_SOUND
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define WAV_PATH   "st_wav_file.wav"
#define WAV_RATE   22050u
#define WAV_SEC    2u
#define WAV_FRAMES (WAV_RATE * WAV_SEC)
#define DATA_SIZE  (WAV_FRAMES * 2u)
#define WAV_SIZE   (44u + DATA_SIZE)

// ESP: Helper propio: suelta dos bytes de menos peso primero, como quiere el formato WAV.
// ENG: Own helper: drops the two least heavy bytes first, the way the WAV format wants.
static void put16(BYTE* at, unsigned int value){
	at[0] = (BYTE)(value & 0xFF);
	at[1] = (BYTE)((value >> 8) & 0xFF);
}

// ESP: Helper propio: lo mismo pero con cuatro bytes.
// ENG: Own helper: the same thing but with four bytes.
static void put32(BYTE* at, unsigned long value){
	at[0] = (BYTE)(value & 0xFF);
	at[1] = (BYTE)((value >> 8) & 0xFF);
	at[2] = (BYTE)((value >> 16) & 0xFF);
	at[3] = (BYTE)((value >> 24) & 0xFF);
}

// ESP: Helper propio: copia una etiqueta de cuatro letras tal cual, sin contar nulos.
// ENG: Own helper: copies a four letter tag as it is, without counting nulls.
static void put_tag(BYTE* at, const char* tag){
	for(UWORD i = 0; i < 4; ++i) at[i] = (BYTE)tag[i];
}

// ESP: El mismo truco de las dos muestras atras: aqui con el paso de 440 Hz sobre 22050.
// La constante es dos veces el coseno del paso y el arranque es el seno del primer paso.
// ENG: The same two-samples-behind trick: here with the 440 Hz step over 22050.
// The constant is two times the step cosine and the start is the sine of the first step.
static float next_sample(){
	static float behind = 0.1250547f;
	static float further = 0.0f;
	float now = 1.9843024f * behind - further;
	further = behind;
	behind = now;
	return now;
}

// ESP: El módulo no escribe WAV: monta la cabecera el programa, byte a byte y con sus cuentas.
// Los campos que el modulo mira son los canales en 22, la frecuencia en 24 y los bits en 34.
// Los datos empiezan en el 44: cualquier trozo extra antes del audio se lee como ruido.
// ENG: The module writes no WAV: the program assembles the header, byte by byte with its own counts.
// The fields the module looks at are channels at 22, frequency at 24 and bits at 34.
// Data starts at 44: any extra piece before the audio reads as noise.
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
	BYTE* body = wav + 44;
	for(UWORD i = 0; i < WAV_FRAMES; ++i){
		short level = (short)(next_sample() * 0.3f * 32767.0f);
		put16(body + (i * 2), (unsigned int)(unsigned short)level);
	}
	return wav;
}

int main(){
	BYTE* wav = build_wav();
	if(!wav){ puts("sin WAV / no WAV"); return 1; }

	// ESP: write|create: sin create la apertura de un fichero nuevo falla.
	// La escritura responde los bytes aceptados: se compara con la cuenta propia.
	// ENG: write|create: without create the opening of a brand new file fails.
	// The write answers the accepted bytes: it gets compared with the own count.
	void* file = file::open(WAV_PATH, file_mode::write | file_mode::create);
	if(!file){ puts("no se pudo crear el WAV / the WAV could not be created"); memory::free(wav); return 1; }
	UWORD wrote = file::write(file, wav, WAV_SIZE);
	file::close(file);
	printf("WAV escrito: " UWORD_PRINT " de " UWORD_PRINT " bytes\n", wrote, (UWORD)WAV_SIZE);
	memory::free(wav);

	// ESP: La firma breve por ruta abre y cierra el fichero ella sola: el programador no ve el handle.
	// Responde ya con la reproduccion en marcha y no bloquea: el hilo alimenta mientras el programa sigue.
	// Looping en false: al agotarse el archivo el hilo se despide y deja el sonido interno vivo.
	// ENG: The brief signature by path opens and closes the file on its own: the programmer never sees the handle.
	// It answers with playback already running and does not block: the thread feeds while the program goes on.
	// Looping at false: once the file runs out the thread says goodbye and leaves the internal sound alive.
	void* th = sound::file_stream(WAV_PATH, false, 1.0f);
	if(!th){ puts("sin sonido hilo / no threaded sound"); file::remove(WAV_PATH); return 1; }
	printf("es handle hilo: %d\n", (int)sound::is_threaded(th));

	// ESP: La duracion sale del tamano del fichero menos los 44 bytes de la cabecera.
	// La posicion resta la latencia de los dos buferes: de joven puede salir en negativo.
	// ENG: The duration comes from the file size minus the 44 header bytes.
	// The position subtracts the latency of the two buffers: young it can come out negative.
	printf("duracion %g s, posicion %g s\n",
		sound::get_duration(th), sound::get_position(th));

	// ESP: stream rechaza cualquier dato sobre un handle hilo: la alimentacion ya tiene dueno.
	// ENG: stream refuses any data over a threaded handle: the feeding already has an owner.
	printf("entrega propia sobre handle hilo: " UWORD_PRINT "\n",
		sound::stream(th, (void*)"datos", 6));

	// ESP: La ventana del espectro la crea el modulo con new y la entrega: es propiedad del programa.
	// Repetir la llamada con la misma direccion libera la ventana vieja; al final toca borrarla.
	// Los valores traen la ganancia aplicada y los canales van intercalados.
	// ENG: The spectrum window gets created by the module with new and handed over: it is the program property.
	// Repeating the call with the same address frees the old window; at the end it must be deleted.
	// The values arrive with the gain applied and the channels go interleaved.
	float* window = nullptr;
	UWORD samples = 0;
	float average = 0.0f;
	if(sound::get_spectrum(th, &window, &samples, &average)){
		printf("espectro: " UWORD_PRINT " valores, media %g, primer valor %g\n",
			samples, average, window ? (double)window[0] : 0.0);
	}else puts("espectro rechazado / spectrum refused");

	// ESP: fade no mueve el volumen en la llamada: anota el destino y el paso por ciclo del hilo.
	// El hilo da un paso cada entrega, asi que el fundido tarda lo que tarda en leer el archivo.
	// ENG: fade moves no volume in the call: it writes down the destination and the step per thread cycle.
	// The thread takes one step per delivery, so the fade takes whatever reading the file takes.
	sound::fade(th, 0.2f, 400);
	sleep(300);
	printf("volumen tras el fundido: %g\n", sound::volume(th));

	// ESP: seek mueve el puntero del fichero a una trama entera y responde false si se sale del final.
	// reload es lo mismo que seek(0): salta a los 44 bytes y deja que el hilo siga leyendo desde ahi.
	// ENG: seek moves the file pointer to a whole frame and answers false if it leaves past the end.
	// reload is the same as seek(0): it jumps to the 44 bytes and lets the thread keep reading from there.
	printf("seek a medio segundo: %d, posicion %g\n",
		(int)sound::seek(th, 0.5f), sound::get_position(th));
	printf("seek fuera del fichero: %d\n", (int)sound::seek(th, 99.0f));
	printf("reload: %d, posicion %g\n", (int)sound::reload(th), sound::get_position(th));

	// ESP: Playing en false sin haber parado indica que el hilo ya termino el archivo y se despidio.
	// El handle sigue siendo consultable y el espectro conserva el ultimo bufer entregado.
	// ENG: Playing at false without having stopped means the thread finished the file and said goodbye.
	// The handle keeps being queryable and the spectrum holds the last delivered buffer.
	int guard = 0;
	while(sound::playing(th) && ++guard < 80) sleep(50);
	printf("sonando tras el agotamiento: %d (espera de %d veces 50 ms)\n",
		(int)sound::playing(th), guard);
	if(sound::get_spectrum(th, &window, &samples, nullptr)){
		printf("espectro del ultimo bufer: " UWORD_PRINT " valores\n", samples);
	}

	// ESP: Sobre un handle normal el espectro responde false: la ventana ni se toca.
	// ENG: Over a plain handle the spectrum answers false: the window does not even get touched.
	void* plain = sound::create(1024);
	float* other = nullptr;
	UWORD other_n = 0;
	printf("espectro sobre handle normal: %d\n",
		(int)sound::get_spectrum(plain, &other, &other_n, nullptr));
	sound::destroy(plain);

	// ESP: destroy espera al hilo antes de soltar nada: con el archivo agotado el paso es inmediato.
	// La ruta se cierra sola por la firma breve, asi que despues ya se puede borrar.
	// ENG: destroy waits for the thread before releasing anything: with the file drained the step is instant.
	// The path closes itself through the brief signature, so afterwards it can already be removed.
	sound::destroy(th);
	if(window) delete[] window;
	printf("borrado: %s\n", file::remove(WAV_PATH) ? "si" : "no");
	puts("fin / done");
	return 0;
}
