#include<LPC21XX.h>
#include"mainproject_header.h"
#define RDR0 (U0LSR&1)
#define THRE0 ((U0LSR>>5)&1)
#define RDR1 (U1LSR&1)
#define THRE1 ((U1LSR>>5)&1)
#include<LPC21XX.H>

void delay_sec(unsigned int sec)
{
int a[]={15,60,30,15,15};
unsigned int PCLK;
PCLK=a[VPBDIV]*1000000;
T0PC=0;
T0PR=PCLK-1;
T0TC=0;
T0TCR=1;
while(T0TC<sec);
T0TCR=0;
}


void delay_ms(unsigned int ms)
{
int a[]={15,60,30,15,15};
unsigned int PCLK;
PCLK=a[VPBDIV]*1000;
T0PC=0;
T0PR=PCLK-1;
T0TC=0;
T0TCR=1;
while(T0TC<ms);
T0TCR=0;
}

void uart0_init(u32 baud)
{
	int pclk;
	u32 result=0;
	int a[]={15,60,30,15,15};
	pclk=a[VPBDIV]*1000000;
	PINSEL0|=0x5;
	result=pclk/(16*baud);
	U0LCR=0x83;
	U0DLL=result&0xFF;
	U0DLM=(result>>8)&0xFF;
	U0LCR=0x03;
}

void uart0_tx(u8 data)
{
U0THR=data;
while(THRE0==0);
}

u8 uart0_rx(void)
{
while(RDR0==0);
return U0RBR;
}


void uart0_tx_string(char*p)
{
while(*p!='\0')
{
U0THR=*p;
while(THRE0==0);
p++;
}
U0THR='\0';
while(THRE0==0);
}

void uart0_rx_string(char *p,s32 n)
{
s8 i;
for(i=0;i<n;i++)
{
while(RDR0==0);
p[i]=U0RBR;
if(p[i]=='\r')
break;
}
p[i]='\0';
 }

void uart0_tx_integer(s32 n)
{

int i=0;
char a[16];
if(n==0)
 {
 	uart0_tx('0');
	return;
 }
if(n<0)
{
	uart0_tx('-');
	n=-n;
}
while(n)
{
	a[i++]=n%10+48;
	n=n/10;
}

for(i--;i>=0;i--)
uart0_tx(a[i]);
}

void uart0_tx_float(float f)
 {
 int n1,n2,i=0;
 char a[20];
 if(f==0)
 {
 uart0_tx(0);
 return;
 }
 if(f<0)
 {
 uart0_tx('-');
 f=-f;
 }
 n1=f;
 n2=((f-n1)*1000000);
  
 
 
 while(n2)
 {
 a[i++]= (n2%10)+48;
 n2=n2/10;
 }
 a[i++]='.';
 while(n1)
 {
 a[i++]= n1%10+48;
 n1=n1/10;
 }
 if(f<0)
 a[i++]=('-');

 for(i--;i>=0;i--)
 uart0_tx(a[i]);
 }



/////////////////////////////////////////////////////////////////
void uart1_init(u32 baud)
{
	int pclk;
	u32 result=0;
	int a[]={15,60,30,15,15};
	pclk=a[VPBDIV]*1000000;
	PINSEL0|=0x50000;
	result=pclk/(16*baud);
	U1LCR=0x83;
	U1DLL=result&0xFF;
	U1DLM=(result>>8)&0xFF;
	U1LCR=0x03;
}

void uart1_tx(u8 data)
{
U1THR=data;
while(THRE1==0);
}

u8 uart1_rx(void)
{
while(RDR1==0);
return U1RBR;
}


void uart1_tx_string(char*p)
{
while(*p)
{
uart1_tx(*p);
p++;
}
}

void uart1_rx_string(char *p,s32 n)
{
s8 i;
for(i=0;i<n;i++)
{
while(RDR1==0);
p[i]=U1RBR;
if(p[i]=='\r')
break;
}
p[i]='\0';
 }

void uart1_tx_integer(s32 n)
{

int i=0;
char a[16];
if(n==0)
 {
 	uart1_tx('0');
	return;
 }
if(n<0)
{
	uart1_tx('-');
	n=-n;
}
while(n)
{
	a[i++]=n%10+48;
	n=n/10;
}

for(i--;i>=0;i--)
uart1_tx(a[i]);
}









void uart1_tx_float(float f)
 {
 int n1,n2,i=0;
 char a[20];
 if(f==0)
 {
 uart1_tx(0);
 return;
 }
 if(f<0)
 {
 uart1_tx('-');
 f=-f;
 }
 n1=f;
 n2=((f-n1)*1000000);
  
 
 
 while(n2)
 {
 a[i++]= (n2%10)+48;
 n2=n2/10;
 }
 a[i++]='.';
 while(n1)
 {
 a[i++]= n1%10+48;
 n1=n1/10;
 }
 if(f<0)
 a[i++]=('-');

 for(i--;i>=0;i--)
 uart1_tx(a[i]);
 }

 



