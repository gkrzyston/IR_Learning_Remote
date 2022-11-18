#ifndef SERIAL_INTERFACE
#define SERIAL_INTERFACE

int open_interface();
void read_interface(int fd, char* msg_buf);
void write_interface(int fd, char* msg);

#endif // !SERIAL_INTERFACE
