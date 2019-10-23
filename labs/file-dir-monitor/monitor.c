#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define BUF_LEN sizeof(struct inotify_event) * 1024
/*
inotify_init(2) creates an inotify instance and returns a file 
descriptor referring to the inotify instance.

inotify_add_watch(2) manipulates the "watch list" associated with
an inotify instance. Each watch has a unique "watch descriptor", 
an integer returned by inotify_add_watch(2) when the watch is created.

When events occur for monitored files and directories, those
events are made available to the application as structured data
that can be read from the inotify file descriptor using read(2)

inotify_rm_watch(2) removes an item from an inotify watch list.
*/

int fd;
int wd;
int rd;
char* p;
struct inotify_event *event;
void displayEvent(struct inotify_event* event);
int main(char argc, char** argv){
    if(argc < 2){
        printf("Error, please specify a directory to watch over\n");
        return -1;
    }
    fd = inotify_init1(O_NONBLOCK);
    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    char* buffer = (char*)malloc(BUF_LEN);
    while(1){
        rd = read(fd, buffer, BUF_LEN);
        p = buffer;
        event = (struct inotify_event*)p;
        //if(event->mask & IN_CREATE ){ printf("Subdirectorio creado"); }
        for (p = buffer; p < buffer + rd; ) {
             event = (struct inotify_event *) p;
             displayEvent(event);
             p += sizeof(struct inotify_event) + event->len;
         }
    }
    close(fd);
    return 0;
}

void displayEvent(struct inotify_event* event){
     if (event->mask & IN_ACCESS)    infof("Archivo o directorio accesado\n");
     if (event->mask & IN_CREATE)        warnf("Subdirectorio o archivo creado\n");
     if (event->mask & IN_DELETE)        warnf("Subdirectorio o archivo eliminado\n");
     if (event->mask & IN_OPEN)          infof("Directorio o archivo abierto\n");
     if (event->mask & IN_MODIFY)          warnf("Modificacion en archivo del directorio\n");
}
