.section .data
balizas: .byte 0x33, 0xCC  @ Definir los estados de las balizas
tablaEs: .byte 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00

.section .text
.global simulador_balizas
.global listadeEspera
.extern mostrar_leds
.extern display_binary
.extern retardo
.extern apagar_leds
.extern teclas

simulador_balizas:
    PUSH {R4, R5, LR}          @ Guardar los registros usados

    LDR R4, =balizas           @ Cargar la dirección de la tabla de balizas
    MOV R5, #2                 @ Número de estados de balizas
    MOV R6, #2                 @ Índice para retardo(2)

loop_balizas:
    LDRB R0, [R4], #1          @ Cargar el estado de balizas y avanzar al siguiente estado
    BL mostrar_leds            @ Llamar a mostrar_leds
    MOV R0, R0                 @ Mover el byte a R0 para display_binary
    BL display_binary          @ Llamar a display_binary
    MOV R0, R6                 @ Índice para retardo
    BL retardo                 @ Llamar a retardo
    CMP R0, #0                 @ Comprobar el resultado de retardo
    BEQ apagar_simulador_balizas  @ Si retardo devuelve 0, apagar LEDs y finalizar

    SUBS R5, R5, #1            @ Decrementar el contador de estados
    BNE loop_balizas           @ Repetir si no ha terminado

    MOV R5, #2                 @ Reiniciar el contador de estados
    LDR R4, =balizas           @ Reiniciar la dirección de la tabla de balizas
    B loop_balizas             @ Repetir el bucle

apagar_simulador_balizas:
    BL apagar_leds             @ Apagar los LEDs
    POP {R4, R5, PC}           @ Restaurar los registros y regresar

listadeEspera:
    PUSH {R4, R5, R6, LR}        @ Guardar registros usados

    LDR R4, =tablaEs             @ Cargar la dirección base de la tabla
    MOV R5, #0                   @ Inicializar el índice a 0
    MOV R6, #16                  @ Tamaño de la tabla

loop_listadeEspera:
    LDRB R0, [R4, R5]            @ Cargar el byte actual de la tabla
    BL mostrar_leds              @ Llamar a mostrar_leds
    MOV R0, R0                   @ Mover el byte a R0 para display_binary
    BL display_binary            @ Llamar a display_binary

    MOV R0, #0                   @ Índice para retardo
    BL retardo                   @ Llamar a retardo
    CMP R0, #0                   @ Comprobar el resultado de retardo
    BEQ apagar_listadeEspera     @ Si retardo devuelve 0, apagar LEDs y finalizar

    BL teclas                    @ Comprobar las teclas
    CMP R0, #27                  @ Comprobar si se presionó ESC (27 en ASCII)
    BEQ apagar_listadeEspera     @ Si se presionó ESC, apagar LEDs y finalizar

    ADD R5, R5, #1               @ Incrementar el índice
    CMP R5, R6                   @ Comparar el índice con el tamaño de la tabla
    BNE loop_listadeEspera       @ Si no se ha alcanzado el final de la tabla, repetir

    MOV R5, #0                   @ Reiniciar el índice a 0
    B loop_listadeEspera         @ Repetir el bucle

apagar_listadeEspera:
    BL apagar_leds               @ Apagar los LEDs
    POP {R4, R5, R6, PC}         @ Restaurar registros y regresar