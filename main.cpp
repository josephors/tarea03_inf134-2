#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "hash.h"

using namespace std;

void procesarLinea(string linea, Registro &registro_pedidos){
    //para cada línea que leamos, generaremos un Pedido??
    //cout << linea << endl;
}

int main() {
    //nombre del archivo a leer
    string nombre_archivo = "Menu.txt";

    //abrir el archivo
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
    
    Registro registro_pedidos; //se crea registro

    Plato* menu = new Plato[cant_lineas];

    //leer el archivo línea por línea
    int i=0;
    while (getline(file, linea)) {
        //guardamos el menu existente en un arreglo
        string nombre_plato;
        string precio_plato;
        bool cambio=false;

        for(int j=0; j<linea.length(); j++){
            if(linea[j]=='-'){
                cambio=true;
                j+=1;
            }
            if(!cambio){
                nombre_plato+=linea[j];
            } else {
                precio_plato+=linea[j];
            }
        }

        menu[i].nombre = nombre_plato;
        menu[i].precio = stoi(precio_plato);

        //procesarLinea(linea, registro_pedidos); //procesarLinea tiene que ir procesando los comandos, no el texto del ".txt"

        i++;
    }

    

    //cerrar el archivo
    file.close();

    //ejecutar todos los destructores
    delete[] menu;

    return 0;
}
