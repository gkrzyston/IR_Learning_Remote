#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "serial_interface.h"
#include "xbee.h"

/* TODO: Create Slelection Menu for choosing USB Device */
const char* TERMINAL = "/dev/ttyUSB0";

int main()
{
    int fd = open_interface();
    if (fd < 0) return 1;
    
    if (handshake(fd) < 0) return 1;
    
    issue_xbee_command(fd, IMPORT_CONFIGURATION, 0, NULL);
    issue_xbee_command(fd, QUIT, 0, NULL);
    return 0;
}
