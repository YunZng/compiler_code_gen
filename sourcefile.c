struct Point{
  int x, y;
};

int main(void){
  struct Point p, * q;

  q = &p;

  q->y = 3;

  return q->y;
}
