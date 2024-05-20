


#include<LPC21XX.h>
#include"mainproject_header.h"
s32 flag=0,count=0,result;
#define sw1 ((IOPIN0>>15)&1)
#define sw2 ((IOPIN0>>14)&1)
s8 a[31],r[20];
s32 c=0,b=0;
u32 flag1=0,flag2=0;

__irq void uart1_handler(void)
{
if(((U1IIR>>1)&7)==2)
{
	a[c]=U1RBR;
	if(a[c]=='\r')
	{
		a[c]='\0';
		flag1=1;
		c=0;
	}
	else if(c==11)
	{
	a[12]='\0';
	c=0;
  flag1=1;
	}
	if(flag1==0)
		c=c+1;	 
}
VICVectAddr=0;
}		

__irq void uart0_handler(void)
{
if(((U0IIR>>1)&7)==2)
{
	r[b]=U0RBR;
	//uart0_tx(a[b]);
	if(r[b]=='\r')
	{
		r[b]='\0';
		flag2=1;
		b=0;
	}
	else if(b==18)
	{
	r[19]='\0';
	b=0;
  flag2=1;
	}
	if(flag2==0)
		b=b+1;	 
}
VICVectAddr=0;
}		



__irq void EINT0_Handler(void)
{  
	flag^=1;
	EXTINT=1;
	VICVectAddr=0;
}
void config_vic(void)
{
	VICIntSelect=0;
	VICVectAddr0=(u32)uart1_handler;
	VICVectAddr1=(u32)uart0_handler;
	VICVectAddr2=(u32)EINT0_Handler;
	VICVectCntl0=7|(1<<5);
	VICVectCntl1=6|(1<<5);
	VICVectCntl2=14|(1<<5);
	VICIntEnable|=1<<14|1<<6|1<<7;
}
void en_eint0(void)
{
PINSEL1|=1;
EXTMODE=1;
EXTPOLAR=0;
U0IER=1;
U1IER=1;
} 

int main()
{
	int hour,h,m,s,date,month,year,day;
	lcd_init();
	i2c_init();
	uart0_init(9600);
	uart1_init(9600);
	en_eint0();
	config_vic();
	i2c_byte_write_frame(0xD0,0x0,0x50);
	i2c_byte_write_frame(0xD0,0x1,0x59);
	i2c_byte_write_frame(0xD0,0x2,0x11|1<<6|1<<5); //1<<6 is to give 12 hour mode  and 1<<5 to set pm
	
	i2c_byte_write_frame(0xD0,0x3,0x7);
	i2c_byte_write_frame(0xD0,0x4,0x24);
	i2c_byte_write_frame(0xD0,0x5,0x3);
	i2c_byte_write_frame(0xD0,0x6,0x24);
	  
while(1)
{
	  if(flag==1)
	  {
	  		lcd_cmd(0x01);
			h=0,m=0,s=0,date=1,month=1,year=0,day=1;
		
			lcd_string("Hour Setting");
				while(sw1==1)
				{
			  	if(flag==0)
				goto END;
				lcd_cmd(0xc0);
				if(h/10==0)
				lcd_data('0');
				lcd_number(h);
				if(count==0)
				lcd_string("AM");
				else
				lcd_string("PM");
				if(sw2==0)
				{
				while(sw2==0);
				h++;
				}
				if(h==12)
				{	
				count^=1;
				h=0;
				}
				}
				while(sw1==0);
				result=h%10;
				result|=((h/10)%10)<<4;

				if(count)
				i2c_byte_write_frame(0xD0,0x2,result|1<<6|1<<5);
				else
				i2c_byte_write_frame(0xD0,0x2,result|1<<6);
				count=0;
		
				lcd_cmd(0x01);
//////////////////////////////////////////////////////////////////////////////////////////
				lcd_string("Minute Setting");
				while(sw1==1)
				{
				 	if(flag==0)
				goto END;
				lcd_cmd(0xc0);
				if(m/10==0)
				lcd_data('0');
				lcd_number(m);
				if(sw2==0)
				{
				while(sw2==0);
				m++;
				}
				if(m==60)
				{	
				m=0;
				}
				}
				while(sw1==0);
				result=m%10;
				result|=((m/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x1,result);
		
				lcd_cmd(0x01);
///////////////////////////////////////////////////////////////////////////////////////////
		
				lcd_string("Seconds Setting");
				while(sw1==1)
				{
					if(flag==0)
				goto END;
		
				lcd_cmd(0xc0);
				if(s/10==0)
				lcd_data('0');
				lcd_number(s);
				if(sw2==0)
				{
				while(sw2==0);
				s++;
				}
				if(s==60)
				{	
				s=0;
				}
				}
				while(sw1==0);
				result=s%10;
				result|=((s/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x0,result);
		
				lcd_cmd(0x01);
/////////////////////////////////////////////////////////////////////////////////////////////
			
				lcd_string("Year Setting");
				while(sw1==1)
				{
			  	if(flag==0)
				goto END;
				lcd_cmd(0xc0);
				lcd_number(20);
				if(year/10==0)
				lcd_data('0');
				lcd_number(year);
				if(sw2==0)
				{
				while(sw2==0);
				year++;
				}
				if(year==100)
				{	
				year=0;
				}
				}
				while(sw1==0);
				result=year%10;
				result|=((year/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x6,result);
		
				lcd_cmd(0x01);
/////////////////////////////////////////////////////////////////////////////////////////////
			
				lcd_string("Month Setting");
				while(sw1==1)
				{
					if(flag==0)
				goto END;
				lcd_cmd(0xc0);
				if(month/10==0)
				lcd_data('0');
				lcd_number(month);
				if(sw2==0)
				{
				while(sw2==0);
				month++;
				}
				if(month==13)
				{	
				month=1;
				}
				}
				while(sw1==0);
				result=month%10;
				result|=((month/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x5,result);
		
				lcd_cmd(0x01);
////////////////////////////////////////////////////////////////////////////////////////////
				lcd_string("Date Setting");
				while(sw1==1)
				{
					if(flag==0)
				goto END;
				lcd_cmd(0xc0);
				if(date/10==0)
				lcd_data('0');
				lcd_number(date);
				if(sw2==0)
				{
				while(sw2==0);
				date++;
				}
				if((year%4==0)&&(month==2))
				{
				if(date==30)
					date=1;
				}
				else if(month==2)
				{
				if(date==29)
				date=1;
				}
				else if(month==1|month==3|month==5|month==7|month==8|month==10|month==12)
				{
					if(date==32)
	 					date=1;
				}
				else
				{
					if(date==31)
					date=1;
				}

				}
				while(sw1==0);
				result=date%10;
				result|=((date/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x4,result);
		
				lcd_cmd(0x01);
////////////////////////////////////////////////////////////////////////////////////////////
	 			 	lcd_string("Day Setting");
				while(sw1==1)
				{
				  	if(flag==0)
				goto END;
				lcd_cmd(0xc0);
					switch(day)
				{
				case 1:lcd_string("Mon");break;
				case 2:lcd_string("Tue");break;
				case 3:lcd_string("Wed");break;
				case 4:lcd_string("Thu");break;
				case 5:lcd_string("Fri");break;
				case 6:lcd_string("Sat");break;
				case 7:lcd_string("Sun");break;
				}
				
				if(sw2==0)
				{
				while(sw2==0);
				day++;
				}
				if(day==8)
				{	
				day=1;
				}
				}
				while(sw1==0);
				result=day%10;
				result|=((day/10)%10)<<4;
				i2c_byte_write_frame(0xD0,0x3,result);
		
				lcd_cmd(0x01);
//////////////////////////////////////////////////////////////////////////////////////////
		END:
		lcd_cmd(0x01);
		h=0,m=0,s=0,day=0,month=0,year=0,date=0;
		flag=0;
	  }
	
	
	
	   if(flag==0)
		{
		lcd_cmd(0x80);
		hour=i2c_byte_read_frame(0xD0,0x2);
		h=hour&0X1F;
		m=i2c_byte_read_frame(0xD0,0x1);
		s=i2c_byte_read_frame(0xD0,0x0);
		day=i2c_byte_read_frame(0xD0,0x3);
		date=i2c_byte_read_frame(0xD0,0x4);
		month=i2c_byte_read_frame(0xD0,0x5);
		year=i2c_byte_read_frame(0xD0,0x6);

		lcd_cmd(0x80);
		lcd_data((h/0x10)+48);
		lcd_data((h%0x10)+48);
		lcd_data(':');
		
		lcd_data((m/0x10)+48);
		lcd_data((m%0x10)+48);
		lcd_data(':');
		
		lcd_data((s/0x10)+48);
		lcd_data((s%0x10)+48);
		 	if((hour>>5)&1)
		lcd_string("PM");
		else
		lcd_string("AM");		 
		lcd_cmd(0xc0);
	


		lcd_data((date/0x10)+48);
		lcd_data((date%0x10)+48);
		lcd_data(':');

		
		lcd_data((month/0x10)+48);
		lcd_data((month%0x10)+48);
		lcd_data(':');
		lcd_string("20");
		
		lcd_data((year/0x10)+48);
		lcd_data((year%0x10)+48);


	
		switch(day)
		{
		case 1:lcd_string("Mon");break;
		case 2:lcd_string("Tue");break;
		case 3:lcd_string("Wed");break;
		case 4:lcd_string("Thu");break;
		case 5:lcd_string("Fri");break;
		case 6:lcd_string("Sat");break;
		case 7:lcd_string("Sun");break;
		}
		lcd_cgram();
		lcd_cmd(0xcd);	  
		
		lcd_data(0);	
		
	}

	if(flag1==1)
	{
		flag1=0;
		hour=i2c_byte_read_frame(0xD0,0x2);
		h=hour&0X1F;
		m=i2c_byte_read_frame(0xD0,0x1);
		s=i2c_byte_read_frame(0xD0,0x0);
		date=i2c_byte_read_frame(0xD0,0x4);
		month=i2c_byte_read_frame(0xD0,0x5);
		year=i2c_byte_read_frame(0xD0,0x6);
		a[12]=' ';
		a[13]=((h/0x10)+48);
		a[14]=((h%0x10)+48);
		a[15]=':';
		a[16]=((m/0x10)+48);
		a[17]=((m%0x10)+48);
		a[18]=':';
		a[19]=((s/0x10)+48);
		a[20]=((s%0x10)+48);
		a[21]=' ';
		a[22]=((date/0x10)+48);
		a[23]=((date%0x10)+48);
		a[24]='/';
		a[25]=((month/0x10)+48);
		a[26]=((month%0x10)+48);
		a[27]='/';
		a[28]=((year/0x10)+48);
		a[29]=((year%0x10)+48);
		a[30]='\0';
	    uart0_tx_string(a);
	
	}

	if(flag2==1)
	{
		lcd_cmd(0x01);
 		flag2=0;
		lcd_string(r);
		delay_ms(1000);
		lcd_cmd(0x01);
	}

}

}
	



	