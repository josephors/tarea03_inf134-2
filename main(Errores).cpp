#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "hash.h"

using namespace std;

void procesarLinea(string linea, Registro &registro_pedidos){
    //para cada línea que leamos, generaremos un Pedido??
    //cout << linea << endl;
    istringstream iss(linea); // iss es de sstream, sirve para manejar mejor las palabras en una linea
    string comando;
    iss >> comando; // la primera palabra de una linea siempre va a ser el comando
    
    // la primera linea que se lea va a ser el n de mesas, por lo que se inicializara en el else 


    
    if (comando == "registrar") {
        string tipo_pedido;
        int numero_mesa;
        iss >> tipo_pedido>> numero_mesa;

    } else if (comando == "agregar") {
        string nombre_plato;
        iss >> nombre_plato;
          
        // aqui por la mesa correspondiente se creara un arreglo de platos
    } else if (comando == "info") {
        // mostrara la info de la mesa
    } else if (comando == "pedir") {
        //pedir finalizara el pedido, ira siempre despues de un agregar
    } else if (comando == "pagar") {
        // finalizara el pedido y se borrara del registro?
    }else if (comando == "cerrar") {
        // muestra el total de lo ganado con la propina incluida
        
    }
    
};


int main() {
    //nombre del archivo a leer
    string nombre_archivo = "Menu.txt";
    int n_mesas;

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
    
    Registro registro_pedidos;  
    cin>>n_mesas; // primer input n_mesas = size
    registro_pedidos.RegistroAux(n_mesas);

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
