#include "hash.h"
#include <iostream>
#include <stdexcept>

using namespace std;


/* ****
* float retornarFactorDeCarga
******
* Devuelve el factor de carga actual de la tabla hash, que indica la proporción de posiciones ocupadas respecto al tamaño total.
******
* Returns:
* float, el factor de carga calculado como el número de elementos ocupados dividido por el tamaño de la tabla hash.
**** */
float Registro::retornarFactorDeCarga() {
    return factor_de_carga;
}

////-------------------------------Registro------------------------


Registro::Registro(int size) : size(size), ganancias(0), pedido_actual(nullptr) {
    pedidos = new Pedido[size](); //pasar el size de int a size_t?? creo que no es necesario
}

Registro::~Registro() {
    // for (int i = 0; i < size; i++) {
    //     delete pedidos[i]; // Libera cada pedido
    // }
    delete[] pedidos; // Libera el arreglo
}
/* ****
* int hash
******
* Calcula el índice hash para almacenar o buscar un pedido en la tabla hash.
******
* Input:
* int n_mesa: Número de la mesa o ID asociado al pedido.
* bool tipo_pedido: `true` si el pedido es para servir, `false` si es para llevar.
******
* Returns:
* int, índice hash calculado basado en el número de mesa y tipo de pedido.
**** */

int Registro::hash(int n_mesa, bool tipo_pedido){ // adaptado del codigo anterior
    int n=0;
    if(tipo_pedido==false){
        n=1;
    }
    int hash_value = 0;
    hash_value = (hash_value + n_mesa * 31 + n) % size;
    return hash_value;
};
/* ****
* int p
******
* Función de sondeo cuadrático para manejar colisiones en la tabla hash.
******
* Input:
* int i: Número de iteración en el proceso de sondeo.
******
* Returns:
* int, el valor de desplazamiento basado en el número de iteración.
**** */
int Registro::p(int i){
    //return (i*i) % size;
    return (i); // SOLUCION TEMPORAL aunque funcional
}

void Registro::agregar_pedido(Pedido* pedido) {
    int n_mesa = pedido->getMesa();
    bool servir = pedido->getTipo();

    int pos_inicial = hash(n_mesa, servir);

    size_t i = 0;
    
    // Manejo de colisiones
    while (pedidos[(pos_inicial + p(i)) % size].getMesa() != 0) { //si es que la casilla está ocupada...
        i++; //seguimos avanzando

        if (i >= size) {
            throw runtime_error("Tabla de hashing llena. Considera redimensionar.");
        }
    }
    
    pedidos[(pos_inicial + p(i)) % size] = *pedido; // 
}
Pedido* Registro::eliminar_pedido(int id, bool tipo) {
    // Calcular la posición inicial usando hash
    int pos_inicial = hash(id, tipo);
    size_t i = 0;

    // Búsqueda en la tabla hash
    while (pedidos[(pos_inicial + p(i)) % size].getMesa() != 0) { // Si la posición está ocupada
        Pedido& pedido = pedidos[(pos_inicial + p(i)) % size];
        if (pedido.getMesa() == id && pedido.getTipo() == tipo) {
            // Pedido encontrado
            int total = pedido.precio_total();
            ganancias += total + total * 0.1; // Agregar ganancias con propina

            // Marcar el pedido como eliminado
            pedido.setMesa(0); // Marcar mesa como vacía
            pedido.setTipo(false); // Resetear tipo

            return &pedido; // Retornar el pedido eliminado
        }
        i++;
        if (i >= size) break; // Salir si revisamos toda la tabla
    }

    return nullptr; // Pedido no encontrado
}    
Pedido* Registro::get_pedido(int id, bool tipo) {
    // Calcular la posición inicial usando hash
    int pos_inicial = hash(id, tipo);
    size_t i = 0;

    // Búsqueda en la tabla hash
    while (pedidos[(pos_inicial + p(i)) % size].getMesa() != 0) { // Si la posición está ocupada
        Pedido& pedido = pedidos[(pos_inicial + p(i)) % size];
        if (pedido.getMesa() == id && pedido.getTipo() == tipo) {
            return &pedido; // Retornar el pedido encontrado
        }
        i++;
        if (i >= size) break; // Salir si se revisa toda la tabla sin encontrar
    }

    return nullptr; // Pedido no encontrado
}
/* ****
* Pedido* get_pedido_actual
******
* Devuelve el pedido que actualmente está activo en el sistema.
******
* Returns:
* Pedido*, puntero al pedido actual o `nullptr` si no hay ninguno activo.
**** */
Pedido* Registro::get_pedido_actual() {
    return pedido_actual; // Retorna el puntero al pedido actual o nullptr si no hay ninguno
}
/* ****
* void set_pedido_actual
******
* Establece el pedido actual en el sistema.
******
* Input:
* Pedido* pedido: Puntero al pedido que será marcado como el actual.
******
* Returns:
* void, no retorna valores.
**** */
void Registro::set_pedido_actual(Pedido* pedido) {
    pedido_actual = pedido; // Establece el pedido actual
}
/* ****
* void Platos_Precios
******
* Muestra los nombres de los platos y el precio total de un pedido.
******
* Input:
* int id: Identificador del pedido (número de mesa o ID).
* bool tipo_pedido: `true` si el pedido es para servir, `false` si es para llevar.
******
* Returns:
* void, no retorna valores.
* Muestra la información directamente en la consola.
**** */

void Registro::Platos_Precios(int id, bool tipo_pedido) {
    // Obtener el pedido usando el método existente
    Pedido* pedido = get_pedido(id, tipo_pedido);

    // Verificar si el pedido existe
    if (pedido == nullptr) {
        cerr << "Error: Pedido no encontrado." << endl;
        return;
    }

    // Mostrar los nombres de los platos
    cout << "Platos del pedido:" << endl;
    for (int i = 0; i < 25; ++i) { // Usar el tamaño fijo del arreglo
        cout << "- " << pedido->getPlatos()[i].nombre << endl;
    }

    // Mostrar el precio total del pedido
    int total = pedido->precio_total(); // Calcular el precio total una vez
    cout << "Total del pedido: " << total << " pesos" << endl;
}
/* ****
* Plato* getPlatos
******
* Devuelve un puntero al arreglo de platos asociados al pedido.
******
* Returns:
* Plato*, puntero al arreglo de platos en el pedido.
**** */

Plato* Pedido::getPlatos(){
    return platos;
}


//-------------------------------Pedido--------------------------

Pedido::Pedido() {
    platos = new Plato[25];
    servir = false;
    cant_platos = 0;
    n_mesa = 0 ; //valor por default, que representa que no hay nada agregado
    clave=-1;
}

Pedido::~Pedido() { 
    delete[] platos; 
}
void Pedido::agregar_plato(Plato* plato) {
    // Verificar si hay espacio disponible para agregar más platos
    if (cant_platos < 25) {
        platos[cant_platos] = *plato; // Copiar el plato al arreglo
        cant_platos++;               // Incrementar el contador de platos
        cout << "Plato \"" << plato->nombre << "\" agregado al pedido." << endl;
    } else {
        // Si el pedido está lleno, se lanza una excepción
        throw runtime_error("No se pueden agregar más platos al pedido. Capacidad máxima alcanzada.");
    }
}


int Pedido::precio_total() {
    int total = 0;

    // Sumar los precios de todos los platos en el pedido
    for (size_t i = 0; i < cant_platos; ++i) {
        total += platos[i].precio;
    }

    return total;
}
/* ****
* void setMesa
******
* Asigna un número de mesa al pedido.
******
* Input:
* int mesa: Número de la mesa a asignar.
******
* Returns:
* void, no retorna valores.
**** */

void Pedido::setMesa(int mesa){
    n_mesa = mesa;
};   

/* ****
* int getMesa
******
* Devuelve el número de mesa asociado al pedido.
******
* Returns:
* int, el número de mesa asignado al pedido. Si no se ha asignado, retorna 0 por defecto.
**** */

int Pedido::getMesa(){
    return n_mesa;
};        

/* ****
* void setTipo
******
* Asigna el tipo de pedido, indicando si es para servir o para llevar.
******
* Input:
* bool tipo: `true` si el pedido es para servir, `false` si es para llevar.
******
* Returns:
* void, no retorna valores.
**** */

void Pedido::setTipo(bool tipo){
    servir = tipo;
}     // .

/* ****
* bool getTipo
******
* Devuelve el tipo de pedido.
******
* Returns:
* bool, `true` si el pedido es para servir, `false` si es para llevar.
**** */

bool Pedido::getTipo(){
    return servir;
};        
