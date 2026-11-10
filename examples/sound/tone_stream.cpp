/* // ESP
	Ejemplo de una salida de audio armada a mano, muestra a muestra.
	Muestra: sound::create, sound::set, sound::play, sound::stream, sound::sync,
	sound::playing, sound::stop y sound::destroy, con un tono fabricado sin matematicas.
	Compilar: g++ tone_stream.cpp -o tone_stream
	Requiere: #define ST_IMPORT_SOUND antes de incluir la cabecera.
	// ENG
	Example of an audio output built by hand, sample after sample.
	Shows: sound::create, sound::set, sound::play, sound::stream, sound::sync,
	sound::playing, sound::stop and sound::destroy, with a tone made without maths.
	Build: g++ tone_stream.cpp -o tone_stream
	Needs: #define ST_IMPORT_SOUND before including the header.
*/
#define ST_IMPORT_SOUND
#include "System_Tools.h"
using namespace System_Tools;

#define RATE_HZ    44100u
#define CHANNELS   2
#define BITS       16
#define FRAME_SIZE (CHANNELS * (BITS / 8))
#define BUF_SIZE   8192u
#define CHUNK_SIZE 8192u
#define TURNS      16

// ESP: El oscilador va por la receta de las dos muestras atras: la nueva sale de restar
// la de mas atras al doble del coseno del paso multiplicado por la de justo atras.
// Asi la senoidal corre continua y sin pedirle una sola funcion a las matematicas.
// El paso es 2*pi*440/44100, o sea unas 100 muestras y media por ciclo del La standard.
// ENG: The oscillator runs on the two-samples-behind recipe: the new one comes out of
// subtracting the further one from the step cosine doubled times the one right behind.
// That way the sine keeps running without asking the maths for a single function.
// The step is 2*pi*440/44100, which is about 100 and a half samples per cycle of standard A.
static float next_sample(){
	static float behind = 0.0626487f;
	static float further = 0.0f;
	float now = 1.9960716f * behind - further;
	further = behind;
	behind = now;
	return now;
}

// ESP: Helper propio: escribe tramas estereo de 16 bits con la misma senal en los dos canales.
// Los bytes sueltan primero el de menos peso, que es el orden que quiere el PCM de audio.
// La ganancia deja la senal floja: a tope, dos canales iguales ya rozan la saturacion.
// ENG: Own helper: writes stereo 16-bit frames with the same signal on both channels.
// The bytes release the least heavy one first, which is the order audio PCM wants.
// The gain leaves the signal weak: at full, two equal channels already brush the saturation.
static UWORD fill_frames(BYTE* dest, UWORD frames, float gain){
	UWORD written = 0;
	for(UWORD i = 0; i < frames; ++i){
		short level = (short)(next_sample() * gain * 32767.0f);
		for(BYTE c = 0; c < CHANNELS; ++c){
			dest[written] = (BYTE)((unsigned short)level & 0xFF);
			dest[written + 1] = (BYTE)(((unsigned short)level >> 8) & 0xFF);
			written += 2;
		}
	}
	return written;
}

int main(){
	// ESP: create arma los dos buferes de envio con la capacidad pedida: aqui 8192 bytes cada uno.
	// No abre dispositivo ninguno: hasta el play el handle solo existe en memoria.
	// ENG: create lays the two sending buffers with the asked capacity: 8192 bytes each here.
	// It opens no device: until the play the handle only exists in memory.
	void* snd = sound::create(BUF_SIZE);
	if(!snd){ puts("sin handle / no handle"); return 1; }

	BYTE* chunk = (BYTE*)memory::alloc(CHUNK_SIZE * 2);
	if(!chunk){ puts("sin bufer / no buffer"); sound::destroy(snd); return 1; }
	memory::slloc(chunk, 0, CHUNK_SIZE * 2);

	printf("formato: %u Hz, %u canales, %u bits -> trama de %u bytes\n",
		RATE_HZ, CHANNELS, BITS, FRAME_SIZE);

	// ESP: Antes de play no hay adonde enviar: stream responde 0 y no gasta ningun turno.
	// ENG: Before play there is nowhere to send: stream answers 0 and spends no turn at all.
	printf("entrega antes de play: " UWORD_PRINT " bytes\n",
		sound::stream(snd, chunk, CHUNK_SIZE));

	sound::set(snd, RATE_HZ, CHANNELS, BITS);
	// ESP: set ignora los ceros y no toca nada mientras la salida suena: hay que pararla antes.
	// ENG: set ignores the zeros and touches nothing while the output sounds: it must be stopped first.
	printf("sonando tras create: %d\n", (int)sound::playing(snd));

	if(!sound::play(snd)) puts("play rechazado / play refused");
	// ESP: playing puede seguir en false aunque play responda true: sin salida conectada no hay nada abierto.
	// Las entregas son las que reconectan el dispositivo cuando el altavoz aparece.
	// ENG: playing can stay false even when play answers true: with no output plugged nothing is open.
	// The deliveries are what reconnect the device once the speaker shows up.
	printf("sonando tras play: %d\n", (int)sound::playing(snd));

	// ESP: Cada vuelta rellena un bufer entero y lo entrega: sync avisa cuando queda un turno libre.
	// Esperar con sleep es lo correcto; insistir con stream solo gasta CPU.
	// ENG: Each round fills one whole buffer and delivers it: sync warns when a turn stands free.
	// Waiting with sleep is the right thing; insisting with stream only burns CPU.
	UWORD turns_ok = 0;
	for(int turn = 0; turn < TURNS; ++turn){
		UWORD bytes = fill_frames(chunk, CHUNK_SIZE / FRAME_SIZE, 0.25f);
		int guard = 0;
		while(!sound::sync(snd) && ++guard < 100) sleep(10);
		UWORD accepted = sound::stream(snd, chunk, bytes);
		if(!accepted) break;
		++turns_ok;
	}
	printf("turnos entregados: " UWORD_PRINT " de %d\n", turns_ok, TURNS);
	if(!turns_ok) puts("sin salida de audio: las entregas responden 0 / no audio output: deliveries answer 0");

	// ESP: Pedir mas de lo que cabe no es un fallo: la respuesta trae lo que realmente entro.
	// Lo demas queda en el bufer del programa y hay que reenviarlo en la llamada siguiente.
	// ENG: Asking more than fits is not a failure: the answer brings what really went in.
	// The rest stays in the program buffer and must be resent on the next call.
	UWORD big_ask = CHUNK_SIZE * 2;
	UWORD big_got = sound::stream(snd, chunk, big_ask);
	printf("entrega de " UWORD_PRINT " pedidos: " UWORD_PRINT " aceptados\n", big_ask, big_got);

	// ESP: stop suelta la salida y los turnos en camino; el handle sigue siendo util.
	// play sobre el mismo handle vuelve a abrir el dispositivo y las entregas vuelven a entrar.
	// ENG: stop releases the output and the turns on the way; the handle keeps being usable.
	// play over the same handle opens the device again and the deliveries come back in.
	sound::stop(snd);
	printf("sonando tras stop: %d, entrega tras stop: " UWORD_PRINT "\n",
		(int)sound::playing(snd), sound::stream(snd, chunk, CHUNK_SIZE));
	if(sound::play(snd)){
		UWORD after = sound::stream(snd, chunk, CHUNK_SIZE);
		printf("reconectado, entrega: " UWORD_PRINT "\n", after);
	}

	// ESP: Un handle con buferes de cero bytes nace valido y nunca acepta una sola muestra.
	// ENG: A handle with zero-byte buffers is born valid and never accepts a single sample.
	void* sordo = sound::create(0);
	if(sordo){
		sound::play(sordo);
		printf("create(0), entrega: " UWORD_PRINT "\n", sound::stream(sordo, chunk, 64));
		sound::destroy(sordo);
	}

	// ESP: destroy primero detiene y despues suelta: el puntero deja de servir para todo el modulo.
	// ENG: destroy stops first and releases after: the pointer stops working for the whole module.
	sound::destroy(snd);
	if(!memory::free(chunk)) chunk = nullptr;
	puts("fin / done");
	return 0;
}
