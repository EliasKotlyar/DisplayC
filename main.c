#include "lcd3310.h"
#include <unistd.h>
void main ()
{

  LCD_CS_MAKE_OUT();
  LCD_CS_HIGH();
  
  
  
  LCDInit();
  LCDContrast(0x50);  
  LCDStr(0, (unsigned char *)"*** OLIMEX ***          ", 1);
  LCDStr(1, (unsigned char *)"   OLIMEXINO  ", 0);
  LCDStr(2, (unsigned char *)"    STM32 +   ", 1);
  LCDStr(3, (unsigned char *)"  MOD-LCD3310 ", 0);
  LCDStr(4, (unsigned char *)"  using GPIO  ", 1);
  LCDStr(5, (unsigned char *)"  interface   ", 0);
  
  while(1){
	sleep(10);
  }
  
  
}