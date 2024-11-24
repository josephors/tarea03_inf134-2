#include "hash.h"
#include <iostream>
#include <stdexcept>

using namespace std;

/*****
* registro_cuentas
******
* Función constructora de nuestra clase; crea una tabla hash con una cantidad de ranuras dictada por "ranuras". 
******
* Input:
* 
* .......
******
* Returns:
* no retorna nada.
*****/
registro_cuentas::registro_cuentas() {
    tabla = new cuenta[ranuras]();
}

/*****
* ~registro_cuentas
******
* Destructor de nuestra clase; elimina nuestra tabla hash.
******
* Input:
* 
* .......
******
* Returns:
* no retorna nada.
*****/
registro_cuentas::~registro_cuentas() {
    delete[] tabla;
}

/*****
* int hash
******
* Método el cual genera un "hash value" o "clave de hash" relativo a nuestra tabla de hash, en base al rol que le entreguemos por parámetro.
******
* Input:
* string rol : El rol para el cual generaremos la clave de hash.
* .......
******
* Returns:
* int, el "hash value" o "clave de hash" generado con el rol que se le entregó al método.
*****/
int registro_cuentas::hash(string rol) {
    int hash_value = 0;
    for (char c : rol) {
        hash_value = (hash_value * 31 + c) % ranuras; //en esta suma, "c" actúa el valor en ASCII de la letra que estemos recorriendo.
    }
    return hash_value;
}

/*****
* int p
******
* Método que "gestiona" las colisiones, y que en caso de que haya una colisión para la ranura de valor "hash(rol)", generará, de forma lineal, una nueva clave de hash, en la cual insertar nuestro valor en cuestión.
******
* Input:
* string rol : El rol para el cual queremos hacer este rehashing, o sea, el rol que tuvo una colisión.
* int i : Cantidad i de colisiones que ha tenido este valor a insertar. Inicialmente es cero, pero en caso de tener 1 o más colisiones, iremos generando nuevas claves de hash hasta hallar una ranura en la que poder insertar el valor al que referencia nuestro rol.
* .......
******
* Returns:
* int, retorna la nueva clave de hash en donde insertar el valor que tuvo la colisión.
*****/
int registro_cuentas::p(string rol, int i) {
    return (hash(rol) + i) % ranuras;
}

/*****
* cuenta obtener
******
* Método que obtiene/retorna la cuenta asociada, en la tabla hash, al rol que pasemos como parámetro. Esto lo hace calculando la clave de hash del rol que tenemos por parámetro, dirigiéndonos a esa ranura y retornar el valor almacenado en esa ranura, que es un valor de tipo struct "cuenta".
******
* Input:
* string rol : Rol de la cuenta que queremos obtener de nuestra tabla hash.
* .......
******
* Returns:
* cuenta, nos retorna la cuenta asociada al rol que pasamos por parámetro.
*****/
cuenta registro_cuentas::obtener(string rol){
    int hash_value = hash(rol);
    int index = hash_value;
    int i = 0;
	
    while (tabla[index].rol != rol && i<ranuras){
        index = p(rol,i);
        i++;
    } //recorreremos nuestra tabla hasta hallar la cuenta que estamos buscando, o hasta haber recorrido toda la tabla...
    
	if (i==ranuras){ //si no hubo match de rol (si es que hubiera habido match, i sería menor que ranuras)...
        cout << "Rol no existente" << endl;
        return cuenta();
    } else {
        return tabla[index];
    }
}

/*****
* void agregar
******
* Método que agrega, mediante hashing, la cuenta que tiene como parámetro a nuestra tabla hash.
******
* Input:
* cuenta c : Valor de tipo "cuenta" que queremos agregar a la tabla hash.
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::agregar(cuenta c) {
    int index = hash(c.rol)%ranuras;
	
    if (tabla[index].nombre == ""){ //si tabla[index].nombre está libre...
        tabla[index] = c;
		actualizarStats('a');	
    
	} else { //si no está libre, o sea, tenemos una colisión...
		if (tabla[index].rol == c.rol){
			cout << "Rol ya existente" << endl;
		} else {
			int i=0;
			bool found=false;
			
			while(!found && i<ranuras){ //mientras no encontremos ranura disponible...
				int pos = p(c.rol, i);
				
				if(tabla[pos].nombre ==""){ //si encontramos una ranura libre...
					found = true;
					tabla[pos] = c;

					actualizarStats('a');
				}
				
				i++;  
			}
			
			if (!found){ //si recorrimos toda la tabla y no hay espacio libre...
				//redimensionar y agregar el actual.
				redimensionar(ranuras*2); //duplicamos la capacidad de la tabla hash
				
				agregar(c); //agregamos la cuenta que no pudimos agregar
			}
		}
    }
}

/*****
* void eliminar
******
* Método que elimina de nuestra tabla hash la cuenta con el rol que pasemos como parámetro.
******
* Input:
* string rol : Rol de la cuenta que queremos eliminar de nuestra tabla hash.
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::eliminar(string rol){
    int i = 0;
    int index = hash(rol)%ranuras;
    //AQUI VA ESTAR LA 1ERA CONDICION (QUE LO ENCONTREMOS ALTIRO)
    //PARA COMPARAR HABRA QUE ACCEDER AL ROL
    if (tabla[index].rol==rol){
        tabla[index].nombre= "";
        tabla[index].descripcion = "";
        tabla[index].rol= "";
        // AQUI HAY QUE AGREGAR UN CONTANDOR DE ELIMINACION
		actualizarStats('e');
    }
    else {
        bool found = false;
        while(i<ranuras && found==false){
            int pos = p(rol,i);
            if(tabla[pos].rol==rol){
                tabla[index].nombre= "";
                tabla[index].descripcion = "";
                tabla[index].rol= "";
				actualizarStats('e');
                found = true;
			}
            i++;
		}
        if(found == false){
            cout<<"Rol no existente"<<endl;
        }
    }

}

/*****
* void modificar
******
* Método que modifica la descripción de la cuenta con el rol que pasemos por primer parámetro por una nueva descripción, que es el segundo parámetro del método.
******
* Input:
* string rol : Rol de la cuenta a la que le queremos modificar la descripción.
* string descripcion : Nueva descripción con la que reemplazaremos la anterior.
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::modificar(string rol, string descripcion){
    
    int i = 0;
    int index = hash(rol)%ranuras;
    if (tabla[index].rol == rol){
        tabla[index].descripcion = descripcion;
    }  
    else{
        bool found = false;
        while(i<ranuras && found== false){
            int pos = p(rol,i);
            if (tabla[pos].rol == rol){
                tabla[pos].descripcion = descripcion;
                found = true;
            i++;
            }
        } cout<<"Rol no existente"<<endl;
    }
};

/*****
* void estadisticas
******
* Método que muestra en la consola las ranuras ocupadas, las ranuras totales y el factor de carga que tiene nuestra tabla hash.
******
* Input:
* 
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::estadisticas(){
	cout << "RANURAS OCUPADAS: " << ocupadas << endl;
	cout << "RANURAS TOTALES: " << ranuras << endl;
	cout << "FACTOR DE CARGA: " << factor_de_carga << endl;
}

/*****
* void actualizarStats
******
* Método que actualizará las estadísticas "ocupadas" y "factor_de_carga" cuando ocurra un UPDATE de tipo añadir o eliminar. 
******
* Input:
* char tipoUpdate: El tipo de UPDATE que ocurrió: 'a' para añadir, 'e' para eliminar.
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::actualizarStats(char tipoUpdate){
	if(tipoUpdate=='a'){
		//aumentamos las ranuras ocupadas
		ocupadas++;
	} else if (tipoUpdate=='e') {
		//disminuimos las ranuras ocupadas
		ocupadas--;
	}
	
	factor_de_carga=(ocupadas*1.0)/ranuras; //multiplicamos por 1.0 para que el resultado sea float.
}

/*****
* void redimensionar
******
* Método que redimensionará nuestra tabla hash, cambiándola a una cantidad "n" de ranuras. Esto lo hace 
  creando una tabla "nuevaTabla" momentanea, agregando nuestros valores de la anterior tabla a esta nueva haciendo
  un "rehashing" (asignándoles nuevas claves hash dado la nueva cantidad total de ranuras), liberando la 
  memoria de la anterior y asignando como valor esta "nuevaTabla" a registro_cuentas::tabla. Ahora, 
  registro_cuentas::tabla tendrá los mismos valores almacenados, pero una nueva cantidad "n" de ranuras.
******
* Input:
* int n: El nuevo tamaño o dimensión que queremos que tome nuestra tabla, o al cual queremos que se redimensione.
* .......
******
* Returns:
* void, no retorna nada.
*****/
void registro_cuentas::redimensionar(int n){
    cuenta* nuevaTabla = new cuenta[n]();
	
	//agregamos cada una de las cuentas a la nueva tabla
	for(int i=0; i<ranuras; i++){
		if(tabla[i].nombre!=""){ //si la ranura tiene algo...
			cuenta c = tabla[i];
			
			int index = hash(c.rol) % n;
			
			//agregamos valor en "nuevaTabla"
			if (nuevaTabla[index].nombre == ""){
				nuevaTabla[index] = c;
			} else {
				bool found=false;
				int j=0;
				
				while(!found){ //mientras no encontremos ranura libre...
					int pos = (hash(c.rol) + j) % n;
					
					if(nuevaTabla[pos].nombre == ""){ //si la ranura está libre...
						found = true;
						nuevaTabla[pos] = c;
					}
					
					j++;
				}
			}
		}
	}

	delete[] tabla;
	
	tabla=nuevaTabla;
	ranuras=n;
	actualizarStats('x');
}
