#include <16F887.h>
#device adc=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(clock=4M)

#define LCD_ENABLE_PIN PIN_C7
#define LCD_RS_PIN PIN_C5
#define LCD_RW_PIN PIN_C6
#define LCD_Data4  PIN_D4
#define LCD_Data5  PIN_D5
#define LCD_Data6  PIN_D6
#define LCD_Data7  PIN_D7

