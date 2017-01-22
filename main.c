#include <main.h>
#include <lcd416.c>

#define TIMER_VALUE 100 // 256 -  10ms/(64 * 4/4Mhz)  => 99.75 

int16 miliseconds;

void show(int x, int y, char nadador, float time);
void show2(int x, int y, char nadador, float time);

#INT_TIMER0   //Activa la interrupcion, por Timer 
void timer0_isr()
{
   miliseconds += 10;
   set_timer0(TIMER_VALUE); // Valor del timer para 10ms.
}

void main()
{
   // Variables
   int8 puestos[] = {0,0,0,0}, portB, count = 0; // Almacena los puestos
   float tiempos[] = {0,0,0,0}, seconds; // Almacena los tiempos segun el puesto.
   int16 ultimo;

   // Inicializacion del LCD 4x16
   lcd_init();
   
   // Input pins
   set_tris_b(0xFF); // Puerto B como entradas.
   
   // Interrupcion Timer 0
   setup_timer_0(T0_INTERNAL|T0_DIV_64); 
   enable_interrupts(INT_TIMER0); //habilitar la interrupcion del timer0

   printf(LCD_PUTC,"Presione START");
   while (!input(PIN_B7)); // Espera que pulse START.
   lcd_putc('\f');
   enable_interrupts(GLOBAL); //interrupcion global
   set_timer0(TIMER_VALUE); // Pone el valor del timer para 10ms
   
   while(TRUE)
   {
      if(count == 4) break;
      if(ultimo == miliseconds) continue;
      ultimo = miliseconds;
      seconds = (float)miliseconds / 1000.0f;
      if(tiempos[0] == 0)
         show(1, 1, 'A', seconds);
   
      if(tiempos[1] == 0)
         show(1, 2, 'B', seconds);
         
      if(tiempos[2] == 0)
         show(17, 1, 'C', seconds);
         
      if(tiempos[3] == 0)
         show(17, 2, 'D', seconds);
      portB = input_B(); // PORTB0-B3
      for(int8 i = 0; i < 4; i++)
      {
         if(portB & (0x01 << i) && tiempos[i] == 0){
            puestos[count++] = i;
            tiempos[i] = seconds;
         }
      }

   }
   //Disable interrupts
   disable_interrupts(GLOBAL);
   lcd_putc('\f');
   lcd_gotoxy(3,2);
   printf(LCD_PUTC,"Resultados");
   delay_ms(500);   
   lcd_putc('\f');
   
   printf(LCD_PUTC,"1=\n2=");
   lcd_gotoxy(17,1);
   printf(LCD_PUTC,"3=");
   lcd_gotoxy(17,2);
   printf(LCD_PUTC,"4=");
   
   show2(4, 1,puestos[0] + 'A', tiempos[puestos[0]]);
   show2(4, 2,puestos[1] + 'A', tiempos[puestos[1]]);
   show2(20, 1,puestos[2] + 'A', tiempos[puestos[2]]);
   show2(20, 2,puestos[3] + 'A', tiempos[puestos[3]]);
   // Muestra resultados;

}

void show(int x, int y, char nadador, float time){
   lcd_gotoxy(x,y);
   printf(LCD_PUTC, "%c = %.2f", nadador, time);
}

void show2(int x, int y, char nadador, float time){
   lcd_gotoxy(x,y);
   printf(LCD_PUTC, "%c -> %.2f", nadador, time);
}

