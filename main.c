#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Inicializar semilla para números aleatorios
    srand(time(NULL));

    int clientes_totales, vip_totales, normal_totales, nuevos_totales;
    int i=0, j=0, reloj=0;
    int FILA[100]={}, ID[100]={}, TIPO[100]={}, TIEMPO_ATENCION[100]={};


        reloj++;

        //DECLARAR CLIENTES
        int prob_cliente=rand()%100+1;
        int id=0, tipo=0, tiempo_atencion;

        if(prob_cliente<41){
            clientes_totales++;
            id++;
            ID[id-1]=id;
            tipo=rand()%3+1;

            switch(tipo){
            case 1: //VIP
                vip_totales++;
                TIPO[id-1]=tipo;
                break;

            case 2: //NUEVO
                nuevos_totales++;
                TIPO[id-1]=tipo;
                break;

            case 3: //NORMAL
                normal_totales++;
                TIPO[id-1]=tipo;
                break;
            }



        }




    printf("")
    return 0;
}
