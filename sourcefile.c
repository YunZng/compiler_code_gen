int main(void){
  int x, y, z;
  x = 3;
  y = x + 7;
  z = 2 * y;
  if(x < y){
    z = x / 2 + y / 3;
  } else{
    z = x * y + y;
  }
  return z;
}
