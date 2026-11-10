/* // ESP
	Ejemplo del volumen maestro compartido: la version de sound::create que no esta
	declarada en el encabezado pero si definida en el modulo.
	Muestra: sound::create(buffer_size, float*), sound::master_volume en sus dos firmas,
	sound::set_master_volume, sound::volume y sound::set_volume enlazados a un unico float.
	Compilar: g++ shared_master.cpp -o shared_master
	Requiere: #define ST_IMPORT_SOUND antes de incluir la cabecera.
	El encabezado solo declara create(unsigned); la firma con maestro vive en la
	definicion y, al ser libreria de cabecera, el programa la ve y la enlaza.
	Este ejemplo suena: se compueba compilando, y se ejecuta cuando el usuario quiera oir.
	// ENG
	Example of the shared master volume: the sound::create version that is not declared
	in the header but is defined in the module.
	Shows: sound::create(buffer_size, float*), sound::master_volume in its two signatures,
	sound::set_master_volume, sound::volume and sound::set_volume tied to a single float.
	Build: g++ shared_master.cpp -o shared_master
	Needs: #define ST_IMPORT_SOUND before including the header.
	The header declares only create(unsigned); the master signature lives in the
	definition and, being a header library, the program sees it and links it.
	This example makes noise: it gets checked by compiling, and runs whenever the user wants to listen.
*/
#define ST_IMPORT_SOUND
#include "System_Tools.h"
using namespace System_Tools;

#define RATE_HZ    22050u
#define CHANNELS   1
#define BITS       16
#define BUF_SIZE   4096u
#define CHUNK      2048u
#define TURNS      24

// ESP: master guarda el float compartido: es memoria del programa, no del modulo.
// Vivira mientras algun handle lo apunte, porque el modulo lo lee en cada muestra y no lo copia.
// ENG: master holds the shared float: it is the program's memory, not the module's.
// It will live while some handle points at it, because the module reads it on every sample and never copies it.
static float master = 0.8f;

// ESP: Una sierra simple escrita a mano en 16 bits con signo: nada de matematicas ni de ficheros.
// La amplitud se deja al 60 por ciento para que el maestro de arriba tenga margen de sobra.
// ENG: A simple saw written by hand in signed 16 bits: no maths and no files.
// The amplitude stays at 60 percent so the master above has plenty of room.
static void fill_turn(short* dst, UWORD samples, unsigned int* phase){
	for(UWORD i = 0; i < samples; ++i){
		*phase = (*phase + 1u) % RATE_HZ;
		dst[i] = (short)((int)(*phase * 2) - (int)RATE_HZ) / 4;
	}
}

int main(){
	// ESP: create con el segundo argumento enlaza el maestro en el mismo paso: sin set_master_volume aparte.
	// Los dos handles apuntan el MISMO float: una escritura sobre master gobierna la salida de ambos.
	// ENG: create with the second argument links the master in the very same step: no separate set_master_volume.
	// Both handles point the SAME float: one write over master governs the output of the two.
	void* low = sound::create(BUF_SIZE, &master);
	void* high = sound::create(BUF_SIZE, &master);
	if(!low || !high){ puts("sin salida de audio / no audio output"); return 1; }
	sound::set(low,  RATE_HZ, CHANNELS, BITS);
	sound::set(high, RATE_HZ * 3 / 2, CHANNELS, BITS);

	// ESP: master_volume(handle) entrega la direccion enlazada: nullptr en el handle sin maestro.
	// La otra firma escribe esa direccion en un destino: ambas miran el mismo sitio.
	// ENG: master_volume(handle) hands over the linked address: nullptr on the handle without a master.
	// The other signature writes that address into a destination: both look at the same place.
	float* back = nullptr;
	sound::master_volume(low, &back);
	printf("enlazado: %p, igual a &master: %d, leido: %p\n",
		(void*)back, (int)(back == &master), (void*)sound::master_volume(low));

	// ESP: El volumen por handle sigue siendo independiente: el maestro multiplica, no reemplaza.
	// gain total = volume * *master, segun escala stream.
	// ENG: The per-handle volume stays independent: the master multiplies, it does not replace.
	// total gain = volume * *master, as stream scales it.
	sound::set_volume(low, 1.0f);
	sound::set_volume(high, 0.35f);

	// ESP: Un tercer handle sin maestro: la firma base deja el puntero en nullptr y escala solo por volume.
	// Se apaga a mano con su volumen, porque no tiene a quien compartir la subida.
	// ENG: A third handle without a master: the base signature leaves the pointer in nullptr and scales by volume only.
	// It gets silenced by hand through its volume, because it has nobody to share the climb with.
	void* lone = sound::create(BUF_SIZE);
	if(lone){
		sound::set(lone, RATE_HZ / 2, CHANNELS, BITS);
		printf("handle solo sin maestro: %p\n", (void*)sound::master_volume(lone));
	}

	short* turn = (short*)memory::alloc(CHUNK * sizeof(short));
	if(!turn){ puts("sin memoria / no memory"); return 1; }
	unsigned int phase_a = 0, phase_b = 0, phase_c = 0;

	sound::play(low);
	sound::play(high);
	if(lone) sound::play(lone);

	for(int t = 0; t < TURNS; ++t){
		// ESP: Moviendo el float compartido se mueven los dos handles a la vez: no hay recarga alguna.
		// El cambio se nota en la siguiente muestra, no en la proxima llamada.
		// ENG: Moving the shared float moves both handles at once: there is no reload of any kind.
		// The change shows on the next sample, not on the next call.
		if(t == TURNS / 3)      master = 0.15f;
		else if(t == 2 * TURNS / 3) master = 0.6f;

		fill_turn(turn, CHUNK, &phase_a);
		sound::stream(low, turn, CHUNK * sizeof(short));
		fill_turn(turn, CHUNK, &phase_b);
		sound::stream(high, turn, CHUNK * sizeof(short));
		if(lone){
			sound::set_volume(lone, (float)(TURNS - t) / (float)TURNS);
			fill_turn(turn, CHUNK, &phase_c);
			sound::stream(lone, turn, CHUNK * sizeof(short));
		}

		// ESP: sync avisa cuando queda un turno libre: esperar con sleep es lo correcto.
		// El guarda por si la salida no llega: sin dispositivo, el bufer nunca se consume y el bucle colgaria.
		// ENG: sync warns when one turn stands free: waiting with sleep is the right thing.
		// The guard stays because the output may never come: with no device the buffer never gets consumed and the loop would hang.
		int guard = 0;
		while(!sound::sync(low) && ++guard < 100) sleep(10);
	}

	printf("maestro al final: %g, el mismo float: %d\n", master, (int)(sound::master_volume(high) == &master));

	// ESP: destroy libera el handle y su salida, pero NO el float del programa: la propiedad sigue siendo del codigo.
	// memory::szlen sobre un float de pila no es una consulta valida: por eso solo se imprime su valor.
	// ENG: destroy frees the handle and its output, but NOT the program's float: ownership stays with the code.
	// memory::szlen over a stack float is no valid query: that is why only its value gets printed.
	sound::destroy(low);
	sound::destroy(high);
	if(lone) sound::destroy(lone);
	if(!memory::free(turn)) puts("memoria de turnos liberada / turn memory released");
	printf("master intacto tras los destroy: %g\n", master);
	return 0;
}
