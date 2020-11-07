#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define ROW 10
#define COLUMN 10

void file2maze(char* filename,char maze[][COLUMN]);
void dig(char maze[ROW][COLUMN],int i, int j);
void draw(char* c, int k);
bool safe(int i,int j);
void show(char maze[ROW][COLUMN]);

int main(int argc, char const *argv[]) {
  int i,j;
  char maze[ROW][COLUMN];
  file2maze("maze.txt",maze);
  dig(maze,1,-1);
  return maze[8][9] == ' '?1:0;
}

void file2maze(char* filename,char maze[][COLUMN]){
  int i,j;
  char c;
  char str[12];
  i = 0;
  j = 0;
  FILE* file = fopen(filename,"r");
  if (file == NULL) {
    fprintf(stderr,"file not found");
    exit(1);
  }
  while((c = fgetc(file))!='\n');
  while(fgets(str,12,file)!=NULL){
    for(j = 0;j<10;j++){
      maze[i][j] = str[j];
    }
    i++;
  }
  fclose(file);
}

void dig(char maze[ROW][COLUMN],int i, int j){
  int k;
  int x,y;
  static int directions[8][2] = {{-1,0},{0,-1},{0,1},{1,0}};
  for (k = 0; k < 4; k++) {
    x = i + directions[k][0];
    y = j + directions[k][1];
    //find path
    if (x == 8&&y == 9) {
      draw(&maze[x][y], k);
      show(maze);
      return;
    }
    if (safe(x,y)&&maze[x][y] == ' ') {
      draw(&maze[x][y], k);
      dig(maze,x,y);
      maze[x][y] = ' ';
    }
  }
}
void draw(char* c, int k){
  if (k == 1) {
    *c = '<';
  }else if (k == 2) {
    *c = '>';
  }else{
    *c = '|';
  }
}
bool safe(int i,int j){
  return i>=0&&i<ROW&&j>=0&&j<COLUMN;
}
void show(char maze[ROW][COLUMN]){
  int i,j;
  for (i = 0; i < ROW; i++) {
    for (j = 0; j < COLUMN; j++) {
      printf("%c",maze[i][j] );
    }
    printf("\n");
  }
}