//
// Author: Arey Ferrero Ramos.
//

typedef struct                                          // Coordenadas de sprites.
{
    short px;                                           // [-64..288]
    short py;                                           // [-32..224]
} t_pos;

t_pos ang_pos[360];                                     // Vector con posiciones (px, py) para cada ángulo.

unsigned char bpm = 60;                                 // "Beats" por minuto.
unsigned char accent = 4;                               // Divisiones para generar el acento.
unsigned char paused = !0;                              // Metrónomo pausado? (!0: sí, 0: no).

unsigned int ang_actual = 0;                            // Ángulo actual (en formato Q12).
unsigned int fraccion;								    // Incremento del ángulo en un VBL (Q12).
														// No se puede inicializar aquí porque las 
                                                        // variables globales en C requieren inicializadores
                                                        // constantes, y la expresión depende de otras variables.
														// Por lo tanto, o hardcodeamos los valores, o bien la
														// inicializamos en el main.

int main(void)
{
    inicializaciones();

	// La fórmula que surge al plantear el problema es la siguiente: ((bpm/accent)/10) << 12. 
	// Esta funciona perfectamente cuando el BPM es superior o igual a 40. Pero el problema
	// es que al hacer (bpm/accent), estamos haciendo una división entera, lo mismo pasa al
	// dividir entre 10. Entonces al ejecutar el left shift, ya hemos perdido muchos decimales.
	// La solución es simple, debemos hacer primero la multiplicación por 4096 y luego las divisiones.
	fraccion = (bpm << 12) / (accent * 10);

    do
    {
        tareas_independientes();
        if (gestionar_botones())
        {                                               // Actualizar fracción según nuevos
	    	fraccion = (bpm << 12) / (accent * 10); 	// valores de bpm i accent.
        }
        swiWaitForVBlank();
        actualizar_pantallas();
		
        // activar_beat();								// En el archivo "activar_beat.md" se justifica el porqué 
														// llamamos a la función desde la RSI. Podríamos llamarla
														// desde aquí perfectamente.
    } while(1);
	return(0);
}
