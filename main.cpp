#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "hash.h"

using namespace std;

void procesarLinea(string linea, Registro &registro_pedidos){
    cout << linea << endl;
}

int main() {
    // Nombre del archivo a leer
    string nombre_archivo = "Menu.txt";

    // Abrir el archivo
    ifstream file(nombre_archivo);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombre_archivo << endl;
        return 1;
    }

    //creamos "entorno" hashing
    string linea;
    getline(file, linea); // numero de platos

    int cant_lineas = stoi(linea);
    cout << cant_lineas << endl;
    
    Registro registro_pedidos;// se crea registro

    // Leer el archivo línea por línea
    while (getline(file, linea)) {
        procesarLinea(linea, registro_pedidos);
    }

    // Cerrar el archivo
    file.close();

    //ejecutar destructores
    //

    return 0;
}
