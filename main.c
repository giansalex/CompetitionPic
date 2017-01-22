#include <main.h>
#include <lcd.c>
#define TIMER_VALUE 100 // 256 -  10ms/(64 * 4/4Mhz)  => 99.75

int16 miliseconds;   int16 ultimo;
int8 count = 0;
int8 puestos[] = {0,0,0,0};
int16 times[] = {0,0,0,0};
void show(int x, int y, char nadador, float time);
void show2(int x, int y, char nadador, float time);

#INT_TIMER0   //Activa la interrupcion, por Timer 
void timer0_isr()
{
   miliseconds += 10;
//   output_toggle(PIN_C0);
}

int8 portB;
#INT_RB
void EXT_RB()
{
   portB = (input_B() ^ 0xF0) >> 4; // PORTB4-B7
   for(int8 i = 0; i < 4; i++)
   {
      if(portB & (0x01 << i) && times[i] == 0){
         puestos[count++] = i;
         times[i] = ultimo;
         //disable interrupt in the pin_bi
      }
   }
}

void main()
{

   float seconds;
   // Inicializacion del LCD 4x16
   lcd_init();
   
   // Input pins
   set_tris_b(0xFF); // Todos como entradas.
   
   // Interrupcion Puerto B
   enable_interrupts(INT_RB4|INT_RB5|INT_RB6|INT_RB7);
   EXT_int_edge(H_TO_L);
   
   // Interrupcion Timer 0
   setup_timer_0(T0_INTERNAL|T0_DIV_64);//_16
   enable_interrupts(INT_TIMER0); //habilitar la interrupcion externa
   
   printf(LCD_PUTC,"Presione START");
   while (input(PIN_B0)); // Espera que pulse START.
   lcd_putc('\f');
   enable_interrupts(GLOBAL); //interrupcion global
   set_timer0(TIMER_VALUE);
   
   while(TRUE)
   {
      if(count == 4) break;
      if(ultimo == miliseconds) continue;
      ultimo = miliseconds;
      seconds = (float)miliseconds / 1000.0f;
      if(times[0] == 0)
         show(1, 1, 'A', seconds);
   
      if(times[1] == 0)
         show(1, 2, 'B', seconds);
         
      if(times[2] == 0)
         show(17, 1, 'C', seconds);
         
      if(times[3] == 0)
         show(17, 2, 'D', seconds);
         
      set_timer0(TIMER_VALUE);
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
   
   show2(4, 1,puestos[0] + 'A', times[puestos[0]] / 1000.0f);
   show2(4, 2,puestos[1] + 'A', times[puestos[1]] / 1000.0f);
   show2(20, 1,puestos[2] + 'A', times[puestos[2]] / 1000.0f);
   show2(20, 2,puestos[3] + 'A', times[puestos[3]] / 1000.0f);
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

