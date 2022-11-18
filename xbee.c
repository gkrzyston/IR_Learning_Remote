#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "xbee.h"
#include "serial_interface.h"

/* Attempt to handshake with the XBee Device */
int8_t handshake(int fd) {
    printf("Searching for XBee Device...\t");
    char loading_symbol[5] = "\\|/-";
    uint8_t idx = 0;
    for (uint32_t i = 0; i < 20; ++i) {
        if (read_acknowledgement(fd)) {
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
            
        write_interface(fd, "1");
        usleep(1000);
    }
    if (handshake_fail(fd) < 0) return -1;
    else return 1;
}

/* Prompt the user to repeat the handshake or exit program */
int8_t handshake_fail(int fd) {
    printf(" \nDevice not found...\nTry Again? (y/n): ");
    
    char buf[1024] = {'\0'};
    fgets(buf, 1024, stdin);

    switch(buf[0]) {
        case 'y':
            handshake(fd);
            return 1;
            break;
        case 'n':
            return -1;
            break;
        default:
            printf("\nInvalid input, please select either 'y' or 'n'.\n");
            handshake_fail(fd);
            return 1;
            break;
    }
}

/* Return true if any character is sent back from the XBee Device */
int read_acknowledgement(int fd) {
    unsigned char buf[80];
    int rdlen = read(fd, buf, sizeof(buf) - 1);
    return rdlen != 0;
}

/* Wait for the device to say it is ready to recieve the command */
/* then issue command with optional arguments */
int8_t issue_xbee_command(int fd, Command command, int argc, char** argv) {
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
            break;
        default:
            break;
    }
    return 1;
}
