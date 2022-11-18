#include "xbee.h"

int8_t handshake_fail();

/* Open the serial connection to the xbee */
int8_t init_xbee() {
    if (open_interface() < 1) return -1;
    if (handshake() < 0) return -1;
    return 0;
}

/* Attempt to handshake with the XBee Device */
int8_t handshake() {
    printf("Searching for XBee Device...\t");
    
    char loading_symbol[5] = "\\|/-";
    uint8_t idx = 0;
    
    for (uint32_t i = 0; i < 20; ++i) {
        if (read_acknowledgement()) {
            printf(" \nXBee Device Found!\n");
            usleep(50000);
            return 1;
        }

        if (!(i%3)) {
            printf("%c\b", loading_symbol[idx]);
            fflush(stdout);
            if (idx < 3) ++idx;
            else idx = 0;
        }
            
        write_interface("1");
        usleep(1000);
    }

    if (handshake_fail() < 0) return -1;
    return handshake();
}

/* Wait for the device to say it is ready to recieve the command */
/* then issue command with optional arguments */
int8_t issue_xbee_command(Command command/* , int argc, char** argv */) {
    char msg_buf[8192];

    switch(command) {
        case IMPORT_CONFIGURATION:
            write_interface("icon");
            read_interface(msg_buf);
            /* TODO: SAVE CONFIGURATION SOMEWHERE */
            printf("Imported Configuration:\n%s",msg_buf);
            usleep(50000);
            break;
        case QUIT:
            write_interface("quit");
            break;
        default:
            break;
    }
    return 1;
}

/* * * * * * * * * * * * * * * */
/* Helper Function Definitions */
/* * * * * * * * * * * * * * * */

/* Prompt the user to repeat the handshake or exit program */
int8_t handshake_fail() {
    printf(" \nDevice not found...\nTry Again? (y/n): ");
    
    char buf[1024] = {'\0'};
    fgets(buf, 1024, stdin);

    switch(buf[0]) {
        case 'y':
            return 1;
            break;
        case 'n':
            return -1;
            break;
        default:
            printf("\nInvalid input, please select either 'y' or 'n'.\n");
            handshake_fail();
            break;
    }
    return -1;
}
