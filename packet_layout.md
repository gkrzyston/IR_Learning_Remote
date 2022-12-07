# Packet Layouts
## Edit Device:
```
HOST TX: 12 byte transmission
    command code:   edev
    bytes[11:10]:   <device number>
    bytes[9:0]:     <new device name>
CLIENT TX:
    N/A
```
## Edit Command:
```
HOST TX: 14 byte transmission
    command code:   ecom
    bytes[13:12]:   <device number>
    bytes[11:10]:   <command number>
    bytes[9:0]:     <new command name>
CLIENT TX:
    N/A
```
## Export Configuration:
```
HOST TX: 2219 byte transmission
    command code:   xcon
    bytes[2218:0]:  <configuration file>
CLIENT TX:
    Acknowledge:    1
HOST TX: 2219 byte transmission
    bytes[2218:0]:  <configuration file>
CLIENT TX:
    Acknowledge:    1
...
```
## Import Configuration:
```
HOST TX:
    icon
CLIENT TX: 2219 byte transmission
    bytes[2218:0]: <configuration file>
HOST TX:
    Acknowledge:    1
CLIENT TX: 2219 byte transmission
    bytes[2218:0]: <configuration file>
HOST TX:
    Acknowledge:    1
...
```
## View Configuration:
```
HOST TX:
    vcon
CLIENT TX: 198 byte transmission
    <device list, \n deliminated>
```
## View Device:
```
HOST TX: 2 byte transmission
    vdev<device number>
CLIENT TX: 600 byte transmission
    <command list, \n deliminated>
```
## Configuration File Format:
Currently, there are 18 devices and 24 commands per device, '\n' deliminated.  This configuration format would need to be modified to support a customizable number of devices/buttons.
Total size of configuration file: 39942 bytes. Sent in 2219 byte chunks (separated by device).
```
11 bytes:      <Device 1 Name>\n
11 bytes:      <Command 1 Name>\n
81 bytes:      <Command 1 Data>\n
11 bytes:      <Command 2 Name>\n
...
81 bytes:      <Command 24 Data>
11 bytes:      <Device 2 Name>
...
```
