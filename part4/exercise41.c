#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define ITERATION 10
#define ROW 20
#define COLUMN 20

enum bool{false, true};
typedef enum bool bool;


void test();
void setzero(int grid[][COLUMN]);
void generate(int gridold[][COLUMN],int gridnew[][COLUMN],int directions[][2]);
bool safe(int i, int j);
void lifechange(int* target, int origin, int cnt);

int main(int argc, char const *argv[]) {
  //test();
  char c;
  int i, x, y;
  int grid1[ROW][COLUMN];
  int grid2[ROW][COLUMN];
  int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  setzero(grid1);
  setzero(grid2);
  FILE* fop = fopen("file1.lif","r");
  while((c=fgetc(fop)) != '\n');
  for ( i=0; i < 4 ;i++) {
    fscanf(fop,"%d%d\n",&x,&y);
    grid1[x][y] = 1;
  }
  generate(grid1,grid2,directions);
  printf("now grid looks like this:\n");
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      printf("%2d",grid2[i][j]);
    }
    printf("\n");
  }
  fclose(fop);
  return 0;
}

void setzero(int grid[][COLUMN]){
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      grid[i][j] = 0;
    }
  }
}

void test(){
  int grid1[ROW][COLUMN];
  int grid2[ROW][COLUMN];
  int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  setzero(grid1);
  setzero(grid2);
  grid1[1][2] = 1;
  grid1[2][2] = 1;
  grid1[3][2] = 1;
  lifechange(&grid2[0][0],grid1[0][0],3);
  assert(grid2[0][0] == 1);
  printf("origin grid looks like this:\n" );
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      printf("%2d",grid1[i][j]);
    }
    printf("\n");
  }
  generate(grid1,grid2,directions);
  printf("now grid looks like this:\n");
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      printf("%2d",grid2[i][j]);
    }
    printf("\n");
  }
}

bool safe(int i, int j){
  return i>=0&&i<ROW&&j>=0&&j<COLUMN;
}

void generate(int gridold[][COLUMN],int gridnew[][COLUMN],int directions[][2]){
  int cnt,x,y,i,j,k;
  for (i = 0; i < ROW; i++) {
    for (j = 0; j < COLUMN; j++) {
      cnt = 0;
      //count how many neighbours inhabited
      for (k = 0; k < 8; k++) {
        x = i + directions[k][0];
        y = j + directions[k][1];
        if (safe(x,y)&&gridold[x][y]) {
          cnt ++;
        }
      }
      lifechange(&gridnew[i][j],gridold[i][j],cnt);
    }
  }
}

void lifechange(int* target, int origin, int cnt){
  if (cnt == 3) {
    *target = 1;
  }
  if (cnt == 2) {
    *target = origin;
  }
  if (cnt < 2 || cnt > 3) {
    *target = 0;
  }
}