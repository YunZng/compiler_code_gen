struct Person{
  char name[20];
  int age;
};

int main(void){
  struct Person p;
  int a[2];
  // a[1] = 0;
  p.name[3] = 'A';
  return 0;
}
