#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Comentarios sobre prueba de testeo #1
/*
Despues de haber ejecutado el codigo entre unas 4 y 5 veces sin haber realizado ninguna
modificacion, puedo decir que el codigo actual funciona perfectamente; sin embargo, con
la probabilidad de llegada de clientes que teniamos asignada (40%), me percate de que la
fila estaba vacia la mayor parte del tiempo, no superando la cantidad de un cliente en la fila
a la vez. Al provar con otras probavilidades de llegada, ademas de ver el comportamiento del codigo
es esos casos, decidi provar con una probavilidad de llegada del 60%, y pude ver que ahora la fila
estaba mas llena que antes, permitiendo ver un llegada algo mas realista y que permitia ver la funcion
de clientes saliendo de la fila funcionando. Dado estos resultados, sugeriria cambiar la probavilidad a 60.

Ademas, se provo cambiar el valor de tiempo_max a numeros mas grandes y mas pequeños, asi como hacerlo con
o sin arreglos de tamaño proporcional al tiempo. Luego de provar esto, se dedujo que se puede usar cualquier
tiempo maximo, siempre y cuando los areglos de informacion de clientes(como t_llegada por ejemplo)
tengan un tamaño proporcional al tiempo. Si la capacidad supera al tiempo, no pasa nada, pero se estaria
ocupando espacio inecesario; en el caso contrario( si el tiempo supera al almacenamiento), entonces el 
programa dejara de funcionar a decuadamente, dejando la cola vacia y arrojando resultados raros,llega a un
punto donde la simulacion termina cuando deveria continuar, y al final el reporte muestra datos sin
sentido. Recalco que esto es algo a considerar cuando queramos extender el tiempo de simulacion, asi
como para la implementacion del menu, si llega a permitir cambiar el tiempo al gusto del usuario, o si
la simulacion se va a ejecutar indefinidamente hasta que el usuario lo indique.

Atte - J. Arturo Tapia P. , Documentador y tester
*/
int main() {
    srand(time(NULL));

    // Variables
    int i, j, k, c;
    int reloj, tam_cola;
    int cont_vip, cont_nuevo, cont_norm;
    int espera_act, temp_idx;
    int tiempo_max, prob_llegada, tipo_rand;
    int NUM_CAJAS = 3;

    // Arreglos por cada caja (indices 1..3, se declara tama�o 4)
    int caja_libre[4], fin_atencion[4], cliente_en_caja[4];

    // Arreglos de cliente (indices 1..400, se declara tama�o 401)
    int t_llegada[401], atendido[401], se_fue[401], tipo[401];
    int id_num[401], t_atencion[401], cola[401], t_espera[401];
    int t_salida[401], caja_asignada[401];

    // Inicializacion
    i = 0;
    reloj = 1;
    tam_cola = 0;
    cont_vip = 0;
    cont_nuevo = 0;
    cont_norm = 0;

    for (c = 1; c <= NUM_CAJAS; c++) {
        caja_libre[c] = 1;
        fin_atencion[c] = 0;
        cliente_en_caja[c] = 0;
    }

    tiempo_max = 400;
    prob_llegada = 60;

    printf("---------------------------------------\n");
    printf(" SIMULADOR DE FILA DE BANCO CON 3 CAJAS\n");
    printf("----------------------------------------\n");

    do {
        printf("=== Minuto %d ===\n", reloj);

        // Generacion de clientes
        if ((rand() % 100) < prob_llegada) {

            i = i + 1;
            t_llegada[i] = reloj;
            atendido[i] = 0;
            se_fue[i] = 0;
            caja_asignada[i] = 0;

            tipo_rand = (rand() % 3) + 1;
            tipo[i] = tipo_rand;

            switch (tipo_rand) {
                case 1:
                    cont_vip = cont_vip + 1;
                    id_num[i] = cont_vip;
                    t_atencion[i] = (rand() % 3) + 2;
                    break;
                case 2:
                    cont_nuevo = cont_nuevo + 1;
                    id_num[i] = cont_nuevo;
                    t_atencion[i] = (rand() % 4) + 3;
                    break;
                case 3:
                    cont_norm = cont_norm + 1;
                    id_num[i] = cont_norm * 10;
                    t_atencion[i] = (rand() % 5) + 3;
                    break;
            }

            tam_cola = tam_cola + 1;
            cola[tam_cola] = i;

            // prioridad
            j = tam_cola;
            while (j > 1) {
                if (tipo[cola[j]] < tipo[cola[j-1]]) {
                    temp_idx = cola[j];
                    cola[j] = cola[j-1];
                    cola[j-1] = temp_idx;
                    j = j - 1;
                } else {
                    j = 0;
                }
            }

            printf("  [LLEGO] Cliente tipo %d ID: %d\n", tipo[i], id_num[i]);
        }

        // Revisar si alguien se va
        for (j = 1; j <= tam_cola; j++) {
            espera_act = reloj - t_llegada[cola[j]];
            if (tipo[cola[j]] == 1 && espera_act >= 8) {
                se_fue[cola[j]] = 1;
                printf("  [SE FUE] VIP ID: %d\n", id_num[cola[j]]);
            } else {
                if (tipo[cola[j]] == 2 && espera_act >= 6) {
                    se_fue[cola[j]] = 1;
                    printf("  [SE FUE] NUEVO ID: %d\n", id_num[cola[j]]);
                } else {
                    if (tipo[cola[j]] == 3 && espera_act >= 10) {
                        se_fue[cola[j]] = 1;
                        printf("  [SE FUE] NORMAL ID: %d\n", id_num[cola[j]]);
                    }
                }
            }
        }

        // limpiar cola
        k = 0;
        for (j = 1; j <= tam_cola; j++) {
            if (se_fue[cola[j]] == 0 && atendido[cola[j]] == 0) {
                k = k + 1;
                cola[k] = cola[j];
            }
        }
        tam_cola = k;

        // terminar la ocupacion de las cajas
        for (c = 1; c <= NUM_CAJAS; c++) {
            if (caja_libre[c] == 0 && reloj >= fin_atencion[c]) {
                atendido[cliente_en_caja[c]] = 1;
                t_salida[cliente_en_caja[c]] = reloj;
                printf("  [ATENDIDO] Caja %d - ID: %d\n", c, id_num[cliente_en_caja[c]]);
                caja_libre[c] = 1;
            }
        }

        // El que sigueeeee
        for (c = 1; c <= NUM_CAJAS; c++) {
            if (caja_libre[c] == 1 && tam_cola > 0) {
                cliente_en_caja[c] = cola[1];
                t_espera[cliente_en_caja[c]] = reloj - t_llegada[cliente_en_caja[c]];
                fin_atencion[c] = reloj + t_atencion[cliente_en_caja[c]];
                caja_libre[c] = 0;
                caja_asignada[cliente_en_caja[c]] = c;

                printf("  [EN CAJA %d] ID: %d\n", c, id_num[cliente_en_caja[c]]);

                for (j = 1; j <= tam_cola - 1; j++) {
                    cola[j] = cola[j + 1];
                }
                tam_cola = tam_cola - 1;
            }
        }

        printf("  Cola actual: %d\n", tam_cola);
        reloj = reloj + 1;

    } while (reloj <= tiempo_max);


    // Reporte final
    int tot_atendidos, tot_sefueron;
    double prom_espera;
    int suma_temp, cnt_temp;

    int cnt_caja[4];
    int vip_caja[4];
    int nuevo_caja[4];
    int norm_caja[4];
    int sum_espera_caja[4];
    int max_espera_caja[4];

    tot_atendidos = 0;
    tot_sefueron = 0;

    for (c = 1; c <= NUM_CAJAS; c++) {
        cnt_caja[c] = 0;
        vip_caja[c] = 0;
        nuevo_caja[c] = 0;
        norm_caja[c] = 0;
        sum_espera_caja[c] = 0;
        max_espera_caja[c] = 0;
    }

    for (j = 1; j <= i; j++) {

        if (se_fue[j] == 1) {
            tot_sefueron = tot_sefueron + 1;
        }

        if (atendido[j] == 1 && caja_asignada[j] > 0) {
            tot_atendidos = tot_atendidos + 1;
            c = caja_asignada[j];

            cnt_caja[c] = cnt_caja[c] + 1;
            sum_espera_caja[c] = sum_espera_caja[c] + t_espera[j];

            if (t_espera[j] > max_espera_caja[c]) {
                max_espera_caja[c] = t_espera[j];
            }

            switch (tipo[j]) {
                case 1: vip_caja[c] = vip_caja[c] + 1; break;
                case 2: nuevo_caja[c] = nuevo_caja[c] + 1; break;
                case 3: norm_caja[c] = norm_caja[c] + 1; break;
            }
        }
    }

    printf("--------------------------------------\n");
    printf("           REPORTE FINAL\n");
    printf("---------------------------------------\n");
    printf(" Clientes generados: %d\n", i);
    printf("   VIP: %d\n", cont_vip);
    printf("   Nuevos: %d\n", cont_nuevo);
    printf("   Normales: %d\n", cont_norm);
    printf(" Clientes atendidos: %d\n", tot_atendidos);
    printf(" Clientes que se fueron: %d\n", tot_sefueron);

    for (c = 1; c <= NUM_CAJAS; c++) {

        if (cnt_caja[c] > 0) {
            suma_temp = sum_espera_caja[c];
            cnt_temp = cnt_caja[c];
            prom_espera = (int)(((double)suma_temp / cnt_temp) * 60 * 100) / 100.0;
        } else {
            prom_espera = 0;
        }

        printf("--------------------------------------\n");
        printf(" CAJA %d\n", c);
        printf("   Clientes atendidos:  %d\n", cnt_caja[c]);
        printf("   - VIP:  %d\n", vip_caja[c]);
        printf("   - Nuevos: %d\n", nuevo_caja[c]);
        printf("   - Normales: %d\n", norm_caja[c]);
        printf("   Espera promedio: %.2f seg\n", prom_espera);
        printf("   Espera maxima: %d min\n", max_espera_caja[c]);
    }

    return 0;
}
