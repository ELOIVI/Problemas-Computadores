@;
@; Author: Arey Ferrero Ramos.
@;

    @; RSI del vertical blank.
RSI_vblank:
    push {r0-r3, lr}


    @; Necesitamos poder controlar el estado de paused
    ldr r1, =paused
    ldrb r0, [r1]           @; Cargar el valor de paused (1 byte)
    cmp r0, #0              @; ¿Es cero?
    bne fin_rsi 

    @; Actualizar el ang_actual
    ldr r1, =ang_actual
    ldr r0, [r1]
    ldr r2, =fraccion
    ldr r2, [r2]
    add r0, r2                   @; Se suma la fracción al índice 'ang_actual' para generar la nueva posición del vector de ángulos a la que se tendrá que acceder.

    @; Control de desbordamiento, es decir, si se pasa de 360, se le resta y vuelve a empezar
    mov r2, #360
    mov r2, r2, lsl #12          @; Se carga el valor 360 en formato Q12.
    cmp r0, r2                   @; Si se ha llegado al valor límite del índice 'ang_actual'
    subge r0, r2                 @; se le resta 360 (para mantener el valor acumulado de la fracción).
    str r0, [r1]
    
    @; Obtener índice entero 
    mov r0, r0, lsr #12          @; Se elimina la parte decimal (Q12) del ángulo actual porque las posiciones de los vectores siempre deben ser enteras.

    @; Cargar coordenadas del vector
    ldr r3, =ang_pos             @; Se carga la dirección base del vector de ángulos.
    mov r0, r0, lsl #2
    ldrh r1, [r3, r0]            @; R1 = px (ángulo actual).
    add r0, #2
    ldrh r2, [r3, r0]            @; R2 = py (ángulo actual)
    
    @; Mover sprite
    mov r0, #0                   @; debemos poner R0 de nuevo a 0, puesto que se ha modificado y el enunciado nos pide que sea con el sprite 0.
    bl SPR_moverSprite
    
    mov r0, #0x07000000		     @; R0 = OAM.
    mov r1, #1                   @; R1 = Número de sprites.
    bl SPR_actualizarSprites
    
    bl activar_beat              @; Se llama activar_beat desde la RSI (Justificación en el archivo 'activar_beat.md').
    
fin_rsi:
    pop {r0-r3, pc}
