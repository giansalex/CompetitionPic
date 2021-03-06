#include <main.h>
#include <lcd420.c>
#define TIMER_VALUE 100 // 10ms = 4 * 64 * (256 - x) / 4Mhz   => x  = 99.75 

int16 miliseconds = 0; // guarda los milisegundos del timer.

#INT_TIMER0   //Activa la interrupcion, por Timer 
void timer0_isr()
{
   miliseconds += 10; // Aumenta 10ms.
   set_timer0(TIMER_VALUE); // carga del timer para 10ms.
}

void main()
{
   // Variables
   int8 portD, count = 0, i;
   int8 puestos[] = {0,0,0,0}; // Almacena los puestos
   float tiempos[] = {0,0,0,0}; // Almacena los tiempos segun el puesto.
   int16 ultimo;
   float seconds;
   
   // Inicializacion del LCD 4x16
   lcd_init();
   
   // Input pins
   set_tris_d(0xFF); // Puerto D como entradas.
   
   // Interrupcion Timer 0
   setup_timer_0(T0_INTERNAL|T0_DIV_64); // Prescaler = 64
   enable_interrupts(INT_TIMER0); //habilitar la interrupcion del timer0

   lcd_putc("Presione START");
   while (!input(PIN_D7)); // Espera que pulse START.
   lcd_putc('\f');
   enable_interrupts(GLOBAL);// Pone en marcha todas las interrupaciones.
   set_timer0(TIMER_VALUE); // carga del timer para 10ms
   
   while(TRUE)
   {
      if(count == 4) break; // Comprueba que todos los nadadores hayan terminado.
      if(ultimo == miliseconds) continue;
      ultimo = miliseconds;
      seconds = (float)miliseconds / 1000.0f; // Calcula los segundos.
      
      // Imprime los tiempos de cada nadador que estan compitiendo.
      for(i = 0; i< 4; i++){
         if(tiempos[i] == 0){ // Comprueba que el nadador aun siga compitiendo.
            lcd_gotoxy(1, i + 1);
            printf(LCD_PUTC,"%c = %.2f\n", i + 'A', seconds);
         }
      }
      
      //Comprueba si se pulso una tecla
      portD = input_D(); // PORTB0-B3
      for(i = 0; i < 4; i++)
      {
         if(portD & (0x01 << i) && tiempos[i] == 0){ // Evita que un nadador pulse mas de una vez.
            puestos[count++] = i; // guarda la posicion del nadador.
            tiempos[i] = seconds; // guarda el tiempo del nadador X.
         }
      }

   }
   //Disable interrupts
   disable_interrupts(GLOBAL);
   
   lcd_putc('\f');
   lcd_gotoxy(3,2);
   lcd_putc("Resultados");
   delay_ms(500);   
   lcd_putc('\f');
   
   // Muestra los resultados.
   for(i=0;i<4;i++){
      int8 nadador = puestos[i];
      lcd_gotoxy(1, i + 1);
      printf(LCD_PUTC, "%d.- %c = %.2f", i + 1, nadador + 'A', tiempos[nadador] );
   }

}

