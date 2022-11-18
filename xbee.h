#ifndef XBEE
#define XBEE

#include "serial_interface.h"

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

int8_t init_xbee();

int8_t handshake();

int8_t issue_xbee_command(Command command/* , int argc, char** argv */);

#endif /* !XBEE */
