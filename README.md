# EECS373_IR_Learning_Remote
Creating an IR Learning Remote for our EECS 373 Final Project.
## Organization
### Trunk:
Trunk houses the current stable build for the STM32 dev board.  This can easily be flashed to the board using the STM32CubeIDE and will initialize the controller with a default set of devices and commands which are ready to be programmed!
### Command Line Interface:
The CLI is housed under dev/interface.  Clone it to interact with the remote! (Note: Currently this is only compatible with Linux and uses /dev/ttyUSB0 by default.  The port is changeable at the top of serial_interface.c, visit dev/interface for more information)
#### Usage:
To edit the remote's current settings:
```
edit   [option]    [suboption(s)]
       device      device number
       command     device number     command number
```
