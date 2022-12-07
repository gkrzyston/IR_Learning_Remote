#include "xbee.h"

FILE* saved_conf;

int8_t handshake_fail();
void prompt_user_input(char* dest);

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
int8_t issue_xbee_command(Command command, char** argv) {
    char msg_buf[8192];
    msg_buf[0] = '\0';
    uint16_t idx = 0;
    int c;

    switch(command) {
        case EDIT_DEVICE: {
            // Error Handling
            int input_number = atoi(argv[0]);
            if (input_number <= 0 || input_number > 18) {
                printf("Error: Invalid Device Number...\n");
                break;
            }
            // Initiate Transaction
            write_interface("edev");
            // Send Device Number
            strcpy(msg_buf, argv[0]);
            strcat(msg_buf, "\n");
            // Send Updated Name
            prompt_user_input(msg_buf);
            // Write Packets
            write_interface(msg_buf);
            printf("Device Name Updated.\n");
            break;
        }
        case EDIT_COMMAND: {
            // Error Handling
            int input_number = atoi(argv[0]);
            if (input_number <= 0 || input_number > 18) {
                printf("Error: Invalid Device Number...\n");
                break;
            }
            input_number = atoi(argv[1]);
            if (input_number <= 0 || input_number > 24) {
                printf("Error: Invalid Command Number...\n");
                break;
            }
            // Initiate Transaction
            write_interface("ecom");
            // Send Device Number
            strcpy(msg_buf, argv[0]);
            strcat(msg_buf, "\n");
            // Send Command Number
            strcat(msg_buf, argv[1]);
            strcat(msg_buf, "\n");
            // Send Updated Name
            prompt_user_input(msg_buf);
            strcat(msg_buf, "\n");
            // Write Packets
            write_interface(msg_buf);
            printf("Command Name Updated.\n");
            break;
        }
        case EXPORT_CONFIGURATION: {
            write_interface("xcon");
            saved_conf = fopen(".remote_config", "r");
            while (!feof(saved_conf)) {
                c = fgetc(saved_conf);
                msg_buf[idx++] = (char)c;
            }
            msg_buf[idx - 1] = '\0';
            write_interface(msg_buf);
            printf("Remote State Overwritten.\n");
            break;
        }
        case IMPORT_CONFIGURATION: {
            write_interface("icon");
            read_interface(msg_buf);
            saved_conf = fopen(".remote_config", "w");
            fprintf(saved_conf, "%s", msg_buf);
            fclose(saved_conf);
            printf("Remote State Saved.\n");
            break;
        }
        case VIEW_CONFIGURATION: {
            write_interface("vcon");
            read_interface(msg_buf);
            printf("Current Remote Configuration:\n");
            char num_devices_str[8];
            strcpy(num_devices_str, strtok(msg_buf, "\n"));
            int num_devices = atoi(num_devices_str); 
            for (uint8_t i = 0; i < num_devices; ++i) {
                printf("\tDevice %i: %s\n", i+1, strtok(NULL, "\n"));
            }
            break;
        }
        case VIEW_DEVICE: {
            int input_number = atoi(argv[0]);
            if (input_number <= 0 || input_number > 18) {
                printf("Error: Invalid Device Number...\n");
                break;
            }
            write_interface("vdev");
            strcpy(msg_buf, argv[0]);
            strcat(msg_buf, "\n");
            write_interface(msg_buf);
            
            read_interface(msg_buf);
            printf("Device %i:\n", input_number);
            char num_cmds_str[8];
            strcpy(num_cmds_str, strtok(msg_buf, "\n"));
            int num_cmds = atoi(num_cmds_str); 
            for (uint8_t i = 0; i < num_cmds; ++i) {
                printf("\tCommand %i: %s\n", i+1, strtok(NULL, "\n"));
            }
            break;
        }
        case QUIT: {
            write_interface("quit");
            break;
        }
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

/* Prompt the user to input a new name for a device/command */
void prompt_user_input(char* dest) {
    char input[128];
    printf("Input New Description [10 character limit]: ");
    fgets(input, sizeof(input), stdin);
    
    if (strlen(input) >= 12) {
        printf("\tWarning: Device name too long, concatenated to 10 characters.\n");
        input[11] = '\n';
        input[12] = '\0';
    }

    strcat(dest, input);
}
