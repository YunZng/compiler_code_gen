int main(void){
  int n, i, sum, a, b, c, d;

  n = 11;
  i = 1;
  sum = 0;
  a = 1;
  b = 2;
  c = 3;
  d = 4;

  while(i <= n){
    sum = sum + i;
    i = i + 1;
  }

  return sum;
}
