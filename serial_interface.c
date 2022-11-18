#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>

#include "serial_interface.h"

extern const char* TERMINAL;

/* Opens a serial connection to port specified by TERMINAL */
/* 9600 Baud Rate, 8 bits of data, no parity bit or flow control */
/* Adapted From: https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c */
int open_interface() {
    int fd = open(TERMINAL, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", TERMINAL, strerror(errno));
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
