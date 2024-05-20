#include<LPC21XX.h>
#include"mainproject_header.h"
#define RS 1<<17
#define RW 1<<18
#define EN 1<<19

void lcd_data(unsigned char data)
{
unsigned int temp;
IOCLR1=0xFE<<16;
temp=(data&0xF0)<<16;
IOSET1=temp;
IOSET1=RS;
IOCLR1=RW;
IOSET1=EN;
delay_ms(2);
IOCLR1=EN;

IOCLR1=0xFE<<16;
temp=(data&0x0F)<<20;
IOSET1=temp;
IOSET1=RS;
IOCLR1=RW;
IOSET1=EN;
delay_ms(2);
IOCLR1=EN;
 }

void lcd_cmd(unsigned char cmd)
{
unsigned int temp;
IOCLR1=0xFE<<16;
temp=(cmd&0xF0)<<16;
IOSET1=temp;
IOCLR1=RS;
IOCLR1=RW;
IOSET1=EN;
delay_ms(2);
IOCLR1=EN;

IOCLR1=0xFE<<16;
temp=(cmd&0x0F)<<20;
IOSET1=temp;
IOCLR1=RS;
IOCLR1=RW;
IOSET1=EN;
delay_ms(2);
IOCLR1=EN;
 }

void lcd_init(void)
{
IODIR1=0xFE<<16;
IOCLR1=EN;
PINSEL2=0x0;
lcd_cmd(0X2);
lcd_cmd(0X3);
lcd_cmd(0X28);
lcd_cmd(0X0E);
lcd_cmd(0X01);
}

void lcd_string(char*p)
{
	while(*p!='\0')
	{
	lcd_data(*p);
	p++;
	}
}

 void lcd_float(float f)
 {
 int n1,n2,i=0;
 char a[20];
 if(f==0)
 {
 lcd_data(0);
 return;
 }
 if(f<0)
 {
 lcd_data('-');
 f=-f;
 
 }
 n1=f;
 //n2=((f-n1)*1000000);
  n2=((f-n1)*10);/////for adc project 
 
 
 while(n2/10)
 {
 a[i++]= n2%10+48;
 n2=n2/10;
 }
 a[i++]=n2%10+48;
 a[i++]='.';
 while(n1/10)
 {
 a[i++]= n1%10+48;
 n1=n1/10;
 }
 a[i++]=n1%10+48;
 if(f<0)
 a[i++]=('-');

 for(i--;i>=0;i--)
 lcd_data(a[i]);
 }

 lcd_string_circular(char *ptr)
 {
 int i,len;
 for(len=0;ptr[len];len++);

 while(1)
 {
 	for(i=0;i<16;i++)
	{
		delay_ms(500);
		lcd_cmd(0x01);
		lcd_cmd(0X80+i);
		lcd_string(ptr);
		if(len+i>16)
		{
			lcd_cmd(0x80);
			lcd_string(ptr+16-i);
			}
			}}}


 	
 
 /*{
 int i,j;
 char*p;
 while(1)
 {
 	for(i=0;i<16;i++)
	{   
		p=ptr;
		delay_ms(500);
		lcd_cmd(0x01);
		j=(0x80+i);
		while(*p)
		{
			if(j>0x8F)
			j=0x80;
			lcd_cmd(j++);
			lcd_data(*p++);
		}
	}
 }
 }
  */
void lcd_cgram(void)
{
int i;
unsigned char a[]={0x0,0xA,0xa,0x0,0x0,0x11,0xe,0x0};
lcd_cmd(0x40);
for(i=0;i<7;i++)
lcd_data(a[i]);
}




lcd_string_doublecircular(char *ptr1,char *ptr2)
{
int i,a1,a2;
char *p1,*p2;
while(1)
{

	for(i=0;i<16;i++)
	{
		p1=ptr1;
		p2=ptr2;
		delay_ms(500);
		lcd_cmd(0x01);
		a1=0x80+i;
		a2=0xCF-i;
		while(*p1 || *p2)
		{
		if(a1>0x8F)
		a1=0x80;
		if(a2<0xC0)
		a2=0xCF;
			if(*p1)
			{
				lcd_cmd(a1++);
				lcd_data(*p1++);
			}
			if(*p2)
			{
				lcd_cmd(a2--);
				lcd_data(*p2++);
			}
	}
}
}

}

void lcd_number(int n)
{
int i=0;
char a[16];
if(n==0)
 {
 	lcd_data('0');
	return;
 }
if(n<0)
{
	lcd_data('-');
	n=-n;
}
while(n)
{
	a[i++]=n%10+48;
	n=n/10;
}

for(i--;i>=0;i--)
lcd_data(a[i]);
}





		  