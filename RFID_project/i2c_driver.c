/*I2C Driver*/

#include<LPC21XX.h>
#include"mainproject_header.h"
#define SI ((I2CONSET>>3)&1)
u8 temp;

void i2c_byte_write_frame(u8 sa,u8 ma,u8 data)
{
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
	 I2CONCLR=1<<5;
	if(I2STAT!=0x8)
	{
		//uart0_tx_string("Err: write Start\r\n");
		goto exit;
	}

	I2DAT=sa;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x20)
	{
		//uart0_tx_string("Err:  write SA+W\r\n");
		goto exit;
	}
	I2DAT=ma;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		//uart0_tx_string("Err: write Ma\r\n");
		goto exit;
	}
	I2DAT=data;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		//uart0_tx_string("Err: write data\r\n");
		goto exit;
	}
	
	exit:
	I2CONSET=1<<4;
	I2CONCLR=1<<3;

}

u8 i2c_byte_read_frame(u8 sa,u8 ma)
{
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
		 I2CONCLR=1<<5;

	if(I2STAT!=0x8)
	{
		//uart0_tx_string("Err: Start\r\n");
		goto exit;
	}

	I2DAT=sa;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x20)
	{
		//uart0_tx_string("Err: SA+W\r\n");
		goto exit;
	}
	I2DAT=ma;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		//uart0_tx_string("Err: Ma\r\n");
		goto exit;
	}
	
	
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
	I2CONCLR=1<<5;
	if(I2STAT!=0x10)
	{
		//uart0_tx_string("Err: RE Start\r\n");
		goto exit;
	}
	
	
	I2DAT=sa|1;;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x48)
	{
		//uart0_tx_string("Err: Sa+R\r\n");
		goto exit;
	}
	

	I2CONCLR=1<<3;
	while(SI==0);
	temp=I2DAT;
   	
		exit:
	I2CONSET=1<<4;
	I2CONCLR=1<<3;
	return temp;
}

void i2c_init(void)
{
	PINSEL0|=0X50;
	I2SCLL=I2SCLH=75;
	I2CONSET=1<<6;
}


