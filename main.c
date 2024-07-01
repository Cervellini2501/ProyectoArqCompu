#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ncurses.h>
#include <time.h>
#include "EasyPIO.h"
#include "funciones_ass.h"

// Definiciones
#define PASSWORD_LENGTH 5   // Longitud de la contrasena
#define NUM_LEDS 8          // Numero de LEDs

// Declaraciones de funciones
void display_binary(int);                     // Mostrar valor en binario
void contrasenia(char *password);             // Obtener la contrasena del usuario
void mostrar_menu();                          // Mostrar el menu principal
void autoFantastico();                        // Secuencia "Auto Fantastico"
void choque();                                // Secuencia "Choque"
//void simulador_balizas();                   // Secuencia "Simulador Balizas"
//void listadeEspera(void) ;                  // Secuencia "Lista de Espera"
struct termios modifyTerminalConfig(void);    // Configurar terminal
void restoreTerminalConfig(struct termios);   // Restaurar configuracion del terminal
bool teclas(int index);                       // Verificar pulsacion de teclas
void pines(void);                             // Configurar pines
int retardo(int index);                       // Funcion de retardo
void clearInputBuffer();                      // Limpiar el buffer de entrada 
void apagar_leds();                           // Apagar los LEDs
void mostrar_leds(unsigned char output);      // Mostrar LEDs
void adjustSpeed(int index);                  // Ajustar la velocidad

// Variables globales
const unsigned char led[NUM_LEDS] = {14, 15, 18, 23, 24, 25, 8, 7}; // Pines de los LEDs
int retardoTime[] = {500000, 500000, 500000, 500000};               // Tiempo de retardo inicial (0.5 segundos)

// Funcion principal
int main(void) {
    pines(); // Inicializar los pines de los LEDs
    apagar_leds() ;
    char setPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4', '5'}; // Contrasena predeterminada
    char passwordInput[PASSWORD_LENGTH]; // Arreglo para la contrasena ingresada por el usuario
    // Recepcion de la contrasena y validacion
    for (int i = 0; i < 3; i++) { // Permitir hasta 3 intentos
        bool passwordFlag = true; // Bandera para indicar si la contrasena es correcta
        contrasenia(passwordInput); // Obtener la contrasena del usuario
        // Verificar la contrasenia
        for (int j = 0; j < PASSWORD_LENGTH; j++) {
            if (setPassword[j] != passwordInput[j]) { // Comparar caracteres
                passwordFlag = false; // Si hay una diferencia, la contrasena es incorrecta
                break;
            }
        }
        // Si la contrasenia es correcta, mostrar el menu
        if (passwordFlag) {
            printf("\n============  ¡Acceso concedido! Bienvenido ! ! !\n");
            mostrar_menu();
            printf("\n============  Sesion terminada, hasta pronto!  n");
            break;
        } else {
            printf("\n-------------------------------------\n");
            printf("    La clave es incorrecta, intente nuevamente\n");
            printf("-------------------------------------\n\n");
        }
    }
    return 0; // Terminar el programa
}

// Funcion para mostrar un valor en binario
void display_binary(int i) {
    int t;
    for (t = 128; t > 0; t = t / 2) { // Iterar sobre los bits
        if (i & t) printf("* ");      // Si el bit esta encendido, mostrar "*"
        else printf("_ ");            // Si el bit esta apagado, mostrar "_"
    }
    fflush(stdout); // Vaciar el buffer de salida
    printf("\r");   // Retorno de carro
}

// Funcion para obtener la contrasenia del usuario
void contrasenia(char *password) {
    struct termios oldattr = modifyTerminalConfig(); // Configurar el terminal
    printf("Ingrese su clave: ");
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        password[i] = getchar(); // Leer caracter por caracter
        printf("*");             // Mostrar un asterisco por cada caracter
        fflush(stdout);          // Vaciar el buffer de salida
    }
    restoreTerminalConfig(oldattr); // Restaurar configuracion del terminal
    printf("\n");
}

// Funcion para mostrar el menu
void mostrar_menu() {
    int option;
    do {
        clearInputBuffer(); // Limpiar el buffer de entrada
        printf("\n=====================================\n");
        printf("            MENU PRINCIPAL            \n");
        printf("=====================================\n");
        printf("1. Auto Fantastico                   \n");
        printf("2. El Choque                         \n");
        printf("3. Simulador Balizas                 \n");
        printf("4. Expansión de Ondas                \n");
        printf("5. Salir                             \n");
        printf("=====================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &option); // Leer la opcion seleccionada
        printf("\n");

        switch (option) {
            case 1:
                autoFantastico(); // Ejecutar la secuencia "Auto Fantastico"
                break;
            case 2:
                choque(); // Ejecutar la secuencia "Choque"
                break;
            case 3:
                simulador_balizas(); // Ejecutar la secuencia "Simulador Balizas"
                break;
            case 4:
                listadeEspera(); // Ejecutar la secuencia "Expansión de Ondas"
                break;
            case 5:
                printf("Saliendo...\n"); // Salir del programa
                break;
            default:
                printf("Seleccione una opcion valida\n"); // Mensaje de error
        }
    } while (option != 5); // Repetir hasta que se seleccione la opcion de salir
}

// Secuencia "Auto Fantastico"
void autoFantastico() {
    printf("\n--- Auto Fantastico ---\n");
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione u para aumentar la velocidad\n");
    printf("Presione d para disminuir la velocidad\n");
    unsigned char output;
    while (true) {
        output = 0x80; // Comenzar con el bit mas significativo
        for (int i = 0; i < NUM_LEDS; i++) {
            mostrar_leds(output); // Mostrar el valor en los LEDs
            display_binary(output); // Mostrar el valor en binario en la pantalla
            output >>= 1; // Desplazar a la derecha
            if (retardo(0) == 0) { // Esperar el retardo y verificar si se presiono una tecla
                apagar_leds(); // Apagar los LEDs
                return; // Salir de la secuencia
            }
        }
        output = 0x02; // Comenzar con el segundo bit mas bajo
        for (int i = 0; i < 6; i++) {
            mostrar_leds(output); // Mostrar el valor en los LEDs
            display_binary(output); // Mostrar el valor en binario en la pantalla
            output <<= 1; // Desplazar a la izquierda
            if (retardo(0) == 0) { // Esperar el retardo y verificar si se presiono una tecla
                apagar_leds(); // Apagar los LEDs
                return; // Salir de la secuencia
            }
        }
    }
}

// Secuencia "Choque"
void choque() {
    printf("\n--- Choque ---\n");
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione u para aumentar la velocidad\n");
    printf("Presione d para disminuir la velocidad\n");
    int tablaCh[8] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
    while (true) {
        for (int i = 0; i < 8; i++) {
            mostrar_leds(tablaCh[i]);       // Mostrar el valor en los LEDs
            display_binary(tablaCh[i]);     // Mostrar el valor en binario en la pantalla
            if (retardo(1) == 0) {      // Esperar el retardo y verificar si se presionó una tecla
                apagar_leds();          // Apagar los LEDs
                return;                 // Salir de la secuencia
            }
        }
    }
}
/*
// Secuencia "Simulador Balizas"
void simulador_balizas() {
    printf("\n--- SIMULADOR BALIZAS ---\n");
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione u para aumentar la velocidad\n");
    printf("Presione d para disminuir la velocidad\n");

    unsigned char balizas[2] = {0x33, 0xCC}; // 0x33 = 00110011 (0, 1, 4, 5 encendidos), 0xCC = 11001100 (2, 3, 6, 7 encendidos)

    while (true) {
        for (int i = 0; i < 2; i++) {
            mostrar_leds(balizas[i]);       // Mostrar el estado en los LEDs
            display_binary(balizas[i]);     // Mostrar el valor en binario en la pantalla
            if (retardo(2) == 0) {          // Esperar el retardo y verificar si se presionó una tecla
                apagar_leds();              // Apagar los LEDs
                return;                     // Salir de la secuencia
            }
        }
    }
}


void listadeEspera(void) {
    printf("\n--- LISTA DE ESPERA ---\n");
    printf("Presione esc para finalizar la secuencia\n");
    printf("Ingrese u para aumentar la velocidad\n");
    printf("Ingrese d para disminuir la velocidad\n");
   
    unsigned char tablaEs[] = {
            0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, // Ola moviéndose de izquierda a derecha
            0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00  // Ola moviéndose de derecha a izquierda
    };

    do {
        for (int i = 0; i < sizeof(tablaEs); i++) {
            mostrar_leds(tablaEs[i]);
            display_binary(tablaEs[i]);
            if (retardo(0) == 0) { // Utilizar retardo en lugar de delay
                endwin();
                return;
            }
            refresh();

            if (teclas(0)) { // Comprobar las teclas
                endwin();
                return;
            }
        }
    } while (1); // Repite hasta que se toque una tecla

    endwin(); // Finalizar ncurses
}
*/

// Configuracion de la terminal
struct termios modifyTerminalConfig(void) {
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr); // Obtener configuracion actual del terminal
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // Desactivar modo canonico y eco
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Aplicar nueva configuracion
    return oldattr; // Devolver configuracion anterior
}

// Restaurar la configuracion del terminal
void restoreTerminalConfig(struct termios oldattr) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restaurar configuracion anterior
}

// Verificar la pulsacion de teclas
bool teclas(int index) {
    struct termios oldattr = modifyTerminalConfig();
    int ch, oldf;
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // Obtener flags actuales
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Establecer modo no bloqueante
    ch = getchar(); // Leer caracter
    if (ch == 'u' && retardoTime[index] > 100) {
        retardoTime[index] -= 100000; // Aumentar la velocidad
    }
    if (ch == 'd') {
        retardoTime[index] += 100000; // Disminuir la velocidad
    }
    restoreTerminalConfig(oldattr); // Restaurar configuracion del terminal
    fcntl(STDIN_FILENO, F_SETFL, oldf); // Restaurar flags originales
    if (ch == 27) { // Si se presiona ESC
        ungetc(ch, stdin); // Devolver caracter al buffer de entrada
        return true; // Indicar que se presiono ESC
    }
    return false; // No se presiono ESC
}

// Inicializar los pines
void pines(void) {
    pioInit(); // Inicializar EasyPIO
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(led[i], OUTPUT); // Establecer pines como salida
        digitalWrite(led[i],0) ; 
    }
}

// Mostrar LEDs
void mostrar_leds(unsigned char output) {
    for (int j = 0; j < NUM_LEDS; j++) {
        digitalWrite(led[j], (output >> j) & 1); // Escribir valor en los pines
    }
}

// Funcion de retardo para las secuencias
int retardo(int index) {
    for (int i = retardoTime[index]; i > 0; i -= 100000) { // Aumentar el decremento para un retardo más largo
        usleep(100000); // Retardo en microsegundos
        if (teclas(index)) {
            return 0; // Si se presiono una tecla, salir
        }
    }
    return 1; // Continuar si no se presiono una tecla
}

// Limpiar el buffer de entrada
void clearInputBuffer() {
    printf("Presione ENTER para confirmar\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Descartar caracteres
    }
}

// Apagar los LEDs
void apagar_leds() {
    unsigned char off = 0x00; // Valor para apagar los LEDs
    mostrar_leds(off); // Apagar los LEDs
}

// Definición de la función adjustSpeed
void adjustSpeed(int index) {
    // Ajustar la velocidad basándose en el valor actual de retardoTime
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (ch == 'u' && retardoTime[index] > 100) {
            retardoTime[index] -= 100000; // Aumentar la velocidad
        } else if (ch == 'd') {
            retardoTime[index] += 100000; // Disminuir la velocidad
        }
    }
}