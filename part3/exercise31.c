#include<stdio.h>

int button(int time,int timebutton[]);

int main(int argc, char const *argv[]) {
  int timebutton[3]= {600, 60, 10};
  int time,flag = 1;
  while (flag) {
    printf("%s\n","type the time required:" );
    if (scanf("%d",&time) != 1) {
      flag = 0;
    };
    if (flag) {
      printf("number of button pressed = %d\n", button(time,timebutton));
    }
  }
  return 0;
}

int button(int time,int timebutton[]){
  int i = 0,cnt = 0;
  while (i < 3) {
    if (time >= timebutton[i]) {
      time -= timebutton[i];
      cnt ++ ;
    }else{
      i++;
    }
  }
  cnt += time==0?0:1;
  return cnt;
}