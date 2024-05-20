typedef	unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;
typedef signed char s8;

 extern void uart0_init(u32 baud);
 extern void uart0_tx(u8 data);
 extern u8 uart0_rx(void);
 extern void uart0_tx_string(char*p);
 extern void uart0_rx_string(char *p,s32 n);
 extern void uart0_tx_integer(s32 n); 
 extern void uart0_tx_float(float f);

 extern void uart1_init(u32 baud);
 extern void uart1_tx(u8 data);
 extern u8 uart1_rx(void);
 extern void uart1_tx_string(char*p);
 extern void uart1_rx_string(char *p,s32 n);
 extern void uart1_tx_integer(s32 n); 
 extern void uart1_tx_float(float f);

 extern void lcd_data(unsigned char data);
 extern void lcd_cmd(unsigned char cmd);
 extern void lcd_init(void);
 extern void lcd_string(char*p);
 extern void lcd_float(float f);
 extern lcd_string_circular(char *ptr);
 extern void lcd_cgram(void);
 extern lcd_string_doublecircular(char *ptr1,char *ptr2);
 extern void lcd_number(int n);
 extern void i2c_byte_write_frame(u8 sa,u8 ma,u8 data);
 extern u8 i2c_byte_read_frame(u8 sa,u8 ma);
 extern void i2c_init(void);



 extern void delay_sec(unsigned int sec);
 extern void delay_ms(unsigned int ms);






