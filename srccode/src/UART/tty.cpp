#include "tty.h"
#include <termios.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sstream>
using namespace std;

// int fd;

int wait_flag=TRUE;                     //TRUE while no signal received
//	FILE *input;
//	FILE *output;
struct termios oldtio, newtio;       //place for old and new port settings for serial port
struct termios oldkey, newkey;       //place tor old and new port settings for keyboard teletype
//   int tty;	
struct formatFPH3 {
	int temperature;	
	int humidity;
	int speedf1;
	int speedf2;
	int speedf3;
} pformatFPH3;
/*
t Ц температура в ?C;
h Ц влажность в %;
f1 Е f3 Ц скорости вращени€ вентил€торов 1Е3 об/мин.
*/	
void signal_handler_IO (int status)
{
   wait_flag = FALSE;
}

int openUART1(){
//-------------------------
	//----- SETUP USART 1 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart1_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart1_filestream = open("/dev/ttyPS1", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart1_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart1_filestream, &options);
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart1_filestream, TCIFLUSH);
	tcsetattr(uart1_filestream, TCSANOW, &options);
    return uart1_filestream;
}

/*
int openUART(){

	char devicename[80];
//	long Baud_Rate = 921600;         // default Baud Rate (110 through 38400)
	long BAUD = BAUDRATE;                      // derived baud rate from command line
	long DATABITS = CS8;
	long STOPBITS = 1;
	long PARITYON = 0;
	long PARITY = 0;
//	int Data_Bits = 8;              // Number of data bits
//	int Stop_Bits = 1;              // Number of stop bits
//	int Parity = 0;                 // Parity as follows:
                  // 00 = NONE, 01 = Odd, 02 = Even, 03 = Mark, 04 = Space
//    Format = 5; // hex
	//printf("NOM_UART==/dev/ttyPS%d\n", NOM_UART);
    if(NOM_UART==0) {
       strcpy(devicename,"/dev/ttyPS0");
    } else {
       strcpy(devicename,"/dev/ttyPS1");
    }

//   char Param_strings[7][80];
//   char message[90];

//   int fd, c, res, i, error =0;
   int fd;
//   char In1, Key;
   struct sigaction saio;               //definition of signal action
   
//   char buf[255];                       //buffer for where data is put   
//   error=0;
//   input = fopen("/dev/tty", "r");      //open the terminal keyboard
//   output = fopen("/dev/tty", "w");     //open the terminal screen

//   if (!input || !output)

   
   
// fprintf(stderr, "Failed to open config file %s", config_filename);   
//   sprintf(message,"Device=%s, Baud=%li (%li)\r\n",devicename, BAUD, B921600); //output the received setup parameters
//   fputs(message,output);
//   sprintf(message,"Data Bits=%i  Stop Bits=%i  Parity=%i  Format=%i\r\n",DATABITS, STOPBITS, PARITY, Format);
//   fputs(message,output);
   
//   tty = open("/dev/tty", O_RDWR | O_NOCTTY | O_NONBLOCK); //set the user console port up
//   tcgetattr(tty,&oldkey); // save current port settings   //so commands are interpreted right for this program
    // set new port settings for non-canonical input processing  //must be NOCTTY
   newkey.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
   newkey.c_iflag = IGNPAR;
   newkey.c_oflag = 0;
   newkey.c_lflag = 0;       //ICANON;
   newkey.c_cc[VMIN]=1;
   newkey.c_cc[VTIME]=0;
//   tcflush(tty, TCIFLUSH);
//   tcsetattr(tty,TCSANOW,&newkey);   
 
	fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror(devicename);
        printf("Unable to open /dev/ttyPS%d\n", NOM_UART);
		return -1;
    } else {
        // printf("Ok. Enable to open /dev/ttyPS%d\n", NOM_UART);
    }
	  
		//install the serial handler before making the device asynchronous
    saio.sa_handler = signal_handler_IO;
    sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO,&saio,NULL);

      // allow the process to receive SIGIO
    fcntl(fd, F_SETOWN, getpid());
      // Make the file descriptor asynchronous (the manual page says only
      // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
    fcntl(fd, F_SETFL, FASYNC);

    tcgetattr(fd,&oldtio); // save current port settings 
      // set new port settings for canonical input processing 
    newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;       //ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
	pformatFPH3.temperature = 0;
	pformatFPH3.humidity = 0;
	pformatFPH3.speedf1 = 0;
	pformatFPH3.speedf2= 0;
	pformatFPH3.speedf3= 0;

	return fd;
}
*/
unsigned int closeUART(int fd){
    // tcsetattr(fd,TCSANOW,&oldtio);
    //tcsetattr(fd,TCSANOW,&oldkey);
    // close(tty);
    close(fd);        //close the com port

//   fclose(input);
//   fclose(output);
   return 1;
}

unsigned int writeUART(int fd, char* ini_tty){
	// int n =0;
	// n = write(fd, "#w0312122233\r\n", 14);	
	write(fd, ini_tty, 14);
	//printf(ini_tty);
    //printf("n = %d",n);
	return 1;
}

unsigned int writeBuzzerUART(int fd, int status){
	// int n =0;
	if(status){
		write(fd, "#w1000000000\r\n", 14);
		printf("#w0000000000\r\n");
	}
	else{ 	
		write(fd, "#w0000000000\r\n", 14);		
		printf("#w0000000000\r\n");
	}	
	return 1;
	
}

int parseData(char* data, int count){
	int i, j=0,nom_field=0;
	char parameter[16];	
	int numb;
	int mas_numb[6];
	for(i=0;i<count;i++){
		if(data[i]==','){			
			if(j==0 || parameter[0]>57){
				numb = -1;
				if(j>3){
					do{
					// P_OFF Ц Power Off	
					if(parameter[0]=='P' && parameter[1]=='_' && parameter[2]=='O'){    numb = -2; break; }
					// R_ERR Ц EEPROM Corrupted
					if(parameter[0]=='P' && parameter[1]=='_' && parameter[2]=='E'){	numb = -3; break; }				
					// F_ERR Ц Flash Corrupted
					if(parameter[0]=='F' && parameter[1]=='_' && parameter[2]=='E'){	numb = -4; break; }				
					// F_BSY Ц Flash busy
					if(parameter[0]=='F' && parameter[1]=='_' && parameter[2]=='B'){	numb = -5; break; }
					// S_ERR Ц Sensor read error
					if(parameter[0]=='S' && parameter[1]=='_' && parameter[2]=='E'){	numb = -6; break; }				
					// BEGIN Ц Data not ready yet
					if(parameter[0]=='B' && parameter[1]=='E' && parameter[2]=='G'){	numb = -7; break; }
					}while(0>1);
				}	
			} else {
				parameter[j++] = '\0';
				istringstream ( parameter ) >> numb;				
			}
			mas_numb[nom_field] = numb;
			nom_field++;
			j=0;
		} else { 	 
			parameter[j++] = data[i];
		}
	}
		if(j==0 || parameter[0]>57){
			numb = -1;
			
		} else {
			parameter[j++] = '\0';
			istringstream ( parameter ) >> numb;				
		}
		mas_numb[nom_field] = numb;
	pformatFPH3.temperature = mas_numb[0];
	//printf("temperature : %d\n", mas_numb[0]);
	pformatFPH3.humidity = mas_numb[1];
	//printf("humidity : %d\n", mas_numb[1]);
	pformatFPH3.speedf1 = mas_numb[2];
	//printf("speedf1 : %d\n", mas_numb[2]);
	pformatFPH3.speedf2 = mas_numb[3];
	//printf("speedf2 : %d\n", mas_numb[3]);
	pformatFPH3.speedf3 = mas_numb[4];
	// printf("speedf3 : %d\n", pformatFPH3.speedf3);
	return 1;	
}	

unsigned int readUART(int fd_tx, int fd_rx){
	int n =0;
    printf("n = write(fd_tx,... )\n");
	n = write(fd_tx, "#r\r\n", 4);
	if (n < 0){
		fputs("write() of 4 bytes failed!\n", stderr);
		return 0;
    } else
       {
			printf("Successfully wrote 4 bytes\n");
            char buffer[32];
            n = read(fd_rx, buffer, sizeof(buffer));
			if (n < 0){
                fputs("read failed!\n", stderr);
				return 0;
            }else {
                printf("OK. Read from serial port: %s (%d)\n", buffer, n);
				parseData(buffer, n);
			}	
        }
	return 1;	
};

int GetTemperature(){
	return pformatFPH3.temperature;
}; 

int GetHumidity(){
	return pformatFPH3.humidity;
};

int GetSpeedf1(){
	return pformatFPH3.speedf1;
};

int GetSpeedf2(){
	return pformatFPH3.speedf2;
};

int GetSpeedf3(){
	return pformatFPH3.speedf3;
};
	

