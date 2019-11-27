long *readMatrix(char *filename);
long *getColumn(int col, long *matrix);
long *getRow(int row, long *matrix);
int getLock(void);
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long *multiply(long *matA, long *matB);
int saveResultMatrix(long *result);
void *threadFunc(void *arg);

struct MatMul {
  long *matA;
  long *matB;
  size_t rowPosition;
  size_t colPosition;
  long *result;
};