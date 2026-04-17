//Librerias//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//Clientes//
struct Clientes { //Voy a usar struct para no dejar las variables sueltas//
int id;
int prioridad;
int tiempo_llegada;
int tiempo_atencion;
int tiempo_espera;
};

//Cajas//
struct Cajas {
int numero;
int ocupada;
};

int main () {

//Variables con float//
float tiempo_total = 0.0;
float promedio_tiempo = 0.0;
//Variables con int//
int cajero_desocupado = 0; //O sea que el cajero esta desocupado en el minuto 0//


//Numero de clientes//
int num_clientes = 5; //Podemos agregar mas despues, ahorita es para hacer la estructura//
struct Clientes fila [5] = {

       {2, 3, 2, 2, 0},
       {4, 8, 3, 1, 0},
       {1, 0, 4, 3, 0},
       {5, 10, 5, 2, 0},
       {3, 5, 6, 4, 0}

};


//Primer ciclo//
for (int i = 0; i < num_clientes; i++) {

        if ( cajero_desocupado < fila[i].tiempo_llegada){
            cajero_desocupado = fila[i].tiempo_llegada;

        }
    fila[i].tiempo_espera = cajero_desocupado - fila[i].tiempo_llegada;
    cajero_desocupado = cajero_desocupado + fila[i].tiempo_atencion;

     tiempo_total += fila[i].tiempo_espera;
}

//Calculamos el promedio del tiempo//
 promedio_tiempo = tiempo_total / num_clientes;









return 0;
}
