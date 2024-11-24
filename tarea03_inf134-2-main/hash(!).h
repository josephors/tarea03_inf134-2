#ifndef HASH_H
#define HASH_H

#include <string>

using namespace std;

struct cuenta {
    // El rol es el identificador de la persona
	// El nombre y la descripcion son valores asociados al rol
    string rol, nombre, descripcion;
};

class registro_cuentas {
    private:
        float factor_de_carga = 0.0;
        cuenta* tabla; // Aca se almacenaran los elementos de la tabla
        int ranuras = 15; // Cuantas ranuras tiene la tabla hash (inicialmente)
        int ocupadas = 0; // Cuántas ranuras están ocupada
        int p(string rol, int i); // Se obtiene la ranura a revisar en caso de colisión dado el rol y el intento i
        int hash(string rol); // Se obtiene el hash dado el rol
    public:
        registro_cuentas(); // Constructor por defecto
        ~registro_cuentas(); // Destructor
        cuenta obtener(string rol); // Dado el rol, devuelve la cuenta con ese rol
        void agregar(cuenta c); // Se agrega una cuenta a la tabla
        void eliminar(string rol); // Se elimina la cuenta
        void modificar(string rol, string descripcion); // Se modifica la descripcion del rol
        void redimensionar(int n); // Se redimensiona la tabla a n espacios
        void estadisticas(); // Debe mostrar las estadisticas

        void actualizarStats(char tipoUpdate);
};

#endif // HASH_H
