void print_str(const char* s);
void print_nl(void);
void print_i32(int n);

struct Person{
  char name[20];
  int age;
};

int main(void){
  struct Person p;
  p.name[0] = 'A';
  const char* s;
  s = p.name;

  return 0;
}
