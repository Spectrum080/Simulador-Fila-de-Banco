#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include <string.h>



int main()
{

    int opc;


    do{ //inicio del menu

        system("cls"); //limpia la consola

        printf("Coloca el numero de la opcion que quieres realizar\n");
        printf(" 1)A\n 2)B\n 3)C\n 4)Salir\n");
        printf("Opcion: ");

        opc=-1; //reinicia el valor de opc
        scanf("%d", &opc);

        while (getchar() != '\n'); //limpia el bufer

        switch(opc){ 
        case 1:
            //sentencias
            system("cls"); 
            printf("Esta es la opcion A\n");
            system("pause");
            break;

        case 2:
            //sentencias
            system("cls");
            printf("Esta es la opcion B\n");
            system("pause");
            break;

        case 3:
            //sentencias
            system("cls");
            printf("Esta es la opcion C\n");
            system("pause");
            break;

        case 4:
            opc=0;
            break;

        default:
            printf("Opcion no valida, seleccione la opcion nuevamente\n");
            system("pause");
            break;
        }

    }while(opc!=0); //fin del menu

    system("cls");

    printf("Termina el menu");
    return 0;
}
