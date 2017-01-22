#include <16f887.h>
#use delay(clock=4M)
#fuses XT
#byte portb=0x06
#byte portd=0x08


#INT_TIMER0   //Activa la interrupcion, por Timer 
void timer0_isr()
{
  portd=~portd;
  set_timer0(195);//1ms
}

void main()
{
 set_tris_d(0x00);
 portd=0;
 enable_interrupts(global); //interrupcion global
 enable_interrupts(INT_TIMER0); //habilitar la interrupcion externa
 setup_timer_0(T0_INTERNAL|T0_DIV_16);
 set_timer0(195);
 while(true)
 {
 }

}
