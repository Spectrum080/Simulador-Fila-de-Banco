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
// LIBRERÍAS
// ==========================================
#include <stdio.h>      // Funciones básica de entrada/salida (printf, scanf)
#include <stdlib.h>     // Funciones de uso general (rand, srand, system)
#include <time.h>       // Funciones para manipular el tiempo (usado para la aleatoridad)
#include <windows.h>    // Funciones específicas de Windows (colores de consola, tamaño de fuente)

//==========================================
// REGISTRO PREVIO DE FUNCIONES (Prototipos)
//==========================================
// Se listan aquí arriba para que el sistema las conozca desde el inicio
// y nos permita estructurar el programa de forma limpia.

void valores_tipo(int x, int tipo, int contador[], int id[], int tiempo[]);                                      // Asigna tipo de cliente, ID y tiempo de atención
void ord_priori(int x, int fila[]);                                                                              // Ordena la cola según prioridad
void Alguien_se_va(int j,int espera_act,int cola[],int tipo[],int se_fue[],int id_num[]);                        // Evalúa si un cliente abandona la fila por espera excesiva
void reporte_por_caja(int x, double promedio, int cnt[], int vip[], int nuevo[], int norm[], int max_espera[]);  // Reporte de una caja específica.
void Reporte_final (int x, int atendidos, int se_fueron, int contador[]);                                        // Reporte global final del sistema
void menuCabecera(void);                                                                                         // Menús ASCII de interfaz
void menuOpcionA(void);                                                                                          // Menús ASCII de interfaz
void menuOpcionB(void);                                                                                          // Menús ASCII de interfaz
void menuOpcionC(void);                                                                                          // Menús ASCII de interfaz
void cambiaTamanoLetra(int ancho, int alto);                                                                     // Cambia tamaño de letra en consola (Windows API)
void cambiaColorTexto(int colorTexto,int arg1);                                                                  // Cambia color de texto y fondo en consola
void reporteFinal_menu(void);                                                                                         // Título de reporte final ASCII
void error(void);                                                                                                // Mensaje de error ASCII
void menuacotado(int x, int atendidos, int se_fueron, int contador[]);                                           // Reporte compacto para pantallas pequeñas

int main()
{
    // ==========================================
    // INICIALIZACION
    // ==========================================
    srand(time(NULL));                       // Temporizador para variar números aleatorios en cada ejecución

    // --- Variables de Control y Bucles ---
    int opc;                                 // Variable para controlar la opción del menú interactivo final
    int i, j, k, c;                          // Contadores para los ciclos (clientes, colas, cajas, etc.)
    int reloj, tam_cola;                     // reloj: minuto actual de simulación | tam_cola: personas en fila

    // --- Contadores por Categoría de Cliente ---
    int cont_tipo[3];                        // Arreglo para contar cuántos clientes de cada tipo se generan
    int *cont_vip = &cont_tipo[0];           // Acceso directo a la posición 0 (VIP)
    int *cont_nuevo = &cont_tipo[1];         // Acceso directo a la posición 1 (Nuevos)
    int *cont_norm = &cont_tipo[2];          // Acceso directo a la posición 2 (Normales)

    // --- Parámetros de Configuración y Simulación ---
    int espera_act;                          // Tiempo que un cliente lleva esperando
    int tiempo_max, prob_llegada, tipo_rand; // Parámetros de la simulación
    int NUM_CAJAS;                           // Total de cajas disponibles en el banco

    // --- Control de cajas ---
    int caja_libre[4];                       // 1 = Libre, 0 = Ocupada
    int fin_atencion[4];                     // Minuto en el que la caja terminará de atender al cliente actual
    int cliente_en_caja[4];                  // ID del cliente que está siendo atendido actualmente

    // --- Base de Datos Temporal de Clientes ---
    int t_llegada[401];                      // Minuto de llegada al banco
    int atendido[401];                       // Bandera: 1 = Fue atendido, 0 = No atendido aún
    int se_fue[401];                         // Bandera: 1 = Abandonó la fila, 0 = Sigue esperando/atendido
    int tipo[401];                           // Tipo de cliente (1=VIP, 2=Nuevo, 3=Normal)
    int id_num[401];                         // ID único asignado al cliente
    int t_atencion[401];                     // Cuántos minutos tardará su trámite en caja
    int cola[401];                           // Representa físicamente la fila (guarda el ID del cliente)
    int t_espera[401];                       // Total de minutos que el cliente estuvo en la fila
    int t_salida[401];                       // Minuto en el que el cliente salió del banco
    int caja_asignada[401];                  // Número de caja en la que fue atendido (1, 2 o 3)

    // --- Valores Iniciales del Simulador ---
    NUM_CAJAS = 3;
    i = 0;                                    // Total de clientes generados historial
    reloj = 1;                                // La simulación inicia en el minuto 1
    tam_cola = 0;                             // La fila empieza vacía
    *cont_vip = 0;
    *cont_nuevo = 0;
    *cont_norm = 0;
     // --- Genera todas las cajas como vacías y disponibles al abrir el banco ---

    for (c = 1; c <= NUM_CAJAS; c++) {
        caja_libre[c] = 1;
        fin_atencion[c] = 0;
        cliente_en_caja[c] = 0;
    }

    tiempo_max = 400;                          // La simulación correrá durante 400 minutos
    prob_llegada = 60;                         // 60% de probabilidad de que entre un cliente por minuto

    // ==========================================
    // BUCLE PRINCIPAL DE LA SIMULACIÓN (RELOJ)
    // ==========================================
    do {

        if ((rand() % 100) < prob_llegada) {                                               // Generacion de clientes aleatorios

            i = i + 1;                                                                     // Incrementa el contador de clientes.
            t_llegada[i] = reloj;                                                          // Guarda el minuto actual del simulador.
            atendido[i] = 0;                                                               // Inicializa el estado del cliente como NO atendido.
            se_fue[i] = 0;                                                                 // Inicializa el estado como NO se ha ido.
            caja_asignada[i] = 0;                                                          // Indica que todavía no tiene caja asignada.

            tipo_rand = (rand() % 3) + 1;                                                  // Genera un número aleatorio entre 1 y 3.
            tipo[i] = tipo_rand;                                                           // Guarda el tipo generado dentro del cliente actual.
            valores_tipo(i,tipo_rand, cont_tipo,id_num,t_atencion);                        // Llama a la función que asigna características del cliente

            tam_cola = tam_cola + 1;                                                       // Incrementa el tamaño de la cola.
            cola[tam_cola] = i;                                                            // Inserta el cliente al final de la cola.

            j = tam_cola;                                                                  // Inicializa el índice en el último elemento de la cola.
            while (j > 1) {                                                                // Recorre la cola hacia atrás mientras haya al menos dos elementos que comparar.
                if (tipo[cola[j]] < tipo[cola[j-1]]) {                                     // Compara el tipo de cliente actual con el anterior.
                    ord_priori(j,cola);                                                    // Corrige el orden para respetar la prioridad.
                    j = j - 1;                                                             // Continúa revisando hacia atrás por posibles desordenes adicionales.
                } else {
                    j = 0;                                                                 // Si ya está en orden correcto, se detiene el ciclo.
                }
            }
        }

        for (j = 1; j <= tam_cola; j++) {                                                  // Recorre uno por uno todos los clientes que están actualmente en la fila.
            espera_act = reloj - t_llegada[cola[j]];                                       // Calcula el tiempo de espera del cliente actual.
            Alguien_se_va(j,espera_act,cola,tipo,se_fue,id_num);                           // Evalúa si el cliente debe abandonar la fila según su tipo y tiempo de espera.
        }

        k = 0;                                                                             // Representa la nueva posición donde se irán guardando clientes válidos.
        for (j = 1; j <= tam_cola; j++) {                                                  // Recorre todos los clientes que actualmente están en la cola.
            if (se_fue[cola[j]] == 0 && atendido[cola[j]] == 0) {                          // Verifica que el cliente siga activo:
                k = k + 1;                                                                 // Aumenta la posición en la nueva cola.
                cola[k] = cola[j];                                                         // Copia el cliente a la nueva posición de la cola.
            }
        }
        tam_cola = k;                                                                      // Actualiza el tamaño real de la cola después de eliminar clientes.

        for (c = 1; c <= NUM_CAJAS; c++) {                                                 // Recorre todas las cajas disponibles del sistema.
            if (caja_libre[c] == 0 && reloj >= fin_atencion[c]) {                          // Verifica si la caja está ocupada y ya terminó el tiempo de atención.
                atendido[cliente_en_caja[c]] = 1;                                          // Marca al cliente como atendido.
                t_salida[cliente_en_caja[c]] = reloj;                                      // Guarda el minuto exacto en que el cliente salió del sistema.
                caja_libre[c] = 1;                                                         // Libera la caja para que pueda atender a otro cliente.
            }
        }

        for (c = 1; c <= NUM_CAJAS; c++) {                                                 // Recorre todas las cajas disponibles del sistema.
            if (caja_libre[c] == 1 && tam_cola > 0) {                                      // Verifica si la caja está libre y si hay clientes esperando en la cola.
                cliente_en_caja[c] = cola[1];                                              // Toma al primer cliente de la fila (el que está al frente).
                t_espera[cliente_en_caja[c]] = reloj - t_llegada[cliente_en_caja[c]];      // Calcula cuánto tiempo esperó el cliente antes de ser atendido.
                fin_atencion[c] = reloj + t_atencion[cliente_en_caja[c]];                  // Define en qué minuto terminará la atención del cliente.
                caja_libre[c] = 0;                                                         // Marca la caja como ocupada.
                caja_asignada[cliente_en_caja[c]] = c;                                     // Registra en qué caja fue atendido el cliente.

                for (j = 1; j <= tam_cola - 1; j++) {                                      // Desplaza la cola hacia adelante para eliminar al cliente que ya entró a caja.
                    cola[j] = cola[j + 1];
                }
                tam_cola = tam_cola - 1;                                                   // Reduce el tamaño de la cola porque un cliente ya salió de ella.
            }
        }

        reloj = reloj + 1;                                                                 // Avanza el tiempo del sistema en 1 unidad (1 minuto).

    } while (reloj <= tiempo_max);                                                         // El bucle se repite mientras el reloj (tiempo actual) sea menor o igual al tiempo máximo definido.

    // ==========================================
    // CÁLCULOS ESTADÍSTICOS PARA REPORTE FINAL
    // ==========================================
    int tot_atendidos, tot_sefueron;                                                       // Variables que almacenan el total de clientes atendidos y los que abandonaron el sistema.
    double prom_espera;                                                                    // Variable para guardar el promedio de tiempo de espera por caja.
    int suma_temp, cnt_temp;                                                               // Variables auxiliares para cálculos temporales.

    int cnt_caja[4];                                                                       // Contador de clientes atendidos por cada caja.
    int vip_caja[4];                                                                       // Contador de clientes VIP atendidos por cada caja.
    int nuevo_caja[4];                                                                     // Contador de clientes nuevos atendidos por cada caja.
    int norm_caja[4];                                                                      // Contador de clientes normales atendidos por cada caja.
    int sum_espera_caja[4];                                                                // Suma total del tiempo de espera por cada caja.
    int max_espera_caja[4];                                                                // Registra el mayor tiempo de espera por cada caja.

    tot_atendidos = 0;                                                                     // Inicializa el contador total de clientes atendidos en 0.
    tot_sefueron = 0;                                                                      // Inicializa el contador total de clientes que se retiraron en 0.

    for (c = 1; c <= NUM_CAJAS; c++) {                                                     // Recorre cada una de las cajas del sistema para inicializar sus variables de estadísticas.
        cnt_caja[c] = 0;                                                                   // Inicializa en 0 el contador de clientes atendidos por esta caja
        vip_caja[c] = 0;                                                                   // Inicializa en 0 el número de clientes VIP atendidos en esta caja.
        nuevo_caja[c] = 0;                                                                 // Inicializa en 0 el número de clientes nuevos atendidos en esta caja.
        norm_caja[c] = 0;                                                                  // Inicializa en 0 el número de clientes normales atendidos en esta caja.
        sum_espera_caja[c] = 0;                                                            // Inicializa en 0 la suma total de tiempos de espera de esta caja.
        max_espera_caja[c] = 0;                                                            // Inicializa en 0 el mayor tiempo de espera registrado en esta caja.
    }

    for (j = 1; j <= i; j++) {                                                             // Recorre todos los clientes generados durante la simulación.

        if (se_fue[j] == 1) {                                                              // Verifica si el cliente se retiró de la fila sin ser atendido.
            tot_sefueron = tot_sefueron + 1;                                               // Incrementa el contador total de clientes que abandonaron el sistema.
        }

        if (atendido[j] == 1 && caja_asignada[j] > 0) {                                    // Verifica si el cliente fue atendido y tiene una caja asignada válida.
            tot_atendidos = tot_atendidos + 1;                                             // Incrementa el total de clientes atendidos.
            c = caja_asignada[j];                                                          // Obtiene la caja en la que fue atendido el cliente.

            cnt_caja[c] = cnt_caja[c] + 1;                                                 // Aumenta el contador de clientes atendidos en esa caja.
            sum_espera_caja[c] = sum_espera_caja[c] + t_espera[j];                         // Acumula el tiempo de espera del cliente en esa caja.

            if (t_espera[j] > max_espera_caja[c]) {                                        // Compara si el tiempo de espera actual es el mayor registrado.
                max_espera_caja[c] = t_espera[j];                                          // Actualiza el máximo tiempo de espera de la caja.
            }

            switch (tipo[j]) {                                                             // Clasifica al cliente según su tipo.
                case 1: vip_caja[c] = vip_caja[c] + 1; break;                              // Incrementa contador de clientes VIP en esa caja.
                case 2: nuevo_caja[c] = nuevo_caja[c] + 1; break;                          // Incrementa contador de clientes nuevos en esa caja.
                case 3: norm_caja[c] = norm_caja[c] + 1; break;                            // Incrementa contador de clientes normales en esa caja.
            }
        }
    }

    // ==========================================
    // MENU INTERACTIVO POST-SIMULACIÓN
    // ==========================================

    cambiaTamanoLetra(8, 14); // Ajusta tamaño de consola de Windows
    system("color F0"); // Cambia fondo blanco, texto negro


    do{        //MENU//
        system("cls"); // Limpia pantalla
        system("color F2");
        cambiaColorTexto(0,15);
        menuCabecera();


        for(int i=0; i< 2; i++){
            printf("\n");
        }
        printf("ELIGE LA OPCION PARA ANALIZAR ...\n");
        printf("SOLO PUEDE INGRESAR VALORES DE ENTRE (1-6)\n\n");
        printf(" 1)VIP\n 2)NUEVOS\n 3)FRECUENTES\n 4)REPORTE FINAL\n 5)CREDITOS\n 6)SALIR\n\n");

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
            for(int i_x = 1; i_x<=i; i_x++){

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
            for(int i_x = 1; i_x<=i; i_x++){
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
            for(int i_x = 1; i_x<=i; i_x++){
                    //Imprime el ID, la caja de atencion y los minutos de atencion de los clientes Frecuentes

                       if (tipo[i_x]==3){
                            conteo=conteo+1;
                            printf("ID VIP: %d\n",id_num[i_x]);
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
            reporteFinal_menu();
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
            system("cls");
            system("color 07");
            cambiaColorTexto(15,0);
            printf("=====================CREDITOS=====================\n\n");

            printf("==LIDER DE EQUIPO==\n\n");
            printf(" Carlos Jose Gutierrez Cardenas \n Matricula: 010223638\n\n");

            printf("\n==ESPECIALISTA DE LOGICA==\n\n");
            printf(" Victor Hugo Luna Monroy \n Matricula: 010219783\n\n");
            printf(" Jorge Arturo Tapia Perez \n Matricula: 0102240644\n\n");

            printf("\n==ANALISTA DE DATOS==\n\n");
            printf(" Santiago Antonio Rodriguez Chua \n Matricula: 010224172\n\n");

            printf("\n==DESARROLLADOR DE INTERFAZ==\n\n");
            printf(" Seth Carlos Esparragoza Morales \n Matricula: 010223617\n\n");
            printf(" Omar Velasco Hernandez \n Matricula: 010224137\n\n");

            printf("===================================================\n");
            system("pause");
            break;

        case 6:
            //Opcion que da la SALIDA del Sistema de fila de banco
            opc=0;
            break;

        default:
            system("cls");
            cambiaColorTexto(12,15);
            //Mensaje de ERROR en caso de que el usuario se equivoque al ingresar la opcion
            error();
            printf("\nOPCION NO VALIDA \n");
            printf("SOLO PUEDE INGRESAR VALORES DE ENTRE (1-6)\n\n");
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
        case 1:// VIP
            contador[0]= contador[0]+1;
            id[x] = (contador[0]);
            tiempo[x] = (rand() % 3) + 2;// Tardan entre 2 y 4 minutos
            break;
        case 2:// NUEVO
            contador[1]= contador[1]+1;
            id[x] = (contador[1]);
            tiempo[x] = (rand() % 4) + 3;// Tardan entre 3 y 6 minutos
            break;
        case 3:// FRECUENTE
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
        return;
    } else {
        if (tipo[cola[j]] == 2 && espera_act >= 6) {
                // NUEVOS: Menos paciencia (se van a los 6 minutos)
            se_fue[cola[j]] = 1;
            return;
        } else {
            if (tipo[cola[j]] == 3 && espera_act >= 10) {
                // FRECUENTE: Más tolerancia (se van hasta los 10 minutos)
                se_fue[cola[j]] = 1;
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
    printf("   - Frecuentes : %d\n", norm[x]);
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
    printf("   Frecuentes: %d\n", contador[2]);
    printf(" Clientes atendidos: %d\n", atendidos);
    printf(" Clientes que se fueron: %d\n", se_fueron);
    return;
}

// ==========================================
// GRAFICOS DE CONSOLA Y MENUS (ASCII ART)
// ==========================================
void menuCabecera(void){
printf("           /$$                         /$$                 /$$                            /$$$$$$  /$$ /$$                       /$$                 /$$                                                        \n");
printf("          |__/                        | $$                | $$                           /$$__  $$|__/| $$                      | $$                | $$                                                        \n");
printf("  /$$$$$$$ /$$ /$$$$$$/$$$$  /$$   /$$| $$  /$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$       | $$  \\__/ /$$| $$  /$$$$$$         /$$$$$$$  /$$$$$$       | $$$$$$$   /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$          \n");
printf(" /$$_____/| $$| $$_  $$_  $$| $$  | $$| $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$      | $$$$    | $$| $$ |____  $$       /$$__  $$ /$$__  $$      | $$__  $$ |____  $$| $$__  $$ /$$_____/ /$$__  $$          \n");
printf("|  $$$$$$ | $$| $$ \\ $$ \\ $$| $$  | $$| $$  /$$$$$$$| $$  | $$| $$  \\ $$| $$  \\__/      | $$_/    | $$| $$  /$$$$$$$      | $$  | $$| $$$$$$$$      | $$  \\ $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$   \n");
printf(" \\____  $$| $$| $$ | $$ | $$| $$  | $$| $$ /$$__  $$| $$  | $$| $$  | $$| $$            | $$      | $$| $$ /$$__  $$      | $$  | $$| $$_____/      | $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$         \n");
printf(" /$$$$$$$/| $$| $$ | $$ | $$|  $$$$$$/| $$|  $$$$$$$|  $$$$$$$|  $$$$$$/| $$            | $$      | $$| $$|  $$$$$$$      |  $$$$$$$|  $$$$$$$      | $$$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$/          \n");
printf("|_______/ |__/|__/ |__/ |__/ \______/ |__/ \\_______/ \\_______/ \\______/ |__/            |__/      |__/|__/ \\_______/       \\_______/ \\_______/      |_______/  \\_______/|__/  |__/ \\_______/ \\______/  \n");



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
printf("  /$$$$$$                                                              /$$         \n");
printf(" /$$__  $$                                                            | $$          \n");
printf("| $$  \\__//$$$$$$   /$$$$$$   /$$$$$$$ /$$   /$$  /$$$$$$  /$$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$$     \n");
printf("| $$$$   /$$__  $$ /$$__  $$ /$$_____/| $$  | $$ /$$__  $$| $$__  $$|_  $$_/   /$$__  $$ /$$_____/      \n");
printf("| $$_/  | $$  \\__/| $$$$$$$$| $$      | $$  | $$| $$$$$$$$| $$  \\ $$  | $$    | $$$$$$$$|  $$$$$$       \n");
printf("| $$    | $$      | $$_____/| $$      | $$  | $$| $$_____/| $$  | $$  | $$ /$$| $$_____/ \\____  $$      \n");
printf("| $$    | $$      |  $$$$$$$|  $$$$$$$|  $$$$$$/|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$$ /$$$$$$$/      \n");
printf("|__/    |__/       \\_______/ \\_______/ \\______/  \\_______/|__/  |__/   \\___/   \\_______/|_______/  \n");
printf("----------------------------------------------------------------------------------------------------------------\n");
}

void menuOpcionC(void){
printf("           $$\\ \n");
printf("           \\__|     \n");
printf("$$\\    $$\\ $$\\  $$$$$$\\  \n");
printf("\\$$\\  $$  |$$ |$$  __$$\\ \n");
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

void reporteFinal_menu
(void){
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
