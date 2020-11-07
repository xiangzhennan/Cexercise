#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define ROW 3
#define COLUMN 3
bool safe(int i, int j);
void expand(int grid[][COLUMN],int i , int j, int step);
void show(int grid[][COLUMN]);

int main(int argc, char const *argv[]) {
  int i,j;
  int grid[ROW][COLUMN];
  for (i = 0; i < ROW; i++) {
    for (j= 0; j < COLUMN; j++) {
      grid[i][j] = 0;
    }
  }
  grid[1][1] = 1;
  expand(grid,1,1,2);
  return 0;
}

bool safe(int i, int j){
  return i>=0&&i<ROW&&j>=0&&j<COLUMN;
}

void show(int grid[][COLUMN]){
  int i,j;
  for (i = 0; i < ROW; i++) {
    for (j= 0; j < COLUMN; j++) {
      printf("%3d",grid[i][j] );
    }
    printf("\n");
  }
  printf("\n");
}

void expand(int grid[][COLUMN],int i , int j, int step){
  int k,x,y;
  static int directions[4][2] = {{-1,0},{0,-1},{0,1},{1,0}};
  if (step > ROW*COLUMN) {
    show(grid);
    return;
  }
  for (k = 0; k< 4;k++) {
    x = i + directions[k][0];
    y = j + directions[k][1];
    if (safe(x,y)&&!grid[x][y]) {
      grid[x][y] = step;
      expand(grid,x,y,step+1);
      grid[x][y] = 0;
    }
  }
}