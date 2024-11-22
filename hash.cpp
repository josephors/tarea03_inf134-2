#include "hash.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Pedido::Pedido() { 
    platos = new Plato[25];
    servir = false;
    cant_platos = 0;
}

Pedido::~Pedido() { 
    delete[] platos; 
}

Registro::Registro(){
    size=10;
    pedidos = new Pedido[size]();
}

Registro::~Registro() {
    delete[] pedidos;
}

int Registro::hash(int n_mesa, bool tipo_pedido){

};

int Registro::p(int i){

};

void Registro::agregar_pedido(Pedido* pedido){
    //generamos el índice para el pedido
    /*
    string nombre_pedido = pedido

    int pos_inicial, i;
    int pos_actual = hash();
    */

    //agregamos a tabla de hashing

}