#ifndef SERIAL_INTERFACE
#define SERIAL_INTERFACE

#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>

int open_interface();
void read_interface(char* msg_buf);
void write_interface(char* msg);
int8_t read_acknowledgement();

#endif // !SERIAL_INTERFACE
