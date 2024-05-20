/// C library headers
#include <stdio.h>
#include <string.h>
#include<stdlib.h>


// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
typedef struct database
{
   char id[13];
   char name[20];
   char place[20];
   char number[20];
}db;

typedef struct attendance
{
   char entry[4];
   char id[13];
   char name[20];
   char time[10];
   char date[11];
 }at;


int main()
{
  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)

int p=0,count=0;
db A;
char s[20];
at B,T;

  while(1)
  {
  int serial_port = open("/dev/ttyUSB0", O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 31;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }

// Write to serial port
 // unsigned char msg[50] ={'v','e','c','t','o','r','\n'};
 // write(serial_port, msg, sizeof(msg));
// int i;
 // usleep(100);
  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME

  
    FILE* fp=fopen("database.txt","r");
        if(fp==0)
        {
                printf("Database not found\n");
                return 0;
        }
        FILE* dp=fopen("attendance.txt","r+");
        if(dp==0)
                dp=fopen("attendance.txt","w+");
        count=0,p=0;
        printf("Please Scan Your Card id\n");
	printf("------------------------\n");
       

	int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
  	if (num_bytes < 0) 
	{
     	 printf("Error reading: %s", strerror(errno));
     	 return 1;
	}
//////////////////////////////////////////////////
	  printf("%s\n",read_buf);
/////////////////////////////////////////////////



        sscanf(read_buf,"%s %s %s",B.id,B.time,B.date);
        while((fscanf(fp,"%s %s %s %s",A.id,A.name,A.place,A.number))!=EOF)
                if((strcmp(A.id,B.id))==0)
                {
                        strcpy(B.name,A.name);
                        p++;
                        break;
                }
        if(p==0)
        {
                printf("Invalid ID\n");

                continue;
        }

        while((fscanf(dp,"%s %s %s %s %s",T.entry,T.id,T.name,T.time,T.date))!=EOF)
        {
                if(((strcmp(T.entry,"IN"))==0)&&((strcmp(T.name,B.name))==0))
                        count=1;
                else
                        count=0;
        }
        if(count==0)
        {
                printf("Welcome %s\n",B.name);
		strcpy(s,"WELCOME ");
		strcat(s,B.name);
		
  		write(serial_port,s,sizeof(s));
                fseek(dp,0,SEEK_END);
                strcpy(B.entry,"IN");
                fprintf(dp,"%s %s %s %s %s\n",B.entry,B.id,B.name,B.time,B.date);
        }
        else
        {
                printf("Goodbye %s\n",B.name);
		strcpy(s,"BYE ");
		strcat(s,B.name);
		
  		write(serial_port, s, sizeof(s));
                fseek(dp,0,SEEK_END);
                strcpy(B.entry,"OUT");
                fprintf(dp,"%s %s %s %s %s\n",B.entry,B.id,B.name,B.time,B.date);
        }




        fclose(fp);
        fclose(dp);



// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
  close(serial_port);
}
}

