/* TODO: Create Slelection Menu for choosing USB Device */
#define TERMINAL    "/dev/ttyUSB0"

#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>

typedef enum Commands{
    /* 
     * code:        'rdev' 
     * arguments:   none
     */
    READ_DEVICE,
    /* 
     * code:        'vcom' 
     * arguments:   device_number
     */
    VIEW_COMMMANDS,
    /*
     * code:        'icon'
     * arguments:   none
     */
    IMPORT_CONFIGURATION,
    /*
     * code:        'econ'
     * arguments:   none
     */
    EXPORT_CONFIGURATION,
    /* 
     * code:        'quit'
     * arguments    none 
     */
    QUIT
} Command;

void handshake(int);
void handshake_fail(int);

/* Opens a serial connection to port specified by TERMINAL */
/* 9600 Baud Rate, 8 bits of data, no parity bit or flow control */
/* Adapted From: https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c */
int open_interface()
{
    char *portname = TERMINAL;
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }

    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)B9600);
    cfsetispeed(&tty, (speed_t)B9600);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                 /* 8-bit characters */
    tty.c_cflag &= ~PARENB;             /* no parity bit */
    tty.c_cflag &= ~CSTOPB;             /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;            /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* Wait until there are no packets transmitted for 10 milliseconds */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 3;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return fd;
}

/* Reads a message from the serial device */
/* Erases buffer passed into the function before filling it */
void read_interface(int fd, char* msg_buf) {
    msg_buf[0] = '\0';
    unsigned char buf[80];
    int rdlen;
    /* consider adding a timeout for the while loop */
    while(1) {
        rdlen = read(fd, buf, sizeof(buf) - 1);
        if (rdlen > 0) {
            buf[rdlen] = '\0';
            strcat(msg_buf, (const char*)buf);
        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        } else {  /* Message Recieved */
            if (msg_buf[0] != '\0') break;
        }               
    }
}

/* Writes the specified message to the serial device */
void write_interface(int fd, char* msg) {
    int msg_len = strlen(msg);
    int sent_len = write(fd, msg, msg_len);
    if (sent_len != msg_len) {
        printf("Error from write: %d, %d\n", sent_len, errno);
    }
    tcdrain(fd);
}

/* Return true if any character is sent back from the XBee Device */
int read_acknowledgement(int fd) {
    unsigned char buf[80];
    int rdlen = read(fd, buf, sizeof(buf) - 1);
    return rdlen != 0;
}

/* Prompt the user to repeat the handshake or exit program */
void handshake_fail(int fd) {
    printf(" \nDevice not found...\nTry Again? (y/n): ");
    
    size_t bytes = 4;
    char* empty_buf = (char *) malloc (bytes + 1);
    getline(&empty_buf, &bytes, stdin);
    char c =  empty_buf[0];

    switch(c) {
        case 'y':
            handshake(fd);
            break;
        case 'n':
            exit(0);
            break;
        default:
            printf("\nInvalid input, please select either 'y' or 'n'.\n");
            handshake_fail(fd);
            break;
    }
}

/* Attempt to handshake with the XBee Device */
void handshake(int fd) {
    printf("Searching for XBee Device...\t");
    char loading_symbol[5] = "\\|/-";
    uint8_t idx = 0;
    for (uint32_t i = 0; i < 20; ++i) {
        if (read_acknowledgement(fd)) {
            printf(" \nXBee Device Found!\n");
            usleep(50000);
            return;
        }

        if (!(i%3)) {
            printf("%c\b", loading_symbol[idx]);
            fflush(stdout);
            if (idx < 3) ++idx;
            else idx = 0;
        }
            
        write_interface(fd, "1");
        usleep(1000);
    }
    handshake_fail(fd);
}

/* Wait for the device to say it is ready to recieve the command */
/* then issue command with optional arguments */
void issue_xbee_command(int fd, Command command, int argc, char** argv) {
    char msg_buf[8192];

    switch(command) {
        case IMPORT_CONFIGURATION:
            write_interface(fd, "icon");
            read_interface(fd, msg_buf);
            /* TODO: SAVE CONFIGURATION SOMEWHERE */
            printf("Imported Configuration:\n%s",msg_buf);
            usleep(50000);
            break;
        case QUIT:
            write_interface(fd, "quit");
            exit(0);
            break;
        default:
            break;
    }
}

int main()
{
    int fd = open_interface();
    handshake(fd);
    issue_xbee_command(fd, IMPORT_CONFIGURATION, 0, NULL);
    issue_xbee_command(fd, QUIT, 0, NULL);
    
    /* Simple working output */
    /* char msg_buf[8192]; */
    /* read_interface(fd, msg_buf); */
    /* printf("%s", msg_buf); */
}
