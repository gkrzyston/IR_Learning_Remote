#ifndef XBEE
#define XBEE

#include "serial_interface.h"

typedef enum Commands{
    EDIT_DEVICE,
    EDIT_COMMAND,
    
    EXPORT_CONFIGURATION,
    IMPORT_CONFIGURATION,
    
    VIEW_CONFIGURATION,
    VIEW_DEVICE,
    
    QUIT
} Command;

int8_t init_xbee();

int8_t handshake();

int8_t issue_xbee_command(Command command, int argc, char** argv);

#endif /* !XBEE */
