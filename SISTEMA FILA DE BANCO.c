/*******************************************************************************
 * PROYECTO INTEGRADOR: Simulador de Fila de Banco Estructura de Datos FIFO
 * Universidad del Valle de México (UVM) - Campus Reforma
 * Ingeniería en Sistemas Computacionales
 * Lider de Proyecto: Carlos Gutierrez
 * Equipo de Desarrollo: Luna Monroy Víctor Hugo, Esparragoza Morales Seth Carlos, Rodríguez Chua Santiago Antonio, Velasco Hernandez Omar, Tapia Pérez Jorge Arturo
 * Periodo de Desarrollo: Abril 2026 - Mayo 2026
 * DESCRIPCIÓN DEL SISTEMA:
 * Este programa implementa una simulación de eventos discretos en lenguaje C 
 * para gestionar la llegada, espera y atención de clientes en una caja bancaria. 
 * Se fundamenta en una estructura de datos tipo cola (FIFO) e incorpora un 
 * sistema de prioridades de 3 niveles:
 * 1. VIP (Alta prioridad, menor tolerancia de espera)
 * 2. Cliente Nuevo (Prioridad media)
 * 3. Cliente Normal (Prioridad estándar, mayor tolerancia)
 * * CARACTERÍSTICAS TÉCNICAS:
 * - Generación estocástica de clientes basada en probabilidad porcentual por minuto.
 * - Asignación de tiempos de atención variables mediante funciones aleatorias.
 * - Lógica de "agente autónomo" para el abandono de la cola cuando el tiempo 
 * de espera supera la tolerancia según el tipo de cliente.
 * - Reordenamiento dinámico de la cola para respetar la jerarquía de atención.
 * * Notas de la versión: 1.1
 * Se refactorizó el pseudocódigo original para asegurar el correcto dimensionamiento
 * de memoria en arreglos y se corrigió el desfase temporal en la liberación de la caja.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h> // Para omar, revisa su biblioteca, hay muchos inputs como textcolor, colorbackground, etc
#include <time.h> // Útil si en el futuro llegamos a agregar la fecha/hora real del sistema

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

//=======================================================================================================
// 10. Sistema de log de eventos (registro_banco.txt) con formato ASCII en desarrollo, necesito estrucutra logica para registrar cada movimiento de la simulación 
// (llegada, atención, abandono) con detalles como minuto, tipo de cliente, tiempo de espera, etc. El log debe ser legible y organizado para facilitar el análisis posterior.
// dejen esto al final, no hace falta que lo dejen al inicio
// =====================================================================================================

void inicializar_log() {
    // "w" abre el archivo para escribir desde cero. Usa "a" si quieres añadir al final sin borrar
    FILE *archivo_log = fopen("registro_banco.txt", "w"); 
    
    if (archivo_log == NULL) {
        printf("Error al crear el archivo de log.\n");
        return;
    }

    // algunos print para crear un encabezado legible y organizado
    fprintf(archivo_log, "+-------------------------------------------------------------------------+\n");
    fprintf(archivo_log, "|                    LOG DE EVENTOS - SIMULADOR BANCARIO                  |\n");
    fprintf(archivo_log, "+---------+----------+-------------+------------------+-------------------+\n");
    fprintf(archivo_log, "| MINUTO  | EVENTO   | ID CLIENTE  | TIPO DE CLIENTE  | TIEMPO DE ESPERA  |\n");
    fprintf(archivo_log, "+---------+----------+-------------+------------------+-------------------+\n");
    
    fclose(archivo_log);
}

// esta funcion sirve para registrar cada evento que ocurra en la simulación, con detalles como el minuto, tipo de evento, id del cliente, tipo de cliente y tiempo de espera
void registrar_evento(int minuto, char* evento, int id_cliente, int tipo, int espera) {
    FILE *archivo_log = fopen("registro_banco.txt", "a"); // "a" (append) para no sobreescribir
    
    if (archivo_log != NULL) {
        // Se usa formato tabular con anchos fijos (ej. %-7d alinea a la izquierda)
        fprintf(archivo_log, "| %-7d | %-8s | %-11d | %-16d | %-17d |\n", 
                minuto, evento, id_cliente, tipo, espera);
        fclose(archivo_log);
    }
}

// Función para cerrar la tabla al terminar la simulación
void cerrar_log() {
    FILE *archivo_log = fopen("registro_banco.txt", "a");
    if (archivo_log != NULL) {
        fprintf(archivo_log, "+---------+----------+-------------+------------------+-------------------+\n");
        fprintf(archivo_log, "|                        FIN DE LA SIMULACION                             |\n");
        fprintf(archivo_log, "+-------------------------------------------------------------------------+\n");
        fclose(archivo_log);
    }
}