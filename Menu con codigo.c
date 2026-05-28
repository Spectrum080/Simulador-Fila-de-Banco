#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



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

void sim_fila_banco(void){
printf("  /$$$$$$  /$$                         /$$                 /$$                                 /$$                  /$$$$$$  /$$ /$$                       /$$                 /$$\n");
printf(" /$$__  $$|__/                        | $$                | $$                                | $$                 /$$__  $$|__/| $$                      | $$                | $$\n");
printf("| $$  \\__/ /$$ /$$$$$$/$$$$  /$$   /$$| $$  /$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$         /$$$$$$$  /$$$$$$       | $$  \\__/ /$$| $$  /$$$$$$         /$$$$$$$  /$$$$$$       | $$$$$$$   /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$\n");
printf("|  $$$$$$ | $$| $$_  $$_  $$| $$  | $$| $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$       /$$__  $$ /$$__  $$      | $$$$    | $$| $$ |____  $$       /$$__  $$ /$$__  $$      | $$__  $$ |____  $$| $$__  $$ /$$_____/ /$$__  $$\n");
printf(" \\____  $$| $$| $$ \\ $$ \\ $$| $$  | $$| $$  /$$$$$$$| $$  | $$| $$  \\ $$| $$  \\__/      | $$  | $$| $$$$$$$$      | $$_/    | $$| $$  /$$$$$$$      | $$  | $$| $$$$$$$$      | $$  \\ $$  /$$$$$$$| $$  \\ $$| $$      | $$  \ $$\n");
printf(" /$$  \\ $$| $$| $$ | $$ | $$| $$  | $$| $$ /$$__  $$| $$  | $$| $$  | $$| $$            | $$  | $$| $$_____/      | $$      | $$| $$ /$$__  $$      | $$  | $$| $$_____/      | $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$\n");
printf("|  $$$$$$/| $$| $$ | $$ | $$|  $$$$$$/| $$|  $$$$$$$|  $$$$$$$|  $$$$$$/| $$            |  $$$$$$$|  $$$$$$$      | $$      | $$| $$|  $$$$$$$      |  $$$$$$$|  $$$$$$$      | $$$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$/\n");
printf(" \\______/ |__/|__/ |__/ |__/ \\______/ |__/ \\_______/ \\_______/ \\______/ |__/             \\_______/ \\_______/      |__/      |__/|__/ \\_______/       \\_______/ \\_______/      |_______/  \\_______/|__/  |__/ \\_______/ \\______/ \n");
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

int main()
{
    int opc;
    int cont_tipo[3];
    int *cont_vip = &cont_tipo[0];
    int *cont_nuevo = &cont_tipo[1];
    int *cont_norm = &cont_tipo[2];

    cambiaTamanoLetra(8, 14);

    system("color F0");

    do{
        system("cls");
        system("color F0");
        cambiaColorTexto(0,15);
        //menuCabecera();
        sim_fila_banco();
        //menuacotado(1,1,1,cont_tipo);

        printf("Coloca el numero de la opcion que quieres realizar\n");
        printf(" 1)A\n 2)B\n 3)C\n 4)Salir\n");
        printf("Opcion: ");

        opc=-1;
        scanf("%d", &opc);

        while (getchar() != '\n');

        switch(opc){
        case 1:
            system("cls");
            system("color 07");
            cambiaColorTexto(15,0);
            menuOpcionA();
            system("pause");
            break;

        case 2:
            system("cls");
            printf("Esta es la opcion B\n");
            system("pause");
            break;

        case 3:
            system("cls");
           system("color 07");
            cambiaColorTexto(15,0);
            reporteFinal();
            system("pause");
            break;

        case 4:

            opc=0;
            break;

        default:
            cambiaColorTexto(15,4);
            printf("Opcion no valida, seleccione la opcion nuevamente\n");
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
