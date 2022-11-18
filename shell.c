#include "xbee.h"

int main()
{
    if (init_xbee() < 0) return 1;
    issue_xbee_command(IMPORT_CONFIGURATION/* , 0, NULL */);
    issue_xbee_command(QUIT/* , 0, NULL */);
    return 0;
}
