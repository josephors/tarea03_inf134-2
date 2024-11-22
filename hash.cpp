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
