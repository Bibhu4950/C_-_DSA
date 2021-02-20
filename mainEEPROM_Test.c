#include "types.h"
#include "i2c.h"
#include "delay.h"
#include <reg51.h>

#define I2C_EEPROM1_SA 0xA0 //1010 000 0
#define I2C_EEPROM2_SA 0xAE //1010 111 0

sbit led=P3^5;

main()
{
   u8 test=0;
   i2cDevWrite(I2C_EEPROM1_SA,0x00,'A');
   delay_ms(3000);
   test=i2cDevRead(I2C_EEPROM1_SA,0x00);
   delay_ms(3000); 
   if(test=='A')
      led=~led;

   i2cDevWrite(I2C_EEPROM2_SA,0x00,'a');
   delay_ms(3000);
   test=i2cDevRead(I2C_EEPROM2_SA,0x00);
   delay_ms(3000); 
   if(test=='a')
      led=~led;

   while(1);
}
