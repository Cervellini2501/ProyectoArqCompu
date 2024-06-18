#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
//#include <ncurses.h>
#include "Assembly.s"
#include "EasyPIO.h"  // AsegÃºrate de tener esta librerÃ­a para controlar los GPIO


// Definiciones
#define PASSWORD_LENGTH 5   // Longitud de la contraseÃ±a
#define NUM_LEDS 8          // Numero de LEDs
#define MAX_ATTEMPTS 3 // nÃºmero de intentos permitidos

// Tabla de datos
unsigned char TablaCh[] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};

// Tabla para patrones de expansiÃ³n de onda
unsigned char TablaExpansiva[8][8] = {
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // 1 LED
    {0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // 2 LEDs
    {0x07, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00},  // 3 LEDs
    {0x0F, 0x0E, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x00},  // 4 LEDs
    {0x1F, 0x1E, 0x1C, 0x18, 0x10, 0x00, 0x00, 0x00},  // 5 LEDs
    {0x3F, 0x3E, 0x3C, 0x38, 0x30, 0x20, 0x00, 0x00},  // 6 LEDs
    {0x7F, 0x7E, 0x7C, 0x78, 0x70, 0x60, 0x40, 0x00},  // 7 LEDs
    {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80}   // 8 LEDs
};

// Prototipos de funciones
int menu(void);
int ingreso(char *password);
int presskey(void);
int delay(unsigned long int *a);
void disp_binary(int);
void autof(void);
void ChoqueT(void);
void simulador_balizas(void);
void expansion_ondas(void);
void keyHit(int index);
void pinSetup(void);
void ledShow(unsigned char output);
void turnOff();

const unsigned char led[NUM_LEDS] = {14, 15, 18, 23, 24, 25, 8, 7}; // Pines de los LEDs
int delayTime[] = {10000, 10000, 10000, 10000};

// Variable global para controlar la velocidad
int speed = 10;

int menu(void) {
    printf("\nAccediendo al menu . .  .\n");
    int opc = 0;

    do {
        printf("\n--- MENU ---\n");
        printf("1) Auto fantÃ¡stico\n");
        printf("2) Choque\n");
        printf("3) Simulador de balizas\n");
        printf("4) ExpansiÃ³n de ondas\n");
        printf("5) Salir\n");
        printf("Elija su opciÃ³n: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                    autof();
                    break;
                case 2:
                    ChoqueT();
                    break;
                case 3:
                    simulador_balizas();
                    break;
                case 4:
                    expansion_ondas();
                    break;
                case 5:
                    printf("\nSaliendo. . .\n") ;
                    break;
                default:
                    printf("\nElija una opciÃ³n valida") ;
        }
    } while(opc != 5) ;
}

int ingreso(char *password) {
    
    printf("Ingrese clave: ");
    int i = 0;
    
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        password[i] = getchar(); // Leer caracter por caracter
        printf("*");             // Mostrar un asterisco por cada caracter
        fflush(stdout);          // Vaciar el buffer de salida
    }
    password[i] = '\0';
    printf("\n") ;
    
    return strcmp(password,"12345") == 0 ;
}

int presskey(void) {

    char ch = getchar();
    if (ch == 'a') {
        return 0;
    } else if (ch == 'u') {
        speed = (speed > 1) ? speed - 1 : speed; // Aumentar velocidad
    } else if (ch == 'd') {
        speed++; // Disminuir velocidad
    }
    getchar();
    return 1;
}

int delay(unsigned long int *a) {
    for (int i = delayTime[&a]; i > 0; i -= 100) {
        usleep(100); // Retardo en microsegundos
        if (keyHit(&a)) {
            return 0; // Si se presiono una tecla, salir
        }
    }
    return 1; // Continuar si no se presiono una tecla
}


void disp_binary(int i) {
    for (int t = 128; t > 0; t = t / 2) {
        if (i & t) {
            printf("1"); //bit encendido
        } else {
            printf("0");  //bit apagado
        }
    }
    fflush(stdout); // Vaciar el buffer de salida
    printf("\n");
    Leds(i) ;
}

void autof(void) {
    printf("\n--- AUTO FANTASTICO ---\n");
    // printf("Ingrese esc para finalizar la secuencia\n");
    printf("Ingrese u para aumentar la velocidad\n");
    printf("Ingrese d para disminuir la velocidad\n");

    unsigned char output;

    unsigned long int speed=400 ;

    while (1) {
        output = 0x80; // Comenzar con el bit mas significativo
        for (int i = 0; i < NUM_LEDS; i++) {
            ledShow(output); // Mostrar el valor en los LEDs
            disp_binary(output); // Mostrar el valor en binario en la pantalla
            output >>= 1; // Desplazar a la derecha
            delay(&speed);
        }
        for (int i = 64; i > 0; i = i / 2) {

            
            delay(&speed);
            printf("Light pattern: %d\n", i);
            if (presskey() == 0) {
                return;
            }
        }
        output = 0x02; // Comenzar con el segundo bit mas bajo

        for (int i = 0; i < 6; i++) {
            ledShow(output); // Mostrar el valor en los LEDs
            disp_binary(output); // Mostrar el valor en binario en la pantalla
            output <<= 1; // Desplazar a la izquierda

            delay(&speed) ;

            printf("Light pattern: %d\n", i);
            if (presskey() == 0) {
                return;
            }
        }
    }
    if ((getch()) == 'o') { //si se toca "o" sale del bucle
         return;
    }
}

void ChoqueT(void) {
    printf("\n--- CHOQUE ---\n");
    // printf("Ingrese esc para finalizar la secuencia\n");
    printf("Ingrese u para aumentar la velocidad\n");
    printf("Ingrese d para disminuir la velocidad\n");

    unsigned long int speed=400 ;

    for (int i = 0; i < NUM_LEDS; i++) {
        disp_binary(TablaCh[i]) ;
        Leds(-TablaCh[i]) ;
        delay(&speed) ;
    }
    if ((getch()) == 'o') { //si se toca "o" sale del bucle
         return;
    }
}

void simulador_balizas(void) {
    printf("\n--- SIMULADOR BALIZAS ---\n");
    // printf("Ingrese esc para finalizar la secuencia\n");
    printf("Ingrese u para aumentar la velocidad\n");
    printf("Ingrese d para disminuir la velocidad\n");

    unsigned long int speed=400 ;

    int num_cycles = 10;  // NÃºmero de ciclos a ejecutar
    int balizas[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Inicializar las luces de emergencia apagadas

    for (int cycle = 0; cycle < num_cycles; cycle++) {
        // Encender luces
        balizas[0] = 1; 
        balizas[1] = 1;
        balizas[4] = 1; 
        balizas[5] = 1; 
        balizas[2] = 0; 
        balizas[3] = 0; 
        balizas[6] = 0; 
        balizas[7] = 0; 

        delay(&speed); // Esperar el retardo

        // Alternar a otro estado
        balizas[0] = 0; 
        balizas[1] = 0;
        balizas[4] = 0; 
        balizas[5] = 0; 
        balizas[2] = 1; 
        balizas[3] = 1; 
        balizas[6] = 1; 
        balizas[7] = 1; 

        delay(&speed); // Esperar el retardo
    }
    if ((getch()) == 'o') { //si se toca "o" sale del bucle
         return;
    }
}
 

void expansion_ondas(void) {
    printf("\n--- EXPANSION DE ONDAS ---\n");
    // printf("Ingrese esc para finalizar la secuencia\n");
    printf("Ingrese u para aumentar la velocidad\n");
    printf("Ingrese d para disminuir la velocidad\n");

    unsigned long int speed=400 ;

    int numLedsEncendidos = 1;
    while (numLedsEncendidos <= NUM_LEDS) {
        for (int i = 0; i < NUM_LEDS ; i++) {
            delay(&speed);  // Ajusta el tiempo de acuerdo a tus necesidades
        }
        numLedsEncendidos++;
    }
    if ((getch()) == 'o') { //si se toca "o" sale del bucle
         return;
    }
}

// Verificar la pulsaciÃ³n de teclas
int keyHit(int index) {
    int ch;
    ch = getchar(); // Leer caracter

    if (ch == 'u' && delayTime[index] > 100) {
        delayTime[index] -= 100; // Aumentar la velocidad
    } else if (ch == 'd') {
        delayTime[index] += 100; // Disminuir la velocidad
    } else {
        return 1; // Indica que se presionó una tecla que no es 'u' ni 'd'
    }
    return 0; // No se presionó una tecla de salida
}

// Inicializar los pines
void pinSetup(void) {
    pioInit(); // Inicializar EasyPIO
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(led[i], OUTPUT); // Establecer pines como salida
    }
}

// Mostrar LEDs
void ledShow(unsigned char output) {
    for (int j = 0; j < NUM_LEDS; j++) {
        digitalWrite(led[j], (output >> j) & 1); // Escribir valor en los pines
    }
}

// Apagar los LEDs
void turnOff() {
    unsigned char off = 0x00; // Valor para apagar los LEDs
    ledShow(off); // Apagar los LEDs
}

int Leds(int number){
    int i;
    for(int i = 0; i < NUM_LEDS ; i++){
        number = (number >> i) & 0x01 ;
        digitalWrite(led[i], number) ;
    }
    delayMillis(300) ;
    return 0 ;
}

int main(void) {
    pioInit();
    pinSetup();

    char CLAVE[] = "12345";
    char clave[6] ;

    char caracter ;

    int intento = 0 ;

    for (int i = 0; i < NUM_LEDS ; i++) {
        pinMode(led[i], OUTPUT);
    }
    
    // char setPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4', '5'}; // ContraseÃ±a predeterminada
    // char passwordInput[PASSWORD_LENGTH]; // Arreglo para la contraseÃ±a ingresada por el usuario



    do {
        i = 0;
        printf("\n\t Ingrese su clave: ");
        refresh();

        while ((caracter = getch()) != '\n') {
            if (caracter == 127) {  // Retroceso
                if (i > 0) {
                    i--;
                    printw("\b \b");
                    refresh();
                }
            }
            else {
                if (i < 5) {
                    printw("*");
                    refresh();
                    clave[i] = caracter;
                    i++;
                }
            }
        }
        clave[i] = '\0';  // Terminar la cadena de caracteres con el carácter nulo

        if (strcmp(clave, CLAVE) == 0) {
            ingresar = 1;
        }
        else {
            printw("\n\tClave incorrecta\n");
            intentos++;
        }
        refresh();

    } while (intentos < 3 && ingresar == 0);

    endwin(); // Finalizar pantalla

    if (ingresar == 0) {
        printf("\n\tNumero de intentos superados, abortando programa\n");
    }
    else {







    while (intento < MAX_ATTEMPTS) {
        ingreso(passwordInput);
        
        // Verificar si la longitud de la contraseÃ±a ingresada es vÃ¡lida
        if (strlen(passwordInput) != PASSWORD_LENGTH) {
            printf("La contraseÃ±a debe tener exactamente 5 caracteres.\n", PASSWORD_LENGTH);
            intento++;
            continue;
        }
        
        // Verificar contraseÃ±a
        if (strcmp(setPassword, passwordInput) == 0) {
            printf("\nBienvenido al sistema ! ! !\n");
            menu();
            return 0;
        } else {
            printf("\nContraseÃ±a no vÃ¡lida\n");
            intento++;
        }
    }

    if(intento > MAX_ATTEMPTS){
        printf("Ha superado el nÃºmero mÃ¡ximo de intentos fallidos.\n");
        return 1;
    }
    
    return 0;
}

