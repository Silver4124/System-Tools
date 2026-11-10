/* // ESP
	Ejemplo de la lectura de un JSON ya escrito: parseo y recorrido completo.
	Muestra: json::parse, json::type, json::size, json::value, json::value_view,
	json::nme_elem, json::fnd_elem y json::detach sobre un objeto con todos los tipos.
	Compilar: g++ parse_walk.cpp -o parse_walk
	Requiere: #define ST_IMPORT_JSON y #define ST_IMPORT_FILE antes de incluir la cabecera.
	// ENG
	Example of reading an already written JSON: parsing and a full walk.
	Shows: json::parse, json::type, json::size, json::value, json::value_view,
	json::nme_elem, json::fnd_elem and json::detach over an object holding every type.
	Build: g++ parse_walk.cpp -o parse_walk
	Needs: #define ST_IMPORT_JSON and #define ST_IMPORT_FILE before including the header.
*/
#define ST_IMPORT_JSON
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;

#define DEMO_TEXT \
	"{ \"nombre\": \"sistema\", \"version\": 15, \"negativo\": -42, \"real\": 2.5,"\
	" \"activo\": true, \"oculto\": false, \"vacio\": null,"\
	" \"lista\": [ 1, 2, 3, \"tres\" ], \"hijo\": { \"a\": 1, \"b\": \"dos\" } }"
#define DEMO_FILE "st_jw_text.json"

// ESP: Helper propio: muestra el tipo y el valor de un nodo, y baja dentro de los contenedores.
// El nodo se entrega ya abierto: los escalares no hay que cerrarlos y los contenedores sí, con detach.
// ENG: Own helper: shows a node's type and value, and steps inside the containers.
// The node arrives already open: scalars need no closing, containers do, through detach.
static void show_node(const char* label, void* node){
	BYTE kind = json::type(node);
	// ESP: Los hijos terminan su propia linea: si la linea se bajo a un hijo, el padre no repite el salto.
	// ENG: The children finish their own line: if the line dropped down to a child, the father skips the break.
	bool line_open = true;
	printf("  %-9s tipo 0x%02X:", label, (unsigned)kind);
	if(node){
		switch(kind){
			// ESP: value se llama solo en los escalares: en un contenedor tambien reserva handle nuevo.
			// Los negativos salen ya en complemento a dos: el cast directo devuelve el numero.
			// ENG: value gets called on the scalars only: on a container it reserves a new handle too.
			// The negatives come out in two's complement already: a plain cast gives the number.
			case json_type::str:  printf(" cadena [%s] de " UWORD_PRINT " caracteres",
				(const char*)json::value(node, nullptr), json::size(node)); break;
			// ESP: Los enteros viajan empaquetados dentro del puntero: se sacan con un cast.
			// ENG: Integers travel packed inside the pointer: a cast takes them out.
			case json_type::num:  printf(" entero " WORD_PRINT,
				(WORD)json::value(node, nullptr)); break;
			case json_type::num | json_type::uns:
				printf(" entero sin signo " UWORD_PRINT,
					(UWORD)json::value(node, nullptr)); break;
			case json_type::flt:  printf(" real %g",
				*(float*)json::value(node, nullptr)); break;
			// ESP: Un booleano no es un nodo: el valor es el propio puntero, 0 para false y 1 para true.
			// Por eso false y "no encontrado" se parecen: el tipo es el que los distingue.
			// ENG: A boolean is not a node: the value is the pointer itself, 0 for false and 1 for true.
			// That is why false and "not found" look alike: it is the type that tells them apart.
			case json_type::bll:  case json_type::bll | json_type::uns:
				printf(" booleano %d", (int)(UWORD)json::value(node, nullptr)); break;
			case json_type::nll:  printf(" nulo"); break;
			case json_type::ary: {
				UWORD n = json::size(node);
				printf(" array de " UWORD_PRINT, n);
				for(UWORD i = 0; i < n; ++i){
					// ESP: En una matriz el offset es el indice: va convertido en puntero, no como ruta anidada.
					// ENG: In an array the offset is the index: cast into a pointer, not a nested route.
					void* elem = json::value(node, (void*)i);
					if(!elem){ puts("   elemento inexistente / missing element"); continue; }
					// ESP: El salto baja al primer hijo: los demas ya vienen con su linea cerrada del hilo anterior.
					// ENG: The break drops on the first child: the rest come with their line shut by the previous thread.
					if(line_open){
						printf("\n");
						line_open = false;
					}
					char tag[16];
					sprintf(tag, "    [%u]", (unsigned)i);
					show_node(tag, elem);
					json::detach(elem);
				}
				break;
			}
			case json_type::obj: {
				UWORD n = json::size(node);
				printf(" objeto de " UWORD_PRINT " entradas", n);
				for(UWORD i = 0; i < n; ++i){
					const char* name = json::nme_elem(node, i);
					void* child = json::value(node, (void*)name);
					if(!child){ puts("   entrada inexistente / missing entry"); continue; }
					if(line_open){
						printf("\n");
						line_open = false;
					}
					char tag[32];
					sprintf(tag, "    %s", name);
					show_node(tag, child);
					json::detach(child);
				}
				break;
			}
			default: printf(" sin lectura"); break;
		}
	}
	if(line_open) puts("");
}

int main(){
	// ESP: parse acepta texto en bruto: copia todo lo que encuentra en memoria propia del modulo.
	// El texto origen puede ser literal: no hace falta que siga existiendo despues de la llamada.
	// ENG: parse takes raw text: it copies everything it finds into the module own memory.
	// The source text may be a literal: it does not have to keep existing after the call.
	void* jsn = json::parse((void*)DEMO_TEXT);
	if(!jsn){ puts("no se pudo parsear el texto / text could not be parsed"); return 1; }
	printf("raiz: tipo 0x%02X objeto con " UWORD_PRINT " entradas\n",
		(unsigned)json::type(jsn), json::size(jsn));

	// ESP: Pasar el mismo handle por parse es un no-op: responde la misma direccion sin copiar.
	// ENG: Feeding the very same handle through parse is a no-op: it answers the same address without copying.
	printf("parse sobre el propio JSON: %s\n", json::parse(jsn) == jsn ? "misma direccion" : "copia");

	// ESP: Un texto que no es JSON no arma nada: nullptr, sin dejar memoria pendiente.
	// ENG: Text that is not JSON builds nothing: nullptr, with no memory left hanging.
	printf("parse de basura: %p\n", json::parse((void*)("esto tampoco es JSON")));

	// ESP: nme_elem pide un indice 0 basado y devuelve la etiqueta apuntada dentro del propio JSON.
	// value pide la etiqueta como puntero a texto: no admite rutas anidadas con puntos.
	// ENG: nme_elem asks a 0-based index and returns the label pointed inside the JSON itself.
	// value asks the label as a text pointer: it takes no nested dotted routes.
	UWORD members = json::size(jsn);
	for(UWORD i = 0; i < members; ++i){
		const char* name = json::nme_elem(jsn, i);
		// ESP: fnd_elem responde la posicion 1 basada: con un indice 0 basado, es una mas.
		// ENG: fnd_elem answers the 1-based position: from a 0-based index, it is one more.
		UWORD found = json::fnd_elem(jsn, (void*)name);
		void* child = json::value(jsn, (void*)name);
		printf("[%u] fnd_elem da " UWORD_PRINT ", handle %p\n",
			(unsigned)i, found, child);
		if(child){
			show_node(name, child);
			// ESP: value entrega memoria nueva en los contenedores: detach la devuelve.
			// En los escalares no hay handle que soltar: detach rechazaria el puntero.
			// ENG: value hands brand new memory on containers: detach gives it back.
			// On scalars there is no handle to release: detach would refuse the pointer.
			json::detach(child);
		}
	}

	// ESP: Una etiqueta inventada no esta: value responde nullptr y fnd_elem responde 0.
	// ENG: An invented label is not there: value answers nullptr and fnd_elem answers 0.
	printf("entrada inventada: handle %p, fnd_elem " UWORD_PRINT "\n",
		json::value(jsn, (void*)"sin_etiqueta"), json::fnd_elem(jsn, (void*)"sin_etiqueta"));
	// ESP: El indice pasado del ultimo tampoco esta: nullptr en lugar de element.
	// ENG: An index past the last one is not there either: nullptr instead of an element.
	void* lista = json::value(jsn, (void*)"lista");
	printf("matriz de " UWORD_PRINT " elementos, indice 9 -> %p\n",
		json::size(lista), json::value(lista, (void*)9));
	json::detach(lista);

	// ESP: El negativo del texto se guarda en complemento a dos: el cast directo saca el numero.
	// Antes el parser dejaba el signo arriba y la magnitud limpia: habia que enmascarar y negar.
	// ENG: The negative from the text is kept in two's complement: a plain cast takes the number out.
	// The parser used to leave the sign on top and a clean magnitude: it took masking and negating.
	void* negativo = json::value(jsn, (void*)"negativo");
	WORD leido = (WORD)(UWORD)json::value(negativo, nullptr);
	printf("negativo leido: " WORD_PRINT "\n", leido);
	json::detach(negativo);
	// ESP: Coincidir con -42 es lo que antes no pasaba: si el parser vuelve a ir por su cuenta, el ejemplo falla.
	// ENG: Matching -42 is what did not happen before: if the parser drifts apart again, this example fails.
	if(leido != -42){ puts("el negativo no coincide / the negative does not match"); return 1; }

	// ESP: value_view arma el handle dentro del buffer propio: el usuario lo pone y lo quita el stack.
	// buffer_size trae por delante cuantos bytes hacen falta: la peticion se consulta antes de reservar.
	// ENG: value_view builds the handle inside the own buffer: the user lays it out and the stack takes it back.
	// buffer_size comes first with how many bytes are needed: the ask gets queried before reserving.
	UWORD need = 0;
	void* probe = json::value_view(jsn, (void*)"hijo", nullptr, &need);
	printf("bytes de vista necesarios: " UWORD_PRINT ", retorno %p\n", need, probe);
	BYTE view[64];
	void* hijo = json::value_view(jsn, (void*)"hijo", view, &need);
	printf("vista: handle %p tipo 0x%02X entradas " UWORD_PRINT "\n",
		hijo, (unsigned)(hijo ? json::type(hijo) : 0), json::size(hijo));
	if(hijo){
		// ESP: value sobre una entrada de objeto entrega nodo: hay que pedirle el escalarcito al nodo.
		// Leelo directo como char* sacaria la firma del handle, no el texto.
		// ENG: value over an object entry delivers a node: the scalar has to be asked from that node.
		// Reading it straight as a char* would give the handle signature, not the text.
		void* b_node = json::value(hijo, (void*)"b");
		printf("  hijo.b: [%s]\n", b_node ? (const char*)json::value(b_node, nullptr) : "");
		json::detach(b_node);
		// ESP: detach sobre una vista no libera nada: detecta el buffer prestado y lo deja estar.
		// ENG: detach over a view releases nothing: it spots the borrowed buffer and lets it be.
		json::detach(hijo);
	}

	// ESP: parse tambien admite un handle de fichero abierto: saca el texto de alli.
	// ENG: parse also takes an open file handle: it pulls the text out of there.
	void* src = file::open(DEMO_FILE, file_mode::write | file_mode::create);
	if(src){
		UWORD text_size = 0; while(DEMO_TEXT[text_size]) ++text_size;
		file::write(src, DEMO_TEXT, text_size);
		file::close(src);
		src = file::open(DEMO_FILE, file_mode::read);
		if(src){
			void* from_file = json::parse(src);
			printf("parse desde fichero: tipo 0x%02X entradas " UWORD_PRINT "\n",
				(unsigned)json::type(from_file), json::size(from_file));
			json::detach(from_file);
			file::close(src);
		}
	}
	file::remove(DEMO_FILE);

	// ESP: La raiz es memoria del modulo: detach libera el buffer y el handle a la vez.
	// ENG: The root is module memory: detach frees the buffer and the handle at once.
	void* detached = json::detach(jsn);
	printf("detach de la raiz: %s\n", detached ? "liberado" : "rechazado");

	return 0;
}
