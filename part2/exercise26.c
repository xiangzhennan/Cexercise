#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
//thats the length of the side and radius
#define R 200000

int getnumber();

int main(int argc, char const *argv[]) {
  double cnthc = 0.0;//hit the circle
  double pi = 0.0;
  srand(time(NULL));
  for (int i = 1; i < 200000000; i++) {
    int x = getnumber();
    int y = getnumber();
    if(pow(x,2.0)+pow(y,2.0)<=pow(R,2.0)){
      cnthc ++;
    }
    if (!(i % 5000000)) {
      printf("PI is approximately %f\n", 4.0*cnthc/(double)i );
    }
  }

  return 0;
}

int getnumber(){
  return rand() % (R+1);//that ensures the return value hits the square
}