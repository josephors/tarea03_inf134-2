#ifndef HASH_H
#define HASH_H

#include <string>

using namespace std;

struct Plato {
    string nombre;
    int precio;
};

class Pedido {
    private :
        Plato* platos; //arreglo de platos en el pedido, tamaño inicial 25. Lista enlazada.
        bool servir; //true para servir, false para llevar.
        size_t cant_platos;
    public :
        Pedido ();
        ~ Pedido ();
        void agregar_plato (Plato* plato); // agrega un plato al pedido.
        int precio_total(); // retorna la suma del precio de todos los platos del pedido.
};

class Registro {
    private :
        Pedido* pedidos; //arreglo de pedidos, tamaño inicial n (cantidad de mesas). Tabla de hashing.
        size_t size;
        void ajustar_arreglo(); //ajusta el tamaño de la tabla de hashing.
        int ganancias;

        int hash(int n_mesa, bool tipo_pedido);
        int p(int i);
    public :
        Registro ();
        ~ Registro ();
        void agregar_pedido (Pedido* pedido); //agrega el pedido a la tabla de hashing.
        Pedido * get_pedido (int id, bool tipo); //Retorna el pedido según id y tipo (servir true, llevar false).
        Pedido * eliminar_pedido (int id, bool tipo); //Elimina el pedido según id y tipo.

};

#endif // HASH_H