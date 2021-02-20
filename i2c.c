                /* i2c.c */
#include "i2c_defines.h"
#include "i2c.h"
#include "types.h"
#include "delay.h"

/* 
   Any i2c communication session/algorithms begins
   with this event
*/
void i2cStart(void)
{
  //sda=1;//default
  //scl=1;//default
    sda=0;//H - to - L
}

/* 
   Any i2c communication session/algorithms ends
   with this event
*/
void i2cStop(void)
{
  scl=0;//to change sda
  sda=0;//ensure sda is low
  scl=1;//with scl being high
  sda=1;//L to H transition 
}

/*
   A a certain reading byte(s) from slave algorithms
   this event comes  between a start and stop somewhere
*/
void i2cReStart(void)
{
  scl=0;//sda can change during commination
  sda=1;
  scl=1;
  sda=0;//H - to - L
}

/*
   Master is sending byte serially to slave order mst to lsb
*/
void i2cWrite(u8 txByte)
{
  u8 i;
  for(i=0;i<8;i++)
  {
    scl=0;//for data transition
    sda=(txByte>>(7-i))&1;//serialize out msb to lsb order
    scl=1;//for slave to validate so it can read status
  }
}

/*
   Master is reading byte serially from slave order msb to lsb
*/
u8 i2cRead(void)
{
   u8 i,rxByte=0;
   for(i=0;i<8;i++)
   {
     scl=0;//for slave to transit/change sda
     scl=1; //so master can after now validate
     if(sda)
       rxByte|=0x80>>i;//serialize in msb to lsb order
   }
   return rxByte;
}

//   master takes ack from slave after every byte written
void i2cAck(void)
{
   scl=0;//master make scl=0
   sda=1;//master release sda line & slave  pulls down sda
   scl=1;//master also releases scl for validation ack
   while(sda);//master check if slave has ack/or not
}

/* 
   master issue noack to slave for only one byte to read/
   last of multiple bytes to be read as a indication to slave
   that no more further reads will carried out & usually
   after this event next event will be stop event
*/

void i2cNoAck(void)
{
   scl=0;//master make scl=0
   sda=1;//to release sda line/slave should pull down sda
   scl=1;//master also releases scl for validation
}

/*
    Master issues this event for each of all bytes BUT last byte
    of multiple bytes to be read from slave as an indication
    that after a byte read next byte will also be read

    Assume "hello" is to sequence of bytes to be read from slave

    h|MACK|e|MACK|l|MACK|l|MACK|o|NACK
*/ 

void i2cMasterAck(void)    
{
    scl=0;//master makes scl=0,so that slave can release sda
    sda=0;//master pull down sda to issue ack to slave
    scl=1;//master release scl,so slave can read sda
    scl=0;//master make scl=0
    sda=1;//so it can release sda
} 

void i2cDevWrite(u8 slaveAddr,u8 wBuffAddr,u8 dat)
{
   i2cStart();
   i2cWrite(slaveAddr&0xfe);
   i2cAck();
   i2cWrite(wBuffAddr);
   i2cAck();
   i2cWrite(dat);
   i2cAck();
   i2cStop();
   delay_ms(10);
}

u8 i2cDevRead(u8 slaveAddr,u8 rBuffAddr)
{
   u8 dat;
   i2cStart();
   i2cWrite(slaveAddr&0xfe);
   i2cAck();
   i2cWrite(rBuffAddr);
   i2cAck();
   i2cReStart();
   i2cWrite(slaveAddr|0x01);
   i2cAck();
   dat=i2cRead();
   i2cNoAck();
   i2cStop();

   return dat;
}
