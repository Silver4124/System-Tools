/* // ESP
	Ejemplo de las ganancias y del tono: volumen propio, volumen maestro compartido y pitch.
	Muestra: sound::set_volume, sound::volume, sound::set_master_volume, sound::master_volume,
	sound::set_pitch, sound::pitch y sound::is_threaded sobre dos handles normales.
	Compilar: g++ volume_pitch.cpp -o volume_pitch
	Requiere: #define ST_IMPORT_SOUND antes de incluir la cabecera.
	// ENG
	Example of the gains and the pitch: own volume, shared master volume and pitch.
	Shows: sound::set_volume, sound::volume, sound::set_master_volume, sound::master_volume,
	sound::set_pitch, sound::pitch and sound::is_threaded over two plain handles.
	Build: g++ volume_pitch.cpp -o volume_pitch
	Needs: #define ST_IMPORT_SOUND before including the header.
*/
#define ST_IMPORT_SOUND
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: El maestro es un float del programa: el modulo solo guarda la direccion, nunca el valor.
	// Mientras algún handle lo apunte, hay que mantenerlo vivo.
	// ENG: The master is a float of the program: the module only keeps the address, never the value.
	// While some handle points at it, it has to stay alive.
	float master = 0.5f;

	void* left = sound::create(4096);
	void* right = sound::create(4096);
	if(!left || !right){ puts("sin handles / no handles"); return 1; }

	// ESP: Nacen con las ganancias de manual: volumen 1.0, tono 1.0 y sin maestro enlazado.
	// ENG: They are born with the manual gains: volume 1.0, pitch 1.0 and no master linked.
	printf("left: volumen %g, tono %g, maestro %p\n",
		sound::volume(left), sound::pitch(left), (void*)sound::master_volume(left));

	// ESP: set_master_volume enlaza la misma direccion en los dos: un deslizador para los dos a la vez.
	// La firma con dos argumentos de create tambien lo hace, pero la cabecera solo declara la firma base.
	// ENG: set_master_volume links the same address in both: one slider for both at once.
	// The two-argument signature of create does it too, but the header only declares the base one.
	sound::set_master_volume(left, &master);
	sound::set_master_volume(right, &master);
	// ESP: master_volume entrega la direccion, no la cifra: para el numero hay que desreferenciar.
	// ENG: master_volume hands over the address, not the figure: the number needs dereferencing.
	printf("left enlaza la direccion del programa: %d\n",
		(int)(sound::master_volume(left) == &master));

	// ESP: La ganancia de una entrega es volumen del handle por el valor del maestro en ese momento.
	// Los datos ya enviados conservan la ganancia vieja: la huella queda impresa en el bufer.
	// ENG: The gain of a delivery is the handle volume times the master value at that moment.
	// Data already sent keeps the old gain: the footprint stays printed on the buffer.
	sound::set_volume(left, 0.8f);
	float read_volume = 0.0f;
	printf("left: bool %d, volumen %g, efectiva %g\n",
		(int)sound::volume(left, &read_volume), read_volume,
		(double)(read_volume * master));

	// ESP: Cambiar el float del maestro se refleja en la siguiente entrega de todos los enlazados.
	// Nadie avisa a los handles: el cambio se nota porque se lee la direccion en cada muestra.
	// ENG: Changing the master float shows up in the next delivery of every linked handle.
	// Nobody warns the handles: the change lands because the address gets read at each sample.
	master = 0.25f;
	printf("maestro visto desde right: %g\n", *(sound::master_volume(right)));

	// ESP: nullptr desenlaza el maestro y la ganancia vuelve a ser solo la del volumen.
	// La funcion responde true igual: el desenlace tambien es un registro valido.
	// ENG: nullptr unlinks the master and the gain goes back to being only the volume one.
	// The function answers true all the same: unlinking is a valid record too.
	printf("desenlazar: %d, direccion ahora %p\n",
		(int)sound::set_master_volume(left, nullptr), (void*)sound::master_volume(left));

	// ESP: set_pitch guarda el tono y, si la salida esta abierta, pide el cambio al dispositivo.
	// El tono no toca las muestras: altera la velocidad con la que la salida las consume.
	// En un handle normal el dispositivo se abre con frecuencia = muestreo por tono,
	// asi que un cambio a fondo hay queirlo con la salida cerrada o recrear el handle.
	// ENG: set_pitch keeps the tone and, if the output is open, asks the device for the change.
	// The tone touches no samples: it alters the speed with which the output consumes them.
	// In a plain handle the device opens with rate = sampling times tone,
	// so a deep change has to be asked with the output closed, or the handle rebuilt.
	sound::set_pitch(right, 1.5f);
	float read_pitch = 0.0f;
	printf("right: tono leido %g, bool %d\n", sound::pitch(right),
		(int)sound::pitch(right, &read_pitch));

	// ESP: El tono de un handle normal no altera lo que stream copia: aqui solo viajan muestras.
	// Sin salida conectada el play no abre dispositivo y las entregas responden 0.
	// ENG: The tone of a plain handle alters nothing that stream copies: only samples travel here.
	// With no output plugged the play opens no device and the deliveries answer 0.
	sound::play(right);
	BYTE tone[64];
	memory::slloc(tone, 0, 64);
	printf("entrega con tono 1.5: " UWORD_PRINT "\n", sound::stream(right, tone, 64));
	sound::stop(right);

	// ESP: is_threaded separa las dos clases de handle: ninguna ganancia cambia de clase.
	// El tono de un handle hilo obliga al hilo a reconstruir la salida y eso corta el audio.
	// ENG: is_threaded splits the two handle classes: no gain changes the class.
	// The tone of a threaded handle forces the thread to rebuild the output and that cuts the audio.
	printf("left hilo: %d, right hilo: %d\n",
		(int)sound::is_threaded(left), (int)sound::is_threaded(right));

	// ESP: El float del maestro sigue siendo del programa: destroy no lo libera, solo lo olvida.
	// Los handles ya no existen, asi que despues de aqui el float puede morir tranquilo.
	// ENG: The master float stays the program property: destroy never frees it, it only forgets it.
	// The handles no longer exist, so past this point the float can die at ease.
	sound::destroy(left);
	sound::destroy(right);
	printf("maestro tras destroy: %g\n", master);
	puts("fin / done");
	return 0;
}
