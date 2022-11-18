#ifndef XBEE
#define XBEE

#include <stdint.h>

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

int8_t handshake(int);
int8_t handshake_fail(int);

int read_acknowledgement(int fd);
int8_t issue_xbee_command(int fd, Command command, int argc, char** argv);

#endif /* !XBEE */
