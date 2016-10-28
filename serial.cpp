#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyACM0"
#define BAUDRATE B9600
#define Ethernet_PORT 12345

int main()
{
    struct timespec sleep_time = {0, 100 * 1000000};
    
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(Ethernet_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    
    char buf[1024];
    
    int fd;
    struct termios oldtio, newtio;

    fd = open(SERIAL_PORT, O_RDWR);
    if (fd < 0) {
        perror(SERIAL_PORT);
        return 1;
    }
    
    ioctl(fd, TCGETS, &oldtio);
    newtio = oldtio;
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    
    //read(fd, buf, sizeof(buf));
    
    while(1) {
        memset(buf, 0, sizeof(buf));
        recv(sock, buf, sizeof(buf), 0);
        printf("%s\n", buf);
        write(fd, buf, strlen(buf));
    }
    
    ioctl(fd, TCSETS, &oldtio);
    close(fd);
    
    return 0;
}   
