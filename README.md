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
./irlm
# or add the application to a directory on PATH
cp irlm .local/bin
irlm
```
If you recieve the error `Cannot open /dev/ttyUSB0`, make sure your user is a member of the `dialout` group.
```bash
newgrp dialout
sudo usermod -a -G dialout <username>
# Verify that your user is now a member of the dialout group
groups <username>
```

## Usage:
To edit the remote's current settings:
```
edit   [option]    [suboption(s)]
       device      device number
       command     device number     command number
```
