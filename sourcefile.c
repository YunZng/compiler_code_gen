// test for an integer promotion

void print_i32(int n);
void print_nl(void);
void a(int b, int c, int d){
  print_nl();
}

int main(void){
  char c;
  c = -67;
  print_i32(c);
  print_nl();
  a(1, 2, 3);
  return 0;
}
