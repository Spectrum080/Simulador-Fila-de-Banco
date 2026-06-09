//*******************************************************************************
 //* PROYECTO INTEGRADOR: Simulador de Fila de Banco Estructura de Datos FIFO
 //* Universidad del Valle de México (UVM) - Campus Reforma
 //* Ingeniería en Sistemas Computacionales
 //* Lider de Proyecto: Carlos Gutierrez
 //* Equipo de Desarrollo: Luna Monroy Víctor Hugo, Esparragoza Morales Seth Carlos, Rodríguez Chua Santiago Antonio, Velasco Hernandez Omar, Tapia Pérez Jorge Arturo
 //* Periodo de Desarrollo: Abril 2026 - Junio 2026
 //* DESCRIPCIÓN DEL SISTEMA:
 //* Este programa implementa una simulación de eventos discretos en lenguaje C
 //* para gestionar la llegada, espera y atención de clientes en una caja bancaria.
 //* Se fundamenta en una estructura de datos tipo cola (FIFO) e incorpora un
 //* sistema de prioridades de 3 niveles:
 //* 1. VIP (Alta prioridad, menor tolerancia de espera)
 //* 2. Cliente Nuevo (Prioridad media)
 //* 3. Cliente Normal (Prioridad estándar, mayor tolerancia)
 //* * CARACTERÍSTICAS TÉCNICAS:
 //* - Generación estocástica de clientes basada en probabilidad porcentual por minuto.
 //* - Asignación de tiempos de atención variables mediante funciones aleatorias.
 //* - Lógica de "agente autónomo" para el abandono de la cola cuando el tiempo
 //* de espera supera la tolerancia según el tipo de cliente.
 //* - Reordenamiento dinámico de la cola para respetar la jerarquía de atención.
 //* * Notas de la versión: 1.1 (Integrado con Menú V2)
 //* Se refactorizó el pseudocódigo original para asegurar el correcto dimensionamiento.
 //* de memoria en arreglos y se corrigió el desfase temporal en la liberación de la caja.
 //*******************************************************************************//

// ==========================================
// LIBRERIAS
// ==========================================
#include <stdio.h>      // Funciones básica de entrada/salida (printf, scanf)
#include <stdlib.h>     // Funciones de uso general (rand, srand, system)
#include <time.h>       // Funciones para manipular el tiempo (usado para la aleatoridad)
#include <windows.h>    // Funciones específicas de Windows (colores de consola, tamaño de fuente)

// ==========================================
// FUNCIONES
// ==========================================
// Declaración anticipada de las funciones para que el compilador las reconozca antes del main
void valores_tipo(int x,int tipo,int contador[],int id[],int tiempo[]);
void ord_priori(int x, int fila[]);
void Alguien_se_va(int j,int espera_act,int cola[],int tipo[],int se_fue[],int id_num[]);
void reporte_por_caja(int x, double promedio, int cnt[], int vip[], int nuevo[], int norm[], int max_espera[]);
void Reporte_final (int x, int atendidos, int se_fueron, int contador[]);
void menuCabecera(void);
void menuOpcionA(void);
void menuOpcionB(void);
void menuOpcionC(void);
void cambiaTamanoLetra(int ancho, int alto);
void cambiaColorTexto(int colorTexto,int arg1);
void reporteFinal(void);
void error(void);
void menuacotado(int x, int atendidos, int se_fueron, int contador[]);

int main()
{
    // ==========================================
    // INICIALIZACION
    // ==========================================
    srand(time(NULL));

    int opc;
    // Variables
    int i, j, k, c;
    int reloj, tam_cola;
    int cont_tipo[3];
    int *cont_vip = &cont_tipo[0];
    int *cont_nuevo = &cont_tipo[1];
    int *cont_norm = &cont_tipo[2];
    int espera_act;
    int tiempo_max, prob_llegada, tipo_rand;
    int NUM_CAJAS;

    // Arreglos por cada caja (indices 1..3, se declara tamaNo 4)
    int caja_libre[4], fin_atencion[4], cliente_en_caja[4];

    // Arreglos de cliente (indices 1..400, se declara tamaNo 401)
    int t_llegada[401], atendido[401], se_fue[401], tipo[401];
    int id_num[401], t_atencion[401], cola[401], t_espera[401];
    int t_salida[401], caja_asignada[401];

    // Inicializacion
    NUM_CAJAS = 3;
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

    // ==========================================
    // BUCLE PRINCIPAL DE LA SIMULACIÓN (RELOJ)
    // ==========================================
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
            Alguien_se_va(j,espera_act,cola,tipo,se_fue,id_num);
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


    // ==========================================
    // CÁLCULOS ESTADÍSTICOS PARA REPORTE FINAL
    // ==========================================
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

    // ==========================================
    // MENU INTERACTIVO POST-SIMULACIÓN
    // ==========================================
    cambiaTamanoLetra(8, 14); // Ajusta tamaño de consola de Windows
    system("color F0");       // Cambia fondo blanco, texto negro

    do{        //MENU//
        system("cls");     // Limpia pantalla
        system("color F2");
        cambiaColorTexto(0,15);
        menuCabecera(); 


        for(int i=0; i< 2; i++){
            printf("\n");
        }
        printf("ELIGE LA OPCION PARA ANALIZAR ...\n\n");
        printf(" 1)VIP\n 2)NUEVOS\n 3)NORMAL\n 4)REPORTE FINAL\n 5)SALIR\n\n");
        printf("OPCION: ");

        opc=-1;
        scanf("%d", &opc);
        int conteo=0;
        while (getchar() != '\n');


        switch(opc){
        case 1:
            system("cls");
            system("color 07");
            cambiaColorTexto(15,0);
            menuOpcionC();
            conteo=0;
            for(int i_x = 1; i_x<=401; i_x++){

             //Imprime el ID, la caja de atencion y los minutos de atencion de los clientes VIP
                       if (tipo[i_x]==1){
                            conteo=conteo+1;
                            printf("ID VIP: %d\n",id_num[i_x]);
                            printf("Caja: %d\n",caja_asignada[i_x]);
                            printf("Minuto de atencion: %d\n",t_llegada[i_x]);
                            printf("|||||||||||||||||||||||||||||||||||\n");
                             if (conteo % 10 == 0 ){
                                system("pause");
                                system("cls");
                                menuOpcionC();
                            }
                        }  //fin if
            }  //fin for

            system("pause");
            break;

        case 2:
            system("cls");
            system("color 07");
            cambiaColorTexto(15,0);
            menuOpcionA();
            conteo=0;
            for(int i_x = 1; i_x<=401; i_x++){

             //Imprime el ID, la caja de atencion y los minutos de atencion de los clientes NUEVOS
                       if (tipo[i_x]==2){
                            conteo=conteo+1;
                            printf("ID Nuevo: %d\n",id_num[i_x]);
                            printf("Caja: %d\n",caja_asignada[i_x]);
                            printf("Minuto de atencion: %d\n",t_llegada[i_x]);
                            printf("|||||||||||||||||||||||||||||||||||\n");
                             if (conteo % 10 == 0 ){
                                system("pause");
                                system("cls");
                                menuOpcionA();
                            }
                        }  //fin if
            }  //fin for

            system("pause");
            break;

        case 3:
            system("cls");
            system("color 07");
            cambiaColorTexto(15,0);
            menuOpcionB();
             conteo=0;
            for(int i_x = 1; i_x<=401; i_x++){

             //Imprime el ID, la caja de atencion y los minutos de atencion de los clientes NORMALES
                       if (tipo[i_x]==3){
                            conteo=conteo+1;
                            printf("ID Normales: %d\n",id_num[i_x]);
                            printf("Caja: %d\n",caja_asignada[i_x]);
                            printf("Minuto de atencion: %d\n",t_llegada[i_x]);
                            printf("|||||||||||||||||||||||||||||||||||\n");
                             if (conteo % 10 == 0 ){
                                system("pause");
                                system("cls");
                                menuOpcionB();
                            }
                        }  //fin if
            }  //fin for

            system("pause");
            break;

        case 4:
            system("cls");
           system("color 07");
            cambiaColorTexto(15,0);
            reporteFinal();

         //Imprime la cantidad de clientes(NORMAL, VIP, NUEVOS) generados, antendidos y que se fueron en TOTAL
            Reporte_final(i,tot_atendidos,tot_sefueron,cont_tipo);
            for (c = 1; c <= NUM_CAJAS; c++) {

                if (cnt_caja[c] > 0) {
                    suma_temp = sum_espera_caja[c];
                    cnt_temp = cnt_caja[c];
                    prom_espera = (int)(((double)suma_temp / cnt_temp) * 60 * 100) / 100.0;
                } else {
                    prom_espera = 0;
                }

             //Imprime el total de clientes(NORMAL, VIP, NUEVOS) y tiempos de espera POR CAJA
                reporte_por_caja(c,prom_espera,cnt_caja,vip_caja,nuevo_caja,norm_caja,max_espera_caja);
            }
            system("pause");
            break;

        case 5:

            opc=0;
            break;

        default:
            system("cls");
            cambiaColorTexto(12,15);
            error();
            system("pause");
            break;
        }

    }while(opc!=0);

    system("cls");

    cambiaTamanoLetra(8, 16);
    system("color 07");
    cambiaColorTexto(15,0);
    printf("Termina el menu\n");
    return 0;
}

// ==========================================
// FUNCION DE TIPIFICACION (Generación de IDs y Tiempos)
// ==========================================
void valores_tipo(int x,int tipo,int contador[],int id[],int tiempo[]){
    switch (tipo){
        case 1: // VIP
            contador[0]= contador[0]+1;
            id[x] = (contador[0]);
            tiempo[x] = (rand() % 3) + 2; // Tardan entre 2 y 4 minutos
            break;
        case 2: // NUEVO
            contador[1]= contador[1]+1;
            id[x] = (contador[1]);
            tiempo[x] = (rand() % 4) + 3; // Tardan entre 3 y 6 minutos
            break;
        case 3: // NORMAL
            contador[2]= contador[2]+1;
            id[x] = (contador[2]) * 10; // Se multiplica x 10 para diferenciar su ID visualmente
            tiempo[x] = (rand() % 5) + 3; // Tardan entre 3 y 7 minutos
            break;
    }
    return;
}

// ==========================================
// ORDENAMIENTO DE PRIORIDAD (Swap Básico)
// ==========================================
void ord_priori(int x, int fila[]){
    // Intercambia el valor en la posición x con el de la posición x-1
    int temp_idx;
    temp_idx = fila[x];
    fila[x] = fila[x-1];
    fila[x-1] = temp_idx;
    return;
}

// ==========================================
// LÓGICA DE ABANDONO DE LA FILA (Tolerancia)
// ==========================================
// Adicion V.2.1 Funcion para revisar si alguien sale de la fila
void Alguien_se_va(int j,int espera_act,int cola[],int tipo[],int se_fue[],int id_num[]){
    // VIP: Poca paciencia (se van a los 8 minutos)
    if (tipo[cola[j]] == 1 && espera_act >= 8) {
        se_fue[cola[j]] = 1;
        printf("  [SE FUE] VIP ID: %d\n", id_num[cola[j]]);
        return;
    } else {
        if (tipo[cola[j]] == 2 && espera_act >= 6) {
        // NUEVOS: Menos paciencia (se van a los 6 minutos)
            se_fue[cola[j]] = 1;
            printf("  [SE FUE] NUEVO ID: %d\n", id_num[cola[j]]);
            return;
        } else {
            if (tipo[cola[j]] == 3 && espera_act >= 10) {
             // NORMALES: Más tolerancia (se van hasta los 10 minutos)
                se_fue[cola[j]] = 1;
                printf("  [SE FUE] NORMAL ID: %d\n", id_num[cola[j]]);
                return;
            }
        }
    }
    return;
}

// ==========================================
// REPORTES VISUALES POR CAJA
// ==========================================
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
// ==========================================
// IMPRESIÓN DEL REPORTE FINAL GLOBAL
// ==========================================
void Reporte_final (int x, int atendidos, int se_fueron, int contador[]){
    //printf("--------------------------------------\n");
    //printf("           REPORTE FINAL\n");
    //printf("---------------------------------------\n");
    printf(" Clientes generados: %d\n", x);
    printf("   VIP: %d\n", contador[0]);
    printf("   Nuevos: %d\n", contador[1]);
    printf("   Normales: %d\n", contador[2]);
    printf(" Clientes atendidos: %d\n", atendidos);
    printf(" Clientes que se fueron: %d\n", se_fueron);
    return;
}

// ==========================================
// GRAFICOS DE CONSOLA Y MENUS (ASCII ART)
// ==========================================
void menuCabecera(void){
    printf(" /$$      /$$\n");
    printf("| $$$    /$$$\n");
    printf("| $$$$  /$$$$  /$$$$$$  /$$$$$$$  /$$   /$$\n");
    printf("| $$ $$/$$ $$ /$$__  $$| $$__  $$| $$  | $$\n");
    printf("| $$  $$$| $$| $$$$$$$$| $$  \\ $$| $$  | $$\n");
    printf("| $$\\  $ | $$| $$_____/| $$  | $$| $$  | $$\n");
    printf("| $$ \\/  | $$|  $$$$$$$| $$  | $$|  $$$$$$/\n");
    printf("|__/     |__/ \\_______/|__/  |__/ \\______/ \n");
    printf("-----------------------------------------------\n");
}


void menuOpcionA(void){
    printf(" /$$$$$$$  /$$   /$$  /$$$$$$  /$$    /$$ /$$$$$$   /$$$$$$$\n");
    printf("| $$__  $$| $$  | $$ /$$__  $$|  $$  /$$//$$__  $$ /$$_____/\n");
    printf("| $$  \\ $$| $$  | $$| $$$$$$$$ \\  $$/$$/| $$  \\ $$|  $$$$$$ \n");
    printf("| $$  | $$| $$  | $$| $$_____/  \\  $$$/ | $$  | $$ \\____  $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$$   \\  $/  |  $$$$$$/ /$$$$$$$/\n");
    printf("|__/  |__/ \\______/  \\_______/    \\_/    \\______/ |_______/ \n");
    printf("-------------------------------------------------------------------\n");
}

void menuOpcionB(void){
printf("                                                      $$\\                    \n");
printf("                                                      $$ |                    \n");
printf("$$$$$$$\\   $$$$$$\\   $$$$$$\\  $$$$$$\\$$$$\\   $$$$$$\\  $$ | $$$$$$\\   $$$$$$$\\ \n");
printf("$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  _$$  _$$\\  \\____$$\\ $$ |$$  __$$\\ $$  _____|\n");
printf("$$ |  $$ |$$ /  $$ |$$ |  \\__|$$ / $$ / $$ | $$$$$$$ |$$ |$$$$$$$$ |\\$$$$$$\\  \n");
printf("$$ |  $$ |$$ |  $$ |$$ |      $$ | $$ | $$ |$$  __$$ |$$ |$$   ____| \\____$$\\ \n");
printf("$$ |  $$ |\\$$$$$$  |$$ |      $$ | $$ | $$ |\\$$$$$$$ |$$ |\\$$$$$$$\\ $$$$$$$  |\n");
printf("\\__|  \\__| \\______/ \\__|      \\__| \\__| \\__| \\_______|\\__| \\_______|\\_______/ \n");
printf("--------------------------------------------------------------------------------\n");

}

void menuOpcionC(void){
printf("           $$\\ \n");
printf("           \\__|     \n");
printf("$$\\    $$\\ $$\\  $$$$$$\  \n");
printf("\\$$\\  $$  |$$ |$$  __$$\ \n");
printf(" \\$$\\$$  / $$ |$$ /  $$ |\n");
printf("  \\$$$  /  $$ |$$ |  $$ |\n");
printf("   \\$  /   $$ |$$$$$$$  |\n");
printf("    \\_/    \\__|$$  ____/ \n");
printf("               $$ |      \n");
printf("               $$ |      \n");
printf("               \\__|      \n");
printf("--------------------------\n");
}

// ==========================================
// CONTROL DE FORMATO EN CONSOLA
// ==========================================
void cambiaTamanoLetra(int ancho, int alto) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;

    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);


    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);


    fontInfo.dwFontSize.X = ancho;
    fontInfo.dwFontSize.Y = alto;

    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void cambiaColorTexto(int colorTexto,int arg1) {
    int colorFinal=(arg1 * 16)+ colorTexto;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorFinal);
}

void reporteFinal(void){
printf("                                                     /$$                      /$$$$$$  /$$                     /$$\n");
printf("                                                    | $$                     /$$__  $$|__/                    | $$\n");
printf("  /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$    /$$$$$$       | $$  \\__/ /$$ /$$$$$$$   /$$$$$$ | $$\n");
printf(" /$$__  $$ /$$__  $$ /$$__  $$ /$$__  $$ /$$__  $$|_  $$_/   /$$__  $$      | $$$$    | $$| $$__  $$ |____  $$| $$\n");
printf("| $$  \\__/| $$$$$$$$| $$  \\ $$| $$  \\ $$| $$  \\__/  | $$    | $$$$$$$$      | $$_/    | $$| $$  \\ $$  /$$$$$$$| $$\n");
printf("| $$      | $$_____/| $$  | $$| $$  | $$| $$        | $$ /$$| $$_____/      | $$      | $$| $$  | $$ /$$__  $$| $$\n");
printf("| $$      |  $$$$$$$| $$$$$$$/|  $$$$$$/| $$        |  $$$$/|  $$$$$$$      | $$      | $$| $$  | $$|  $$$$$$$| $$\n");
printf("|__/       \\_______/| $$____/  \\______/ |__/         \\___/   \\_______/      |__/      |__/|__/  |__/ \\_______/|__/\n");
printf("                    | $$                                                                                          \n");
printf("                    | $$                                                                                          \n");
printf("                    |__/                                                                                          \n");
printf("------------------------------------------------------------------------------------------------------------------------\n\n");

}

void error(void){

printf("  /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ \n");
printf(" /$$__  $$ /$$__  $$ /$$__  $$ /$$__  $$ /$$__  $$ \n");
printf("| $$$$$$$$| $$  \\__/| $$  \\__/| $$  \\ $$| $$  \\__/ \n");
printf("| $$_____/| $$      | $$      | $$  | $$| $$      \n");
printf("|  $$$$$$$| $$      | $$      |  $$$$$$/| $$      \n");
printf(" \\_______/|__/      |__/       \\______/ |__/      \n");
printf("---------------------------------------------------\n");
}


void menuacotado(int x, int atendidos, int se_fueron, int contador[]){
// son 32 lineas en tu lap
    reporteFinal();
    printf(" Clientes generados: %d\n", x);
    printf("   VIP: %d\n", contador[0]);
    printf("   Nuevos: %d\n", contador[1]);
    printf("   Normales: %d\n", contador[2]);
    printf(" Clientes atendidos: %d\n", atendidos);
    printf(" Clientes que se fueron: %d\n", se_fueron);
    for(int i=1;i<=8;i++){
        printf("\n");
    }
}
