/* // ESP
	Contrajemplo: un buffer de PCM declarado const entregado a sound::stream.
	Compilar: g++ sound_stream_const_buffer.cpp -o sound_stream_const_buffer
	Fallo: invalid conversion from 'const void*' to 'void*' [-fpermissive]
	inicializando el argumento 2 de 'UWORD System_Tools::sound::stream(void*, void*, UWORD)'
	Causa: stream recibe el audio externo como void*, sin const, y un arreglo declarado
	const float[] llega como const void*; hacia atras el C++ normal no la deja pasar.
	Salida: quitar el const del buffer, o copiar el tramo a un buffer propio antes de entregarlo.
	Nota: con -fpermissive baja a aviso. Con los flags del Makefile es error. No ejecutar:
	al ser de audio, este fichero solo se comprueba compilando, y aqui compilar debe fallar.
	// ENG
	Counterexample: a PCM buffer declared const handed over to sound::stream.
	Build: g++ sound_stream_const_buffer.cpp -o sound_stream_const_buffer
	Failure: invalid conversion from 'const void*' to 'void*' [-fpermissive]
	initializing argument 2 of 'UWORD System_Tools::sound::stream(void*, void*, UWORD)'
	Cause: stream takes the external audio as void*, with no const, and an array declared
	const float[] arrives as const void*; backwards the regular C++ does not let it through.
	The way out: drop the const from the buffer, or copy the stretch into a buffer of your own first.
	Note: with -fpermissive it drops to a warning. With the Makefile flags it is an error. Do not run:
	being audio, this file only gets checked by compiling, and here compiling must fail.
*/
#define ST_IMPORT_SOUND
#include "System_Tools.h"
using namespace System_Tools;

#define SAMPLES 1024u

// ESP: El buffer de lectura: una forma de onda ya armada, guardada como constante.
// ENG: The reading buffer: an already laid out wave, kept as a constant.
static const float wave[SAMPLES] = { 0.0f };

int main(){
	void* snd = sound::create(SAMPLES * sizeof(float));

	// ESP: aqui esta el fallo: wave es const void* y stream pide void*.
	// ENG: here is the failure: wave is const void* and stream asks for void*.
	UWORD loaded = sound::stream(snd, wave, sizeof(wave));

	// ESP: Las dos salidas, en el bloque de abajo: quitar el const del arreglo de arriba, o
	// copiar el tramo a un buffer propio. Apague la linea del fallo antes de probar ninguna.
	// ENG: The two exits, in the block below: drop the const from the array above, or copy the
	// stretch into a buffer of your own. Turn the failing line off before testing either one.
#if 0
	static float wave_copy[SAMPLES] = { 0.0f };
	loaded = sound::stream(snd, wave_copy, sizeof(wave_copy));
	float mine[SAMPLES];
	memory::clloc(mine, wave, sizeof(wave));
	loaded = sound::stream(snd, mine, sizeof(mine));
#endif

	printf("muestras cargadas: " UWORD_PRINT "\n", loaded);
	sound::destroy(snd);
	return 0;
}
