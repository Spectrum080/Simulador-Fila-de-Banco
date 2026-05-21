/* Esta es una demostracion del funcionamineto de
una nueva funcion para el programa. Lo que hace
es que toma el tipo de un cliente recien generado y dependiendo de este,
incrementa el contador del respectivo tipo de clinete, le asgna su id y su tiempo
de espera.
La idea es que la funcion se use cada vez que  se genere un nuevo cliente.

programador: Tapia Pérez Jorge Arturo
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* la funcion tomara i, tipo_rand, cont_x_tipo[3], id_num[401] y
t_atencion[401].

La funcion basicamente es el switch que el programa original usa para
asignar id, conter clientes por tipos y asignar los tiempos de atencion*/
void valores_tipo(int x,int tipo,int contador[],int id[],int tiempo[]){
    switch (tipo){
        case 1:
            contador[0]= contador[0]+1;
            id[x] = (*contador);
            tiempo[x] = (rand() % 3) + 2;
            break;
        case 2:
            contador[1]= contador[1]+1;
            id[x] = (*contador);
            tiempo[x] = (rand() % 4) + 3;
            break;
        case 3:
            contador[2]= contador[2]+1;
            id[x] = (*contador) * 10;
            tiempo[x] = (rand() % 5) + 3;
            break;
    }
    return;
}


int main(){
    srand(time(NULL));
    int tipo_rand;
    int i = 0;
    int id_num[401], t_atencion[401];
    int cont_x_tipo[3];
    /* Para que la funcion se pueda usar, tendremos que juntar los contadores de
    clientes de cada tipo, en un solo arreglo, para que la funcion pueda acceder a los tres contadores
    sin tener que invocarla varias veces*/
    int *cont_vip = &cont_x_tipo[0];
    int *cont_nuevo = &cont_x_tipo[1];
    int *cont_norm = &cont_x_tipo[2];
    /*comberti los contadores en punteros y cada uno apunta a un
    elemento en especifico del arreglo "cont_x_tipo", asi podremos seguir usando los nombres originales
    de las funciones. IMPORTANTE: cada ves que queramos acceder al valor de las variables, tendremos que
    agregar un "*" al inicio, para indicar que queremos el valor y no la direccion. Ejemplo:
    printf("Total de clientes nuevos: %d", *cont_nuevo);*/
    int reloj_prueba;
    *cont_vip = 0;
    *cont_nuevo = 0;
    *cont_norm = 0;
    for(reloj_prueba =0; reloj_prueba <=401; reloj_prueba++ ){
        if ((rand() % 100) < 60) {
            i = i+1;
            tipo_rand = (rand() % 3) + 1;

            valores_tipo(i,tipo_rand, cont_x_tipo,id_num,t_atencion);
            printf("i=%d, id_num[i]=%d,t_atencion[i]=%d \n",i,id_num[i],t_atencion[i]);
            printf("LOS VIP:%d, LOS NUEVOS:%d,LOS NORMALES:%d \n",*cont_vip,*cont_nuevo,*cont_norm);
        }
    }
    return 0;
}
