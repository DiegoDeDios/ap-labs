#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int openFile(char* filename){
    int fd = open(filename, O_RDONLY);
    return fd;
}

off_t getSize(int fd){
    off_t curOffset = lseek(fd, (size_t)0, SEEK_CUR);
    off_t size = lseek(fd, (size_t)0, SEEK_END); 
    lseek(fd, curOffset, SEEK_SET);
    return size;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("You must specify a file to print con the stdout\n");
        return -1;
    }
    int fd = openFile(argv[1]);
    if(fd == -1){
        printf("Invalid file\n");
        return -1;
    }
    off_t size = getSize(fd);
    char* fileBuffer = (char*)malloc(sizeof(char)*size);
    read(fd,fileBuffer,size);
    write(1,fileBuffer,size);
    close(fd);
    free(fileBuffer);
    return 0;
}
 