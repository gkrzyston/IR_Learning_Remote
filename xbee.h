#ifndef XBEE
#define XBEE

#include "serial_interface.h"

typedef enum Commands{
    EDIT_DEVICE,            // edev
    EDIT_COMMAND,           // ecom
    
    EXPORT_CONFIGURATION,   // xcon
    IMPORT_CONFIGURATION,   // icon
    
    VIEW_CONFIGURATION,     // vcon
    VIEW_DEVICE,            // vdev
    
    QUIT
} Command;

int8_t init_xbee();

int8_t handshake();

int8_t issue_xbee_command(Command command, char** argv);

#endif /* !XBEE */
