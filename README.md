# EECS373_IR_Learning_Remote Interface
The command line interface for customizing our IR Learning Remote.

## Installation:
Currently, this program is only compatible with Linux operating systems.  Administrative rights are required to access the USB tty.
```bash
# make the executable
git clone <repository_url>
cd <new_repository_directory>
make release
# run the application
sudo ./irlm
# or add the application to a directory on PATH
cp irlm .local/bin
sudo irlm
```

## Usage:
To edit the remote's current settings:
```
edit   [option]    [suboption(s)]
       device      device number
       command     device number     command number
```
To export saved settings, potentially overwriting the remote's current settings:
```
export    [option]         [suboption(s)]
          configuration    save name
          device           device number     save name
          command          device number     command number     save name
```
To import the remote's current settings, saving them to this computer:
```
import    [option]         [suboption(s)]
          configuration    save name
          device           device number     save name
          command          device number     command number    save name

```
To view or modify saved settings:
```
saved    [option]    [suboption(s)]
         view        configuration     saved configuration name
                     device            saved device name
                     command           saved command name
         edit        configuration     saved configuration name
                     device            saved device name
                     command           saved command name
```
To view the remote's current settings:
```
view    [option]         [suboption(s)]
        configuration
        device           device number
```
## Commands to Add:
- swap (Switch the number of two devices or commands)
- order (Interactively reorder device numbers or command numbers with a device)
- issue (Transmit a signal from within the CLI)
- learn (Begin learning a new command [prompt for name, device, ect.])
- make (create a custom command from one or more existing commands)
