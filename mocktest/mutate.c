#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

#define N 20
#define SWAP(A,B) {int temp; temp = A; A = B; B = temp;}

void setorigin(grid[][N]);
void mutate(grid[][N]);
void mutatecenter(grid[][N]);

int main(int argc, char const *argv[]) {
  srand(time(0));
  int grid[N][N];
  setorigin(grid);
  for (size_t i = 0; i < N*N*N*N; i++) {
    //mutate(grid);
    mutatecenter(grid);
  }
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      printf("%d",grid[i][j] );
    }
    printf("\n");
  }
  return 0;
}

void setorigin(grid[][N]){
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      grid[i][j] = rand()%10;
    }
  }
}
void mutate(grid[][N]){
  int i = rand()%(N-1);
  int j = rand()%(N);
  if (grid[i][j]>grid[i+1][j]) {
    SWAP(grid[i][j],grid[i+1][j])
  }
  i = rand()%(N);
  j = rand()%(N-1);
  if (grid[i][j]>grid[i][j+1]) {
    SWAP(grid[i][j],grid[i][j+1])
  }
}
int getdistance(int i, int j){
  i = i - (N-1)/2;
  j = j - (N-1)/2;
  return i*i + j*j;
}

void mutatecenter(grid[][N]){
  int i1 = rand()% N;
  int j1 = rand()% N;
  int i2 = rand()% N;
  int j2 = rand()% N;
  if (getdistance(i1,j1)>getdistance(i2,j2)&&grid[i1][j1]<grid[i2][j2]) {
    SWAP(grid[i1][j1],grid[i2][j2]);
  }else if(getdistance(i1,j1)<getdistance(i2,j2)&&grid[i1][j1]>grid[i2][j2]){
    SWAP(grid[i1][j1],grid[i2][j2]);
  }
}
