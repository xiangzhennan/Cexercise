#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define ROW 20
#define COLUMN 40
#define ITERATION 50

enum bool{false,true};
typedef enum bool bool;

void setzero(char grid[][COLUMN]);
void initialise(char grid[][COLUMN], char* name );
void showgrid(char grid[][COLUMN]);
void conduct(char gridold[][COLUMN],char gridnew[][COLUMN]);
bool safe(int i, int j);

int main(int argc, char *argv[]) {
  int i;
  char grid1[20][40];
  char grid2[20][40];
  setzero(grid1);
  setzero(grid2);
  initialise(grid1,argv[1]);
  for (i = 0; i < ITERATION; i++) {
    if (i % 2==0) {
      conduct(grid1,grid2);
      showgrid(grid2);
    }else{
      conduct(grid2,grid1);
      showgrid(grid1);
    }
    sleep(1);
  }
  return 0;
}

void setzero(char grid[][COLUMN]){
  int i , j;
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 40; j++) {
      grid[i][j] = 0;
    }
  }
}
void initialise(char grid[][COLUMN], char* name ){
  char* cur = &grid[0][0];
  char c;
  FILE* file = fopen("wirecircuit1.txt","r");
  for (size_t i = 0; i < 20; i++) {
    while((c = fgetc(file))!='\n'){
      *cur = c;
      cur ++ ;
    }
    cur = &grid[i][0];
  }
  fclose(file);
}

void showgrid(char grid[][COLUMN]){
  int i, j;
  for ( i = 0; i < 20; i++) {
    for (j = 0; j < 40; j++) {
      if (grid[i][j] == 0) {
        printf(" ");
      }else{
        printf("%c",grid[i][j]);
      }
    }
    printf("\n");
  }
}

bool safe(int i, int j){
  return i>=0&&i<ROW&&j>=0&&j<COLUMN;
}
void conduct(char gridold[][COLUMN],char gridnew[][COLUMN]){
  int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  int i, j,k,x,y,cnt;
  for ( i = 0; i < 20; i++) {
    for (j = 0; j < 40; j++) {
      if (gridold[i][j] == 'H') {
        gridnew[i][j] = 't';
      }
      if (gridold[i][j] == 't') {
        gridnew[i][j] = 'c';
      }
      if (gridold[i][j] == 'c') {
        cnt = 0;
        for(k = 0; k < 8; k++){
          x = i + directions[k][0];
          y = j + directions[k][1];
          if (safe(x,y)&&gridold[x][y] == 'H') {
            cnt++;
          }
        }
        if (cnt == 1||cnt == 2) {
          gridnew[i][j] = 'H';
        }else{
          gridnew[i][j] = 'c';
        }
      }
    }
  }
}