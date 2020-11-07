#include <stdio.h>
#include <stdbool.h>


void generate(int a[], int b[],int rules[]);
int bi2de(int i , int j ,int k);

int main(int argc, char const *argv[]) {
  int rules[] = {0,1,1,1,0,1,1,0};
  int num1[40];
  int num2[40];
  int last = 1;
  int cnt = 0;
  for (int i = 0; i < 40; i++) {
    if (i == 36) {
      num1[i] = 1;
    }else{
      num1[i] = 0;
    }
  }
  while (cnt<40) {
    if (last == 1) {
      generate(num1, num2, rules);
      last = 2;
    }else{
      generate(num2, num1, rules);
      last = 1;
    }
    cnt++;
  }
  return 0;
}

void generate(int a[], int b[],int rules[]){
  for (int i = 0; i < 40; i++) {
    if (i-1>=0&&i+1<=39) {
      b[i] = rules[bi2de(a[i-1],a[i],a[i+1])];
    }else{
      b[i] = 0;
    }
    if (b[i]) {
      printf("%d",0);
    }else{
      printf(" ");
    }
  }
  printf("\n");
}

int bi2de(int i , int j ,int k){
  return 4*i+2*j+k;
}