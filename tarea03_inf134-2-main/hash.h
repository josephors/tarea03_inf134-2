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
        Plato* platos; //arreglo de platos en el pedido, tamaño inicial 25. 
        bool servir; //true para servir, false para llevar.
        size_t cant_platos;
        int n_mesa; // sirve para identificar la mesa, lo que facilita el hashing *AGREGADO
    public :
        Pedido ();
        ~ Pedido ();
        void agregar_plato (Plato* plato); // agrega un plato al pedido.
        int precio_total(); // retorna la suma del precio de todos los platos del pedido.
        void setMesa(int mesa);      // Asigna un número de mesa al pedido.
        int getMesa();         // Devuelve el número de mesa.
        void setTipo(bool tipo);     // Asigna el tipo de pedido (servir/llevar).
        bool getTipo();        // Devuelve el tipo de pedido.
};

class Registro {
    private :
        Pedido* pedidos; //arreglo de pedidos, tamaño inicial n (cantidad de mesas). Tabla de hashing.
        size_t size;
        void ajustar_arreglo(); //ajusta el tamaño de la tabla de hashing.
        int ganancias;
        int hash(int n_mesa, bool tipo_pedido);
        int p(int i);
        Pedido* pedido_actual; // Pedido en curso
    public :
        Registro (int size);
        ~ Registro ();
        //void RegistroAux(int n_mesas);
        void agregar_pedido (Pedido* pedido); //agrega el pedido a la tabla de hashing.
        Pedido * get_pedido (int id, bool tipo); //Retorna el pedido según id y tipo (servir true, llevar false).
        Pedido * eliminar_pedido (int id, bool tipo); //Elimina el pedido según id y tipo.
        Pedido* get_pedido_actual(); // Método para obtener el pedido actual
        void set_pedido_actual(Pedido* pedido); // Método para establecer el pedido actual
};

#endif // HASH_H
