#define BAUDRATE B921600
#define MODEMDEVICE "/dev/ttyPS1"
#define _POSIX_SOURCE 1         //POSIX compliant source
#define FALSE 0
#define TRUE 1
#define MAX_LEN_TTY 14
#define NOM_UART 1

int GetTemperature();
int GetHumidity();
int GetSpeedf1();
int GetSpeedf2();
int GetSpeedf3();

// int openUART();
int openUART1();
unsigned int closeUART(int fd);
unsigned int writeUART(int fd, char *ini_tty);
unsigned int readUART(int fd);
unsigned int writeBuzzerUART(int fd, int status);

// int  fd_UART;
