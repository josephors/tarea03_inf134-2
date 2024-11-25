#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "hash.h"

using namespace std;

void procesarLinea(string linea, Registro &registro_pedidos, Plato* menu, int largo_menu, int& id_llevar){
    istringstream iss(linea); // iss es de sstream, sirve para manejar mejor las palabras en una linea
    string comando;
    iss >> comando; // la primera palabra de una linea siempre va a ser el comando

    if (comando == "registrar") {
        string tipo_pedido;
        int numero_mesa;
        iss >> tipo_pedido >> numero_mesa;

        // cout << "TIPO_PEDIDO: " << tipo_pedido << "." << endl;
        
        if(tipo_pedido=="mesa"){ // servir = True
            Pedido* nuevoPedido = new Pedido();
            nuevoPedido->setMesa(numero_mesa);
            nuevoPedido->setTipo(true);
            // registro_pedidos.agregar_pedido(nuevoPedido); // aqui agregamos los detalles
            registro_pedidos.set_pedido_actual(nuevoPedido);
            // cout<<nuevoPedido->getMesa()<<endl;
        } else {
            Pedido* nuevoPedido = new Pedido(); // servir = False
            nuevoPedido->setMesa(id_llevar); //tiene id secuencial 
            nuevoPedido->setTipo(false);
            // registro_pedidos.agregar_pedido(nuevoPedido);
            
            // cout << "---------" << endl;
            // cout << nuevoPedido->getMesa() << endl;
            // cout << nuevoPedido->getTipo() << endl;
            registro_pedidos.set_pedido_actual(nuevoPedido);
            // id_llevar++;
        }

    } else if (comando == "agregar") {
        string nombre_plato;
        // iss >> nombre_plato;
        // nombre_plato = nombre_plato.substr(1); //nos saltamos el espacio que queda guardado

        getline(iss, nombre_plato); //guardamos el string restante del stream en nombre_plato

        //eliminamos el espacio en blanco que queda de la operación de stream anterior
        if (!nombre_plato.empty()) { //vemos si no está vacío
            nombre_plato = nombre_plato.substr(1);
        } else {
            cout << "El string está vacío, no se puede eliminar el primer carácter." << endl;
        }

        // Buscar el precio del plato en el menú
        int precio_plato = -1; //valor sin asignar
        for (int i = 0; i < largo_menu; i++) {
            // cout << "-----------" << endl;
            // cout << "NOMBRE_RECORRIDO: " << menu[i].nombre << '.' << endl;
            // cout << "NOMBRE_QUE_BUSCAMOS: " << nombre_plato << '.' << endl;

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
            // cout << "Plato \"" << nombre_plato << "\" agregado al pedido actual con precio " << precio_plato << " pesos." << endl;
        } catch (const runtime_error& e) {
            cerr << "Error al agregar plato: " << e.what() << endl;
        }
        
        
    } else if (comando == "info") {
        // mostrara la info de la mesa
        string tipo_pedido;
        int id;
        iss >> tipo_pedido >> id;

        // Determinar si el pedido es para servir o llevar
        bool es_servir = (tipo_pedido == "mesa");

        // Llamar al método Platos_Precios para mostrar la información del pedido
        registro_pedidos.Platos_Precios(id, es_servir);
    
    } else if (comando == "pedir") {
        //pedir finalizara el pedido, ira siempre despues de un agregar
        bool tipo_mesa = registro_pedidos.get_pedido_actual()->getTipo();
        if(tipo_mesa){
            cout << "mesa " << registro_pedidos.get_pedido_actual()->getMesa() << " Registrado" << endl;
        } else {
            cout << "llevar " << id_llevar << " Registrado" << endl;
        }

        id_llevar++;
        registro_pedidos.agregar_pedido(registro_pedidos.get_pedido_actual()); // aqui agregamos los detalles
        registro_pedidos.set_pedido_actual(nullptr); //reseteamos el pedido actual

    } else if (comando == "pagar") {
        string tipo_pedido;
        int id;
        iss >> tipo_pedido >> id;

        Pedido* pedido = registro_pedidos.eliminar_pedido(id, tipo_pedido == "mesa"); //debería retornar el pedido
        if (pedido) {
            int total = pedido->precio_total(); // falta precio total
            int total_con_propina = total + total * 0.1;
            // cout << "Pedido eliminado y pagado." << endl;
            cout << "Total: " << total << endl;
            cout << "Propina: " << total*0.1 << endl;
            cout << "Total + propina: " << total_con_propina << endl;
            cout << "Factor de carga: " << registro_pedidos.retornarFactorDeCarga() << endl;

        } else {
            cerr << "Pedido no encontrado para pagar." << endl;
        }
    } else if (comando == "cerrar") {
        // muestra el total de lo ganado con la propina incluida
        
        return;
    } else {
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
    // cout << cant_lineas << endl;

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

        nombre_plato = nombre_plato.substr(0, nombre_plato.length()-1); //"hola ", length=4
        menu[i].nombre = nombre_plato;
        menu[i].precio = stoi(precio_plato);

        i++;
    }

    //cerrar el archivo
    file.close();

    //comienza el programa
    cout << "ingrese el n_mesas: ";
    cin >> n_mesas; // primer input n_mesas = size
    Registro registro_pedidos(n_mesas);
	cin.ignore(1, '\n');

    int id_llevar = 1; //se inicaliza desde 1
    while(getline(cin, linea)){
        procesarLinea(linea, registro_pedidos, menu, cant_lineas, id_llevar);
    }

    //ejecutar todos los destructores
    delete[] menu;

    return 0;
}
