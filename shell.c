#include "xbee.h"

void print_usage();

int main()
{
    if (init_xbee() < 0) return 1;

    char input[1024] = { '\0' };
    char command[64];
    
    uint16_t argc;
    char* argv[16];
    
    while (1) {
        /* Take in user input */
        printf(">> ");
        fgets(input, 1024, stdin);
        
        /* Remove ending newline */
        if (strlen(input) == 1) continue;
        input[strlen(input) - 1] = '\0';
        
        /* Isolate command and arguments */
        strcpy(command,strtok(input," "));
        argc = 0;
        while ((argv[argc] = strtok(NULL, " ")) != NULL) ++argc;

        /* Act based on user command */
        if (!strcasecmp(command, "quit")) {
            issue_xbee_command(QUIT, 0, NULL);
            return 0;
        } else if (!strcasecmp(command, "edit")) {
            if (!strcasecmp(argv[0], "device")) {
                if (argc != 2) {
                    print_usage();
                    continue;
                }
                issue_xbee_command(EDIT_DEVICE, 1, argv + 1);
            } else if (!strcasecmp(argv[0], "command")) {
                if (argc != 3) {
                    print_usage();
                    continue;
                }
                issue_xbee_command(EDIT_COMMAND, 2, argv + 1);
            }
        } else if (!strcasecmp(command, "export")) {
            if (!argc) {
                print_usage();
                continue;
            }
            if (!strcasecmp(argv[0], "configuration")) {
                issue_xbee_command(EXPORT_CONFIGURATION, 0, NULL);
            }
        } else if (!strcasecmp(command, "import")) {
            if (!argc) { 
                print_usage(); 
                continue; 
            }
            if (!strcasecmp(argv[0], "configuration")) {
                issue_xbee_command(IMPORT_CONFIGURATION, 0, NULL);
            }
        } else {
            print_usage();
        }
    }

    return 0;
}

void print_usage() {
    printf("                                 Valid Commands:                                 \n\n");
    printf("                     to EDIT the remote's current settings:                      \n");
    printf("---------------------------------------------------------------------------------\n");
    printf(" edit      [option]         [suboption(s)]                                       \n");
    printf("           device           device number                                        \n");
    printf("           command          device number     command number                     \n");
    printf("---------------------------------------------------------------------------------\n\n");
    printf("to EXPORT saved settings, potentially overwriting the remote's current settings: \n");
    printf("---------------------------------------------------------------------------------\n");
    printf(" export    [option]         [suboption(s)]                                       \n");
    printf("           configuration    save name                                            \n");
    // printf("           device           device number     save name                          \n");
    // printf("           command          device number     command number    save name        \n");
    printf("---------------------------------------------------------------------------------\n\n");
    printf("     to IMPORT the remote's current settings, saving them to this computer:      \n");
    printf("---------------------------------------------------------------------------------\n");
    printf(" import    [option]         [suboption(s)]                                       \n");
    printf("           configuration    save name                                            \n");
    // printf("           device           device number     save name                          \n");
    // printf("           command          device number     command number    save name        \n");
    // printf("---------------------------------------------------------------------------------\n\n");
    // printf("                        to view or modify saved settings:                        \n");
    // printf("---------------------------------------------------------------------------------\n");
    // printf(" saved     [option]         [suboption(s)]                                       \n");
    // printf("           view             configuration     saved configuration name           \n");
    // printf("                            device            saved device name                  \n");
    // printf("                            command           saved command name                 \n");
    // printf("           edit             configuration     saved configuration name           \n");
    // printf("                            device            saved device name                  \n");
    // printf("                            command           saved command name                 \n");
    printf("---------------------------------------------------------------------------------\n\n");
    printf("                     to VIEW the remote's current settings:                      \n");
    printf("---------------------------------------------------------------------------------\n");
    printf(" view      [option]         [suboption(s)]                                       \n");
    printf("           configuration                                                         \n");
    printf("           device           device number                                        \n");
    printf("---------------------------------------------------------------------------------\n\n");
    printf("                             other useful commands:                              \n");
    printf("---------------------------------------------------------------------------------\n");
    printf(" help: display this usage guide                                                  \n");
    printf(" quit: disconnect from the XBee and exit the CLI                                 \n");
    printf("---------------------------------------------------------------------------------\n\n");
}
