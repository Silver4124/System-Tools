/* // ESP
	Ejemplo de la construccion de un JSON desde cero y sus ediciones.
	Muestra: json::init, json::add_elem, json::set_value, json::set_type, json::del_elem,
	json::swp_elem, json::mve_elem, json::cls_elms y json::detach sobre un objeto nuevo.
	Compilar: g++ build_object.cpp -o build_object
	Requiere: #define ST_IMPORT_JSON antes de incluir la cabecera.
	// ENG
	Example of building a JSON from scratch and its edits.
	Shows: json::init, json::add_elem, json::set_value, json::set_type, json::del_elem,
	json::swp_elem, json::mve_elem, json::cls_elms and json::detach over a brand new object.
	Build: g++ build_object.cpp -o build_object
	Needs: #define ST_IMPORT_JSON before including the header.
*/
#define ST_IMPORT_JSON
#include "System_Tools.h"
using namespace System_Tools;

// ESP: Helper propio: describe el nodo por fuera y lo cierra: mirar sin dejar handles pendientes.
// ENG: Own helper: describes the node from outside and shuts it: a peek that leaves no handles hanging.
static void tell_node(void* node){
	BYTE kind = json::type(node);
	void* raw = json::value(node, nullptr);
	printf(" tipo 0x%02X:", (unsigned)kind);
	if(kind == json_type::str) printf(" [%s] de " UWORD_PRINT, (const char*)raw, json::size(node));
	else if(kind == json_type::num) printf(" " WORD_PRINT, (WORD)raw);
	else if(kind == (json_type::num | json_type::uns)) printf(" " UWORD_PRINT, (UWORD)raw);
	else if(kind == json_type::flt) printf(" real %g", *(float*)raw);
	else if((kind & 0x0F) == json_type::bll) printf(" booleano %d", (int)(UWORD)raw);
	else if(kind == json_type::nll) printf(" nulo");
	else if(kind == json_type::ary) printf(" array de " UWORD_PRINT, json::size(node));
	else if(kind == json_type::obj) printf(" objeto de " UWORD_PRINT, json::size(node));
	// ESP: Un handle inexistente y un false traen el mismo nullptr: el tipo es el que los separa.
	// ENG: A missing handle and a false carry the same nullptr: it is the type that splits them.
	else printf(" sin lectura");
	json::detach(node);
}

// ESP: Helper propio: abre la entrada nombrada de la raiz, la cuenta y la suelta.
// ENG: Own helper: opens the root named entry, reads it out and lets it go.
static void peek(void* root, const char* name){
	printf("  %-8s", name);
	tell_node(json::value(root, (void*)name));
	puts("");
}

// ESP: Helper propio: muestra los elementos de una matriz segun su tipo real.
// ENG: Own helper: shows an array elements following their real type.
static void show_array(void* arr, const char* note){
	printf("%s: " UWORD_PRINT " elementos", note, json::size(arr));
	for(UWORD i = 0; i < json::size(arr); ++i){
		printf("\n  [" UWORD_PRINT "]", i);
		tell_node(json::value(arr, (void*)i));
	}
	puts("");
}

// ESP: Helper propio: pone un escalar dentro de una entrada que ya existe.
// El handle del hijo se pide y se suelta aqui: entre ediciones no conviene guardarlo.
// ENG: Own helper: puts a scalar inside an entry that already exists.
// The child handle gets asked and released right here: between edits it is best not kept.
static void set_member(void* root, const char* name, void* value){
	void* node = json::value(root, (void*)name);
	json::set_value(node, value);
	json::detach(node);
}

int main(){
	// ESP: init arma el nodo vacio del tipo pedido: los contenedores nacen sin entradas.
	// La raiz es memoria del modulo: detach al final.
	// ENG: init lays out the empty node of the asked type: containers are born with no entries.
	// The root is module memory: detach at the end.
	void* root = json::init(json_type::obj);
	if(!root){ puts("no se pudo iniciar el objeto / object could not be initiated"); return 1; }
	printf("inicio: tipo 0x%02X entradas " UWORD_PRINT "\n",
		(unsigned)json::type(root), json::size(root));

	// ESP: add_elem pide el nombre como puntero a texto y el tipo del valor nuevo.
	// El valor nace limpio: hay que pasarle set_value para que tenga algo dentro.
	// ENG: add_elem asks the name as a text pointer and the type of the new value.
	// The value is born clean: it needs a set_value pass to hold anything inside.
	json::add_elem(root, (void*)"nombre", json_type::str);
	set_member(root, "nombre", (void*)"sistema");
	peek(root, "nombre");

	// ESP: Los enteros van empaquetados dentro del puntero que set_value recibe.
	// Por la via de la escritura el negativo sale en complemento a dos: el cast a WORD lo lee derecho.
	// ENG: Integers go packed inside the pointer set_value receives.
	// Through the writing road the negative comes out in two's complement: a cast to WORD reads it straight.
	json::add_elem(root, (void*)"cuenta", json_type::num);
	set_member(root, "cuenta", (void*)(UWORD)(WORD)-42);
	peek(root, "cuenta");

	// ESP: Un real entra con los bits del float dentro del puntero: se copia la huella, no el numero.
	// set_value lee los cuatro primeros bytes del void*: ahi van los del float.
	// ENG: A float comes in with the float bits inside the pointer: the footprint gets copied, not the number.
	// set_value reads the first four bytes of the void*: that is where the float ones go.
	json::add_elem(root, (void*)"real", json_type::flt);
	float medida = 2.5f;
	void* bits = nullptr;
	memory::clloc(&bits, &medida, sizeof(float));
	set_member(root, "real", bits);
	peek(root, "real");

	// ESP: set_value sobre un booleano le cambia el tipo: 0 deja bll y cualquier otro deja bll con el flag uns.
	// set_value sobre un nulo no tiene donde escribir: responde la misma direccion sin tocar nada.
	// ENG: set_value on a boolean swaps its type: 0 leaves bll and any other leaves bll with the uns flag.
	// set_value on a null has nowhere to write: it answers the same address without touching anything.
	json::add_elem(root, (void*)"bandera", json_type::bll);
	json::add_elem(root, (void*)"vacio", json_type::nll);
	set_member(root, "bandera", (void*)1);
	peek(root, "bandera");
	peek(root, "vacio");

	// ESP: Una matriz se arma por indice: add_elem con el indice donde debe caer el nuevo.
	// El indice igual al conteo apendra al final; pasarse de largo tambien apendra.
	// Cada set_value del elemento vuelve a pedir el handle: tras una edicion la direccion vieja es historia.
	// ENG: An array gets built by index: add_elem with the index where the new one must land.
	// An index equal to the count appends at the end; overshooting appends too.
	// Every element set_value asks for the handle again: after an edit the old address is history.
	json::add_elem(root, (void*)"lista", json_type::ary);
	void* lista = json::value(root, (void*)"lista");
	json::add_elem(lista, (void*)0, json_type::num | json_type::uns);
	void* cero = json::value(lista, (void*)0);
	json::set_value(cero, (void*)(UWORD)10);
	json::detach(cero);
	json::add_elem(lista, (void*)1, json_type::num | json_type::uns);
	cero = json::value(lista, (void*)1);
	json::set_value(cero, (void*)(UWORD)20);
	json::detach(cero);
	// ESP: Un indice mucho mayor que el conteo cae al final: add_elem lo recorta sin avisar.
	// ENG: An index far past the count lands at the end: add_elem trims it without warning.
	json::add_elem(lista, (void*)99, json_type::num | json_type::uns);
	cero = json::value(lista, (void*)2);
	json::set_value(cero, (void*)(UWORD)30);
	json::detach(cero);
	show_array(lista, "matriz nueva");

	// ESP: Insertar en medio desplaza: el indice 0 mete el nuevo delante del primero.
	// ENG: Inserting in the middle shifts: index 0 puts the new one in front of the first.
	json::add_elem(lista, (void*)0, json_type::str);
	cero = json::value(lista, (void*)0);
	json::set_value(cero, (void*)"adelante");
	json::detach(cero);
	show_array(lista, "matriz con insercion");
	json::detach(lista);

	// ESP: Un nombre repetido se rechaza con nullptr: el objeto conserva la entrada original.
	// ENG: A repeated name gets refused with nullptr: the object keeps the original entry.
	printf("nombre repetido: %s\n", json::add_elem(root, (void*)"nombre", json_type::str) ? "aceptado" : "rechazado");

	// ESP: add_elem sobre un escalar no es un fallo: responde la misma direccion sin anadir nada.
	// ENG: add_elem over a scalar is not a failure: it answers the same address without adding anything.
	void* hoja = json::init(json_type::str);
	printf("add_elem sobre cadena: %s, tamano " UWORD_PRINT "\n",
		json::add_elem(hoja, (void*)"algo", json_type::num) == hoja ? "misma direccion" : "otra",
		json::size(hoja));
	json::detach(hoja);

	// ESP: set_type convierte el contenido, no solo la etiqueta: aqui numero a texto y de vuelta.
	// El salto a texto usa el formato de impresion propio: el negativo queda escrito con su guion.
	// El regreso vuelve a leer ese guion: la ida y vuelta sale exacta con el numero firmado.
	// ENG: set_type converts the content, not just the tag: number into text and back here.
	// The jump into text uses its own printing format: the negative gets written with its hyphen.
	// The way back reads that hyphen again: the round trip comes out exact with the signed number.
	void* cuenta = json::value(root, (void*)"cuenta");
	json::set_type(cuenta, json_type::str);
	json::detach(cuenta);
	peek(root, "cuenta");

	// ESP: El texto "-42" pesa tres bytes y el numero firmado pesa lo que la palabra del procesador:
	// expand hace el hueco, y la cuenta se arma digito a digito con el guion aparte.
	// Despues se repite la pierna con un positivo: el mismo camino, sin guion que levantar.
	// ENG: The text "-42" weighs three bytes and the signed number weighs what the processor word is:
	// expand makes the room, and the count gets built digit by digit with the hyphen kept apart.
	// Then the same leg runs with a positive: the same road, with no hyphen to lift.
	cuenta = json::value(root, (void*)"cuenta");
	json::set_type(cuenta, json_type::num);
	json::detach(cuenta);
	peek(root, "cuenta");
	set_member(root, "cuenta", (void*)(UWORD)42);
	cuenta = json::value(root, (void*)"cuenta");
	json::set_type(cuenta, json_type::str);
	json::detach(cuenta);
	cuenta = json::value(root, (void*)"cuenta");
	json::set_type(cuenta, json_type::num);
	json::detach(cuenta);
	peek(root, "cuenta");

	// ESP: Texto con decimales a num: la cifra se trunca al entero, no se entregan los bits del float.
	// "0.5" escrito como cadena acaba en un num que vale 0; "-2.9e2" acaba en -290.
	// ENG: Decimal text into num: the figure gets truncated to the integer, the float bits are not handed over.
	// "0.5" written as a chain ends in a num worth 0; "-2.9e2" ends in -290.
	json::add_elem(root, (void*)"media", json_type::str);
	json::add_elem(root, (void*)"salto", json_type::str);
	set_member(root, "media", (void*)"0.5");
	set_member(root, "salto", (void*)"-2.9e2");
	void* decimal = json::value(root, (void*)"media");
	json::set_type(decimal, json_type::num);
	json::detach(decimal);
	decimal = json::value(root, (void*)"salto");
	json::set_type(decimal, json_type::num);
	json::detach(decimal);
	peek(root, "media");
	peek(root, "salto");
	// ESP: Las dos cifras valen lo que valen: si la conversion entregara la huella del float, el ejemplo sale con 1.
	// ENG: Both figures are worth what they are worth: were the conversion handing the float footprint, the example leaves with 1.
	decimal = json::value(root, (void*)"media");
	WORD media_valor = (WORD)(UWORD)json::value(decimal, nullptr);
	json::detach(decimal);
	decimal = json::value(root, (void*)"salto");
	WORD salto_valor = (WORD)(UWORD)json::value(decimal, nullptr);
	json::detach(decimal);
	if(media_valor != 0 || salto_valor != -290){
		puts("conversion de decimal a entero fallida / decimal to integer conversion failed");
		return 1;
	}

	// ESP: swp_elem trueca dos puestos: en una matriz los valores cambian de indice.
	// ENG: swp_elem trades two slots: in an array the values change their index.
	lista = json::value(root, (void*)"lista");
	json::swp_elem(lista, (void*)0, (void*)2);
	show_array(lista, "matriz con swp_elem");

	// ESP: mve_elem en una matriz rota el orden hacia la otra punta: los demas se corren.
	// ENG: mve_elem in an array rotates the order towards the other end: the rest get shifted.
	json::mve_elem(lista, (void*)0, (void*)2);
	show_array(lista, "matriz con mve_elem");
	json::detach(lista);

	// ESP: mve_elem sobre un objeto cambia el nombre de la entrada, no el puesto.
	// Un destino ya ocupado se rechaza; uno libre se acepta y el contenido no se mueve.
	// El tercer destino libre mide otro tanto que el origen: la etiqueta se reescribe y el buffer se ajusta.
	// ENG: mve_elem over an object changes the entry name, not the slot.
	// A taken destination gets refused; a free one gets accepted and the content does not move.
	// The third free destination measures another amount than the source: the label gets rewritten and the buffer adjusts.
	printf("mve a nombre ocupado: %s\n", json::mve_elem(root, (void*)"nombre", (void*)"cuenta") ? "aceptado" : "rechazado");
	printf("mve a nombre libre: %s\n", json::mve_elem(root, (void*)"cuenta", (void*)"saldo2") ? "aceptado" : "rechazado");
	printf("mve a nombre mas largo: %s\n", json::mve_elem(root, (void*)"nombre", (void*)"nombre_largo") ? "aceptado" : "rechazado");

	// ESP: Tras el rename el nombre viejo se pierde: value de una entrada inexistente entrega nullptr.
	// El helper lo cuenta como tipo 0xFF sin lectura: no es un fallo, es la ausencia.
	// El contenido sobrevive al cambio de medida: la cadena sigue valiendo "sistema" bajo la etiqueta nueva.
	// ENG: After the rename the old name is gone: value of a missing entry hands over nullptr.
	// The helper counts it as type 0xFF without reading: it is not a failure, it is the absence.
	// The content survives the change of measure: the chain still reads "sistema" under the new label.
	peek(root, "nombre");
	peek(root, "cuenta");
	peek(root, "saldo2");
	peek(root, "nombre_largo");
	// ESP: Y el buffer sigue entero despues del agrandado: las entradas vecinas se siguen leyendo igual.
	// ENG: And the buffer stays whole after the growing: the neighbour entries still read the same.
	peek(root, "real");

	// ESP: del_elem quita la entrada y reordena: en objeto la etiqueta desaparece con su valor.
	// ENG: del_elem takes the entry away and reorders: in an object the label vanishes with its value.
	json::del_elem(root, (void*)"saldo2");
	printf("entradas tras del_elem: " UWORD_PRINT "\n", json::size(root));
	peek(root, "saldo2");

	// ESP: fnd_elem y nme_elem siguen sirviendo sobre lo construido a mano: 1 basado y 0 basado.
	// ENG: fnd_elem and nme_elem still work over the hand built stuff: 1-based and 0-based.
	printf("buscado 'real': posicion " UWORD_PRINT ", nombre en 3: %s\n",
		json::fnd_elem(root, (void*)"real"), json::nme_elem(root, 3));

	// ESP: cls_elms vacia el objeto sin borrar el objeto: se queda el tipo y las entradas a cero.
	// ENG: cls_elms empties the object without deleting the object: the type stays and the entries go to zero.
	json::cls_elms(root);
	printf("entradas tras cls_elms: " UWORD_PRINT ", tipo 0x%02X\n",
		json::size(root), (unsigned)json::type(root));

	// ESP: detach sobre un puntero que no es JSON responde nullptr sin tocar memoria ajena.
	// ENG: detach over a pointer that is no JSON answers nullptr without touching foreign memory.
	UWORD stray = 0x11223344;
	printf("detach de un puntero suelto: %p\n", json::detach(&stray));

	// ESP: La raiz se devuelve con detach: libera el buffer del modulo y el handle a la vez.
	// ENG: The root goes back through detach: it frees the module buffer and the handle at once.
	if(json::detach(root)) puts("raiz liberada / root released");

	return 0;
}
