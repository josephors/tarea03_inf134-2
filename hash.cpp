#include "hash.h"
#include <iostream>
#include <stdexcept>

using namespace std;

////-------------------------------Registro------------------------


Registro::Registro(int size) : size(size), ganancias(0), pedido_actual(nullptr), factor_de_carga(0.0), cant_pedidos(0) {
    pedidos = new Pedido[size](); //pasar el size de int a size_t?? creo que no es necesario
}

Registro::~Registro() {
    // for (int i = 0; i < size; i++) {
    //     delete pedidos[i]; // Libera cada pedido
    // }
    delete[] pedidos; // Libera el arreglo
}


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
    return (i*i)%size; // SOLUCION TEMPORAL aunque funcional
}

void Registro::agregar_pedido(Pedido* pedido) {
    int n_mesa = pedido->getMesa();
    bool servir = pedido->getTipo();

    int pos_inicial = hash(n_mesa, servir);

    size_t i = 0;
    // int inicio = 0;

    // for(i=1; pedidos[pos].clave != -1; i++){
    //     pos = (inicio + p(i)) % size;
    // }

    // if (pedidos[pos].clave == k)
    //     return 0; // inserción no exitosa: clave repetida
    // else {
    //     HT[pos].clave = k;
    //     HT[pos].info = I;
    //     return 1; // inserción exitosa
    // }


    // Manejo de colisiones
    while (pedidos[(pos_inicial + p(i)) % size].getMesa() != 0) { //si es que la casilla está ocupada...
        i++; //seguimos avanzando

        if (i >= size) {
            throw runtime_error("Tabla de hashing llena. Considera redimensionar.");
        }
    }
    
    pedidos[(pos_inicial + p(i)) % size] = *pedido; //insertar pedido
    cant_pedidos+=1;
    factor_de_carga=cant_pedidos/size;
}

float Registro::retornarFactorDeCarga(){
    return factor_de_carga;
}

Pedido* Registro::eliminar_pedido(int id, bool tipo) {
    // Calcular la posición inicial usando hash
    int pos_inicial = hash(id, tipo);
    size_t i = 0;

    cout << "--------" << endl;
    cout << pos_inicial << endl;

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
Pedido* Registro::get_pedido_actual() {
    return pedido_actual; // Retorna el puntero al pedido actual o nullptr si no hay ninguno
}
void Registro::set_pedido_actual(Pedido* pedido) {
    pedido_actual = pedido; // Establece el pedido actual
}

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
        if(pedido->getPlatos()[i].nombre.length()>0){
            cout << "- " << pedido->getPlatos()[i].nombre << endl;
        }
    }

    // Mostrar el precio total del pedido
    int total = pedido->precio_total(); // Calcular el precio total una vez
    cout << "Total del pedido: " << total << " pesos" << endl;
}

Plato* Pedido::getPlatos(){
    return platos;
}

//verificar si se están calculando bien los hash() y p()

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
        // cout << "Plato \"" << plato->nombre << "\" agregado al pedido." << endl;
    } else {
        // Si el pedido está lleno, se lanza una excepción
        throw runtime_error("No se pueden agregar más platos al pedido. Capacidad máxima alcanzada.");
    }
}


// } // agrega un plato al pedido.

int Pedido::precio_total() {
    int total = 0;

    // Sumar los precios de todos los platos en el pedido
    for (size_t i = 0; i < cant_platos; ++i) {
        total += platos[i].precio;
    }

    return total;
}

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
