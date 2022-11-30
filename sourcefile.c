
int main(void){
  long a[100], b[100], c[100];
  int M;
  int i, j, k;
  long r, val;

  // M = 10;

  // do the matrix multiplication: note that the
  // loops are structured to avoid iterating over a column
  // of elements and incurring excessive cache misses
  // as a result
  val = c[i * M + i];
  // for(k = 0; k < M; k = k + 1){
  //   for(i = 0; i < M; i = i + 1){
  //     for(j = 0; j < M; j = j + 1){
  //     }
  //   }
  // }

  return 0;
}
