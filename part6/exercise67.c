#include"common.h"

#define SIDE 27

void draw(int grid[][SIDE],int centerx,int centery,int lenside);

int main(int argc, char const *argv[]) {
  int grid[SIDE][SIDE];
  for (size_t i = 0; i < SIDE; i++) {
    for (size_t j = 0; j < SIDE; j++) {
      grid[i][j] = 0;
    }
  }
  draw(grid,SIDE/2,SIDE/2,SIDE/3);
  for (size_t i = 0; i < SIDE; i++) {
    for (size_t j = 0; j < SIDE; j++) {
      printf("%d", grid[i][j]);
    }
    printf("\n" );
  }
  return 0;
}

void draw(int grid[][SIDE],int centerx,int centery,int lenside){
  static int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  int i,j,x,y;
  if (lenside == 0) {
    return;
  }
  for (i = 0; i < lenside; i++) {
    for (j = 0; j < lenside; j++) {
      grid[centerx + i - lenside/2][centery + j -lenside/2] = 1;
    }
  }
  for (i = 0; i < 8; i++) {
    x = centerx+lenside*directions[i][0];
    y = centery+lenside*directions[i][1];
    draw(grid,x,y,lenside/3);
  }
}