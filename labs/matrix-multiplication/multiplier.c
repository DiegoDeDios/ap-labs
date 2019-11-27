#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "logger.h"
#include "matrix.h"

int NUM_BUFFERS;
long **buffers;
pthread_mutex_t *mutexes;
long *result;
pthread_t threads[2000];


int main(int argc, char **argv) {
    // Place your magic here
  if (argc != 3) {
    errorf("PLEASE SPECIFY THE NUMBER OF BUFFERS\n");
    exit(EXIT_FAILURE);
  }
  NUM_BUFFERS = strtol(argv[2], NULL, 10);
  if (NUM_BUFFERS < 2){
    errorf("YOU SHOULD BE ABLE TO CREATE MORE THAN 2 BUFFERS");
    exit(EXIT_FAILURE);
  }
  infof("NUM_BUFFERS => %d\n", NUM_BUFFERS);

  buffers = (long **)malloc(NUM_BUFFERS * sizeof(long *));
  mutexes =
      (pthread_mutex_t *) malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));

  for (int i = 0; i < NUM_BUFFERS; i++) {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    mutexes[i] = mutex;
    pthread_mutex_init(&mutexes[i], NULL);
  }

  infof("pthreads created\n");

  long *matrixA, *matrixB;
  matrixA = readMatrix("matA.dat");
  matrixB = readMatrix("matB.dat");

  infof("matrices read\n");

  result = multiply(matrixA, matrixB);
  infof("Concurrentemente calculado :D\n");
  saveResultMatrix(result);
  infof("Written to DB...\n");

  free(matrixA);
  free(matrixB);
  free(buffers);
  free(mutexes);
  free(result);
  return 0;
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

long *readMatrix(char *filename) {
  int fd = openFile(filename);
  if(fd == -1){
        printf("Invalid file\n");
        return -1;
    }
  off_t size = getSize(fd);
  long* matrix = (long*)malloc(sizeof(long)*size);
  read(fd,matrix,size);
  close(fd);
  return matrix;
}

long *getColumn(int col, long *matrix) {
  if (col < 0 || col > 2000) {
    panicf("col must be 0>col>2000\n");
    exit(EXIT_FAILURE);
  }
  size_t offset = col - 1;
  long *mycol;
  mycol = (long *)malloc(2000 * sizeof(long));

  for (int i = 0; i < 2000; i++) {
    mycol[i] = matrix[offset];
    offset += 2000;
  }
  return mycol;
}

long *getRow(int row, long *matrix) {
  if (row < 0 || row > 2000) {
    panicf("Invalid row size\n");
    exit(EXIT_FAILURE);
  }
  size_t offset = ((2 * row) - 2) * 1000;
  long *myrow;
  myrow = (long *)malloc(2000 * sizeof(long));

  for (int i = 0; i < 2000; i++) {
    myrow[i] = matrix[offset++];
  }
  return myrow;
}

int getLock(void) {
  for (int i = 0; i < NUM_BUFFERS; i++) {
    if (pthread_mutex_trylock(&mutexes[i]) == 0)
      return i;
  }
  return -1;
}

int releaseLock(int lock) {
  if (pthread_mutex_unlock(&mutexes[lock]) == 0) {
    return 0;
  }
  return -1;
}

long dotProduct(long *vec1, long *vec2) {
  long result = 0;
  for (int i = 0; i < 2000; i++)
    result += vec1[i] * vec2[i];
  return result;
}

long *multiply(long *matA, long *matB) {
  result = (long *)malloc(4000000 * sizeof(long));

  for (size_t i = 0; i < 2000; i++) {
    for (size_t j = 0; j < 2000; j++) {
      struct MatMul *multiplier;
      multiplier =
          (struct MatMul *)
          malloc(sizeof(struct MatMul));
      multiplier->matA = matA;
      multiplier->matB = matB;
      multiplier->rowPosition = i + 1;
      multiplier->colPosition = j + 1;
      multiplier->result = result;
      pthread_create(&threads[j], NULL, threadFunc,
               (void *)multiplier);
    }

    for (size_t j = 0; j < 2000; j++)
      pthread_join(threads[j], NULL);
    fflush(stdout);
  }
  printf("\n");
  return result;
}

int saveResultMatrix(long *result) {
  FILE *f = fopen("result.dat", "w");
  if (f == NULL) {
    errorf("Couldn't write results\n");
    return -1;
  }

  for (size_t i = 0; i < 4000000; i++)
    fprintf(f, "%ld\n", result[i]);

  fclose(f);
  return 0;
}

void *threadFunc(void *arg) {
  struct MatMul *data = (struct MatMul *)arg;
  long index;

  int lock1, lock2;
  while ((lock1 = getLock()) == -1);
  while ((lock2 = getLock()) == -1);
  buffers[lock1] = getRow(data->rowPosition, data->matA);
  buffers[lock2] = getColumn(data->colPosition, data->matB);

  index = ((((data->rowPosition - 1) * 2000) + data->colPosition) - 1);
  data->result[index] = dotProduct(buffers[lock1], buffers[lock2]);

  free(buffers[lock1]);
  free(buffers[lock2]);
  free(arg);
  while (releaseLock(lock1) != 0);
  while (releaseLock(lock2) != 0);
  return NULL;
}