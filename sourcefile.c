// This program multiplies 10x10 square matrices,
// represented as 100 element 1-D arrays

long read_i64(void);
void print_i64(long n);
void print_nl(void);

void read_m10x10(long* arr){
  int i;
  for(i = 0; i < 100; i = i + 1){
    arr[i] = read_i64();
  }
}

int main(void){
  long a[100], b[100], c[100];
  int M;
  int i, j, k;
  long r, val;

  M = 10;

  // do the matrix multiplication: note that the
  // loops are structured to avoid iterating over a column
  // of elements and incurring excessive cache misses
  // as a result
  for(k = 0; k < M; k = k + 1){
    for(i = 0; i < M; i = i + 1){
      for(j = 0; j < M; j = j + 1){
        val = c[i * M + j];
      }
    }
  }

  return 0;
}
