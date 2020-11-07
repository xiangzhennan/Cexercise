#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include "neillsimplescreen.h"

#define ROW 20
#define COLUMN 80
/*maximun possible number of fire in grid*/
#define MAXNUM ROW*COLUMN
#define G 100
#define L 1600
/*total rounds of simulation, assuming one round is one day*/
#define DAYS 100

enum bool{false,true};
typedef enum bool bool;

void test();
void setempty(char grid[][COLUMN]);
void setzero(int a[], int length);
void showgrid(char grid[][COLUMN]);
void newday(char grid[][COLUMN],int oldfire[],int lengthold,
  int newfire[],int* lengthnew,int directions[][2]);
void burning(char grid[][COLUMN],int oldfire[],int lengthold,
  int newfire[],int* lengthnew,int directions[][2]);
void naturalchange(char grid[][COLUMN],int newfire[],int* lengthnew);
int coordinate2num(int i ,int j);
bool safe(int i, int j);
void addfire(int fire[],int* length, int x, int y);
bool lightning(char* c,int chance);
void growing(char* c,int chance);

int main(void) {
  int i, length1, length2;
  char grid[ROW][COLUMN];
  /*use fire[] to store the coordinates of each fire*/
  int fire1[MAXNUM];
  int fire2[MAXNUM];
  /*directions are used to locate the neighbour of a cell*/
  int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  test();
  length1 = 0;
  length2 = 0;
  srand(time(0));
  setzero(fire1,MAXNUM);
  setzero(fire2,MAXNUM);
  setempty(grid);
  /*clear screen, get ready to print*/
  neillclrscrn();
  for(i = 0; i< DAYS; i++){
    if (i % 2 == 0) {
      newday(grid,fire1,length1,fire2,&length2,directions);
    }else{
      newday(grid,fire2,length2,fire1,&length1,directions);
    }
    neillbusywait(1.0);
    showgrid(grid);
  }
  return 0;
}

void test(){
  int i, j;
  char grid[ROW][COLUMN];
  int a[10];
  int fire1[MAXNUM];
  int fire2[MAXNUM];
  int length1 = 1;
  int length2 = 0;
  int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  /*test setempty*/
  setempty(grid);
  for (i = 0; i < 20; i++) {
    assert(grid[rand()%ROW][rand()%COLUMN] == ' ');
  }
  /*test setzero*/
  setzero(a,10);
  for (i = 0; i < 10; i++) {
    assert(a[i] == 0);
  }
  /*set a fire manually to test burning*/
  for(i = 0; i < ROW; i++){
    for(j = 0;j < COLUMN; j++){
      grid[i][j] = '@';
    }
  }
  grid[5][5] = '*';
  fire1[0] = 405;
  setzero(fire2,MAXNUM);
  /*test burning()*/
  burning(grid,fire1,length1,fire2,&length2,directions);
  assert(grid[5][5] == ' ');
  assert(grid[4][6] == '*');
  assert(grid[5][6] == '*');
  assert(grid[6][6] == '*');

  /*test every coordinate of new fire is correctly stored in fire2[]*/
  assert(length2 == 8);
  for (i = 0; i < length2; i++) {
    assert(grid[fire2[i]/COLUMN][fire2[i]%COLUMN] == '*');
  }
  burning(grid,fire2,length2,fire1,&length1,directions);
  /*test using different fire[], old coordinates can be coverd correctly*/
  assert(length1 == 16);
  for (i = 0; i < length1; i++) {
    assert(grid[fire1[i]/COLUMN][fire1[i]%COLUMN] == '*');
  }
  /*test addfire*/
  addfire(fire1,&length1,3,10);
  assert(length1 == 17);
  assert(fire1[16]/COLUMN == 3);
  assert(fire1[16]%COLUMN == 10);
  /*test lightning, using pow(0.5,7) < 0.01*/
  for (i = 0; i < 7; i++) {
    lightning(&grid[0][0],2);
  }
  assert(grid[0][0] == '*');
  /*test growing,using pow(0.5,7) < 0.01*/
  for (i = 0; i < 7; i++) {
    growing(&grid[0][0],2);
  }
  assert(grid[0][0] == '@');

  assert(coordinate2num(10,10) == 810);
  assert(coordinate2num(0, 0) == 0);
  assert(coordinate2num(7,21) == 581);
  assert(safe(1,1));
  assert(safe(10,10));
  assert(!safe(-1,10));
  assert(!safe(14,-34));
  assert(!safe(3,900));
  assert(!safe(200,2));
}

void setempty(char grid[][COLUMN]){
  int i , j;
  for(i = 0; i < ROW; i++){
    for(j = 0;j < COLUMN; j++){
      grid[i][j] = ' ';
    }
  }
}

void setzero(int a[], int length){
  int i;
  for(i = 0; i < length; i++){
    a[i] = 0;
  }
}

void showgrid(char grid[][COLUMN]){
  int i, j;
  /*this is the old version without colour*/
  /*for(i = 0; i < ROW; i++){
    for(j = 0;j < COLUMN; j++){
      printf("%c",grid[i][j]);
    }
    printf("\n");
  }*/
  neillcursorhome();
  for(i = 0;i<ROW;i++){
    for(j = 0; j< COLUMN;j++){
      switch(grid[i][j]){
        case ' ':{
          printf("%c", ' ');
          break;
        }
        case '*':{
          neillfgcol(red);
          printf("%c",'*' );
          break;
        }
        case '@':{
          neillfgcol(green);
          printf("%c",'@');
          break;
        }
        default:{
          exit(1);
        }
      }
    }
    printf("\n");
  }
}
/*store coordinate using an integer*/
int coordinate2num(int i ,int j){
  return i * COLUMN + j;
}
/*simulate a day,meanwhile update the coordinates of fire*/
void newday(char grid[][COLUMN],int oldfire[],int lengthold,
  int newfire[],int* lengthnew,int directions[][2]){
    burning(grid,oldfire,lengthold,newfire,lengthnew,directions);
    naturalchange(grid,newfire,lengthnew);
}
/*simulate the spread of fire, store coodinates of new fire in newfire[]*/
void burning(char grid[][COLUMN],int oldfire[],int lengthold,
  int newfire[],int* lengthnew,int directions[][2]){
  int i, j, x, y;
  *lengthnew = 0;
  for(i = 0;i < lengthold;i++){
    for(j = 0; j < 8; j++){
      /*x,y used to store the coordinate of the neighbour of fire*/
      x = oldfire[i] / COLUMN + directions[j][0];
      y = oldfire[i] % COLUMN + directions[j][1];
      if (safe(x,y)&&grid[x][y] == '@'){
        grid[x][y] = '*';
        addfire(newfire,lengthnew,x,y);
      }
    }
    grid[oldfire[i]/COLUMN][oldfire[i]%COLUMN] = ' ';
  }
}
/*add coordinate of fire to fire[]*/
void addfire(int fire[],int* length, int x, int y){
  fire[(*length)++] = coordinate2num(x,y);
}
/*boundary check*/
bool safe(int i, int j){
  return i>=0&&i<ROW&&j>=0&&j<COLUMN;
}
/*simulate lightning and tree growth*/
void naturalchange(char grid[][COLUMN],int newfire[],int* lengthnew){
  int i ,j;
  for(i = 0; i < ROW; i++){
    for(j = 0;j < COLUMN; j++){
      /*this if deal with lightning*/
      if (grid[i][j] == '@'&&lightning(&grid[i][j],L)) {
        addfire(newfire,lengthnew,i,j);
      }
      /*this if deal with growth*/
      if (grid[i][j] == ' ') {
        growing(&grid[i][j],G);
      }
    }
  }
}
/*if lightning happens, change cell to * then return true*/
bool lightning(char* c,int chance){
  if (rand()%chance == 0) {
    *c = '*';
    return true;
  }
  return false;
}
/*if growth happens, change cell to @*/
void growing(char* c,int chance){
  if (rand()%chance == 0) {
    *c = '@';
  }
}