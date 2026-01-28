//
// Author: Santiago Romaní Also
//

#define FREQ_ENT 32728                                          // frecuencia de entrada mínima (Hz)

// Nos dicen que tenemos 15 pulsos/segundo, pero como hay 2 estados, 
// la frecuencia que debe recibir la RSI es de 30 activaciones por segundo.
// Por tanto, dividimos 32728/30 = 1090,933, así que debemos redondear.
short divfreq_vmax = 1091;                                     // Divisor freq. Velocidad máx.

// Para la mínima nos dice que el mínimo será 1 pulso por segundo,
// por 2 estados -> 32728/2 = 16364
short divfreq_vmin = 16364;                                     // Divisor freq. Velocidad mín.

short dec_actual = 0, dec_objetivo = 0;                         // [-9000..9000] centígrados
int ra_actual = 0, ra_objetivo = 0;                             // [0..86399] segundos
unsigned char seek_ra = 0, seek_dec = 0;                        // Búsqueda de objetivo
unsigned char track = 0;                                        //  Seguimiento de objetivo


int main()
{
    inicializaciones();

    // Sabemos que el timer 0 se activa 1 vez por segundo, así que dividimos entre 1
    activar_timer(0, 32728);                                    // Timer 0 siempre activo
    do
    {
        tareas_independientes();
        if (gestionar_interfaz(&ra_objetivo, &dec_objetivo))    // Activar giros de búsqueda
        {
            activar_timer(1, divfreq_vmax); seek_ra = 1; track = 0;
            activar_timer(2, divfreq_vmax); seek_dec = 1;
        }
        swiWaitForVBlank();
        actualizar_pantallas();
    } while (1);
    return(0);
}
