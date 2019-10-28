#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h> 
#include <ctype.h>
#include <errno.h>
#include <sys/inotify.h>
#define BUF_LEN sizeof(struct inotify_event) * 1024
//Global variables -----
int IDS[1024];
int fd;
int wd;
int rd;
char* p;
struct inotify_event *event;
//--------------------------


struct Process{
  int pid; //Field 1
  int parent; //Field 4
  char* name; //Field 2
  char* state; //Field 3
  int memory;  // Field 23
  int threads; // Field 20 of stat
  int op_files; // located at subdirectory id/fd
};


void clear();
int getProcessList();
char isProcess(char* id);
int countFD(int pid);
void listProcess(int pid);
int openFile(char* filename);
off_t getSize(int fd);
void eventHandler(struct inotify_event* event);
void top();

int main(){

  //Place a watcher on the proc directory
    fd = inotify_init1(O_NONBLOCK);
    wd = inotify_add_watch(fd, "/proc", IN_ALL_EVENTS);
    char* buffer = (char*)malloc(BUF_LEN);


  //-------

  top();


  //Event handler
      while(1){
        rd = read(fd, buffer, BUF_LEN);
        p = buffer;
        event = (struct inotify_event*)p;
        for (p = buffer; p < buffer + rd; ) {
             event = (struct inotify_event *) p;
             eventHandler(event);
             p += sizeof(struct inotify_event) + event->len;
         }
    }
    close(fd);

  return 0;
}

void top(){
  int proc = getProcessList();
  printf("|%5s|%6s|%19s|%10s|%8s|%5s|%5s|\n", "PID","Parent","Name","State","Memory","Threads","Files");
  for(int i = 0; i < proc; i++){
     listProcess(IDS[i]);
  }
}
void eventHandler(struct inotify_event* event){
     if (event->mask & IN_CREATE){      
          clear();
          top();
      }
     if (event->mask & IN_DELETE){      
        clear();
        top();
       }
     if (event->mask & IN_MODIFY){     
        clear();
        top();
        }
}

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

void clear() {
  printf("\e[1;1H\e[2J"); 
}

void listProcess(int pid){
    char filename[512];
    sprintf(filename, "/proc/%d/stat", pid);
    FILE *f = fopen(filename, "r");
    int unused;
    char* comm = (char*)malloc(sizeof(char)*256);
    char state;
    int ppid;
    int threads;
    unsigned int mem;
    fscanf(f, "%*d %s %c %d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %d %*ld %*llu %u", comm, &state, &ppid, &threads, &mem);
    fclose(f);

    int op_files = countFD(pid);
    struct Process* newProc = (struct Process*)malloc(sizeof(struct Process));
    newProc->pid = pid;
    newProc->name = comm;
    newProc->parent = ppid;
    newProc->state = state;

    switch (state)
    {
    case 'R':
    newProc->state = "Running";
      break;
    case 'S':
    newProc->state = "Sleeping";
      break;
    case 'D':
    newProc->state = "Idle";
      break;
    case 'Z':
    newProc->state = "Zombie";
      break;
    case 'T':
    newProc->state = "Stopped";
      break;
    case 't':
    newProc->state = "Trace stopped";
      break;
    case 'X':
    newProc->state = "Dead";
      break;
    case 'x':
    newProc->state = "Dead";
      break;
    case 'K':
    newProc->state = "Wakekill";
      break;
    case 'W':
    newProc->state = "Waking";
      break;
    case 'P':
    newProc->state = "Parked";
      break;
    default:
    newProc->state = "Idle";
      break;
    } 
    newProc->op_files = op_files;
    newProc->threads = threads;
    newProc->memory = mem/10000000;
    printf("|%5d|%6d|%19s|%10s|%8luM|%5d|%5d| \n", newProc->pid, newProc->parent, newProc->name, newProc->state, newProc->memory, newProc->threads, newProc->op_files);
    free(newProc);
} 

int countFD(int pid){
  char fdir[512];
  sprintf(fdir, "/proc/%d/fd", pid);
  int fd_open = 0;
  DIR *d;
  struct dirent *dir;
  d = opendir(fdir);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      fd_open++;
    }
    closedir(d);
  }
  return fd_open;
}

char isProcess(char* id){
    size_t length = strlen(id);
    for (int i=0;i<length; i++){
      if (!isdigit(id[i]))
        {
            return 0;
        }
    }
    return 1;
}

int getProcessList(){ //Returns number of processes
  int i = 0;
  DIR *d;
  struct dirent *dir;
  d = opendir("/proc");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(isProcess(dir->d_name)){
        IDS[i++] = atoi(dir->d_name);
        //printf("%s\n", dir->d_name);
      }
    }
    closedir(d);
  }
  return i;
}
