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
void ord_priori(int x, int fila[]){
    int temp_idx;
    temp_idx = fila[x];
    fila[x] = fila[x-1];
    fila[x-1] = temp_idx;
    return;
}
void reporte_por_caja(int x, double promedio, int cnt[], int vip[], int nuevo[], int norm[], int max_espera[]){
    printf("--------------------------------------\n");
    printf(" CAJA %d\n", x);
    printf("   Clientes atendidos:  %d\n", cnt[x]);
    printf("   - VIP:  %d\n", vip[x]);
    printf("   - Nuevos: %d\n", nuevo[x]);
    printf("   - Normales: %d\n", norm[x]);
    printf("   Espera promedio: %.2f seg\n", promedio);
    printf("   Espera maxima: %d min\n", max_espera[x]);
    return;
}
void Reporte_final (int x, int atendidos, int se_fueron, int contador[]){
    printf("--------------------------------------\n");
    printf("           REPORTE FINAL\n");
    printf("---------------------------------------\n");
    printf(" Clientes generados: %d\n", x);
    printf("   VIP: %d\n", contador[0]);
    printf("   Nuevos: %d\n", contador[1]);
    printf("   Normales: %d\n", contador[2]);
    printf(" Clientes atendidos: %d\n", atendidos);
    printf(" Clientes que se fueron: %d\n", se_fueron);
    return;
}
int main() {
    srand(time(NULL));

    // Variables
    int i, j, k, c;
    int reloj, tam_cola;
    int cont_tipo[3];
    int *cont_vip = &cont_tipo[0];
    int *cont_nuevo = &cont_tipo[1];
    int *cont_norm = &cont_tipo[2];
    int espera_act;
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
    *cont_vip = 0;
    *cont_nuevo = 0;
    *cont_norm = 0;

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
            valores_tipo(i,tipo_rand, cont_tipo,id_num,t_atencion);

            tam_cola = tam_cola + 1;
            cola[tam_cola] = i;

            // prioridad
            j = tam_cola;
            while (j > 1) {
                if (tipo[cola[j]] < tipo[cola[j-1]]) {
                    ord_priori(j,cola);
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
    Reporte_final(i,tot_atendidos,tot_sefueron,cont_tipo);

    for (c = 1; c <= NUM_CAJAS; c++) {

        if (cnt_caja[c] > 0) {
            suma_temp = sum_espera_caja[c];
            cnt_temp = cnt_caja[c];
            prom_espera = (int)(((double)suma_temp / cnt_temp) * 60 * 100) / 100.0;
        } else {
            prom_espera = 0;
        }
        reporte_por_caja(c,prom_espera,cnt_caja,vip_caja,nuevo_caja,norm_caja,max_espera_caja);
    }

    return 0;
}
