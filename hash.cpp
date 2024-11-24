#include "hash.h"
#include <iostream>
#include <stdexcept>

using namespace std;

////-------------------------------Registro------------------------


// void Registro::RegistroAux(int n_mesas) { // SOLUCION TEMPORAL
//     size = n_mesas;
//     pedidos = new Pedido*[size]; // Cambiar a un arreglo de punteros
//     for (int i = 0; i < size; i++) {
//         pedidos[i] = nullptr; // Inicializar cada posición como nullptr
//     }
// }

Registro::Registro(int size) : size(size), ganancias(0) {
    pedidos = new Pedido[size](); //pasar el size de int a size_t?? creo que no es necesario
}

Registro::~Registro() {
    // for (int i = 0; i < size; i++) {
    //     delete pedidos[i]; // Libera cada pedido
    // }
    delete[] pedidos; // Libera el arreglo
}


// Pedido* Registro::get_pedido(int id, bool tipo){

// }

// Pedido* Registro::eliminar_pedido(int id, bool tipo) {

// }

int Registro::hash(int n_mesa, bool tipo_pedido){ // adaptado del codigo anterior
    int n=0;
    if(tipo_pedido==false){
        n=1;
    }
    int hash_value = 0;
    hash_value = (hash_value + n_mesa * 31 + n) % size;
    return hash_value;
};

int Registro::p(int i){
    //return (i*i) % size;
    return i*i; // SOLUCION TEMPORAL aunque funcional
}

void Registro::agregar_pedido(Pedido* pedido) {
    int n_mesa = pedido->getMesa();
    bool servir = pedido->getTipo();

    int pos_inicial = hash(n_mesa, servir);
    size_t i = 0;

    // Manejo de colisiones
    while (pedidos[(pos_inicial + p(i)) % size].getMesa() != 0) { //si es que la casilla está ocupada...
        i++;

        if (i >= size) {
            throw runtime_error("Tabla de hashing llena. Considera redimensionar.");
        }
    }
    
    // while (pedidos[(pos_inicial + p(i)) % size] != nullptr) { //si es que la casilla está ocupada...
    //     i++;

    //     if (i >= size) {
    //         throw runtime_error("Tabla de hashing llena. Considera redimensionar.");
    //     }
    // }

    pedidos[(pos_inicial + p(i)) % size] = *pedido; // Insertar pedido / por qué esto no funciona de por sí???
}
//verificar si se están calculando bien los hash() y p()

//-------------------------------Pedido--------------------------

Pedido::Pedido() { // esto tmb
    platos = new Plato[25];
    servir = false;
    cant_platos = 0;
    n_mesa = 0 ; // esto por medio
}

Pedido::~Pedido() { 
    delete[] platos; 
}

// void Pedido::agregar_plato (Plato* plato){

// } // agrega un plato al pedido.

// int Pedido::precio_total(){

// } // retorna la suma del precio de todos los platos del pedido.

void Pedido::setMesa(int mesa){
    n_mesa = mesa;
};    // Asigna un número de mesa al pedido.

int Pedido::getMesa(){
    return n_mesa;
};         // Devuelve el número de mesa.

void Pedido::setTipo(bool tipo){
    servir = tipo;
}     // Asigna el tipo de pedido (servir/llevar).

bool Pedido::getTipo(){
    return servir;
};        // Devuelve el tipo de pedido.
