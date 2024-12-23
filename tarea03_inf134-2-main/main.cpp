#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "hash.h"

using namespace std;

void procesarLinea(string linea, Registro &registro_pedidos,Plato* menu, int menu_size){ 
    int id_llevar = 1;//se inicaliza desde 1
    istringstream iss(linea); // iss es de sstream, sirve para manejar mejor las palabras en una linea
    string comando;
    iss >> comando; // la primera palabra de una linea siempre va a ser el comando


    if (comando == "registrar") {
        string tipo_pedido;
        int numero_mesa;
        iss >> tipo_pedido>> numero_mesa;
        if(tipo_pedido=="mesa"){ // servir = True
            Pedido*nuevoPedido= new Pedido();
            nuevoPedido->setMesa(numero_mesa);
            nuevoPedido->setTipo(true);
            registro_pedidos.agregar_pedido(nuevoPedido); // aqui agregamos los detalles
            registro_pedidos.set_pedido_actual(nuevoPedido);
            cout<<nuevoPedido->getMesa()<<endl;
        }
        else{
            Pedido*nuevoPedido= new Pedido(); // servir = False
            nuevoPedido->setMesa(id_llevar); //tiene id secuencial 
            nuevoPedido->setTipo(false);
            registro_pedidos.agregar_pedido(nuevoPedido);
            registro_pedidos.set_pedido_actual(nuevoPedido);
            id_llevar++;
        }

    } else if (comando == "agregar") {
        string nombre_plato;
        iss >> nombre_plato;

        // Buscar el precio del plato en el menú
        int precio_plato = -1;
        for (int i = 0; i < menu_size; ++i) {
            if (menu[i].nombre == nombre_plato) {
                precio_plato = menu[i].precio;
                break;
            }
        }

        if (precio_plato == -1) {
            cerr << "Error: El plato \"" << nombre_plato << "\" no existe en el menú." << endl;
            return;
        }

        // Obtener el pedido actual desde el registro
        Pedido* pedido_actual = registro_pedidos.get_pedido_actual();
        if (pedido_actual == nullptr) {
            cerr << "Error: No hay un pedido activo para agregar platos." << endl;
            return;
        }

        // Crear el plato y agregarlo al pedido actual
        Plato nuevo_plato = {nombre_plato, precio_plato};
        try {
            pedido_actual->agregar_plato(&nuevo_plato);
            cout << "Plato \"" << nombre_plato << "\" agregado al pedido actual con precio " << precio_plato << " pesos." << endl;
        } catch (const runtime_error& e) {
            cerr << "Error al agregar plato: " << e.what() << endl;
        }
        
        
    }else if (comando == "info") {
        // mostrara la info de la mesa
        
    
    } else if (comando == "pedir") {
        //pedir finalizara el pedido, ira siempre despues de un agregar

    
    }else if (comando == "pagar") {
        string tipo_pedido;
        int id;
        iss >> tipo_pedido >> id;

        Pedido* pedido = registro_pedidos.eliminar_pedido(id, tipo_pedido == "mesa");
        if (pedido) {
            int total = pedido->precio_total(); // falta precio total
            int total_con_propina = total + total * 0.1;
            cout << "Pedido eliminado y pagado." << endl;
            cout << "Total: " << total << endl;
            cout << "Total con propina: " << total_con_propina << endl;
        }else {
        cerr << "Pedido no encontrado para pagar." << endl;
    }
    }else if (comando == "cerrar") {
        // muestra el total de lo ganado con la propina incluida
        return;
    }else{
        cout<<"comando no encontrado"<<endl;
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

    Plato* menu = new Plato[cant_lineas];

    //leer el archivo línea por línea
    int i=0;
    while (getline(file, linea)) {
        //guardamos el menu existente en un arreglo
        string nombre_plato;
        string precio_plato;
        bool cambio=false;

        for(string::size_type j=0; j<linea.length(); j++){
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

        i++;
    }

    //cerrar el archivo
    file.close();

    //comienza el programa
    cout<<"ingrese el n_mesas: ";cin>> n_mesas; // primer input n_mesas = size
    Registro registro_pedidos(n_mesas);

    while(getline(cin, linea)){
        procesarLinea(linea, registro_pedidos,menu,n_mesas);
    }


    //ejecutar todos los destructores
    delete[] menu;

    return 0;
}
