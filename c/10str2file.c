#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
main() {
    int fd=creat("useless_file",S_IWRITE|S_IREAD);
    char buf[256];
    int i=0;
    while (i!=10) {
     int size = read(0,buf,256);
     write(fd,buf,size);
     i++;
    }
    close(fd);
}
