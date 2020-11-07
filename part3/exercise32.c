#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void randomize(int songs[],int total);
void swap(int a[],int i ,int j);
void reorder(int a[], int size);

int main(int argc, char const *argv[]) {
  int total;
  printf("%s\n", "how many songs required?");
  scanf("%d", &total);
  int songs[total];
  for (int i = 0; i < total; i++) {
    songs[i] = i+1;
  }
  randomize(songs,total);
  reorder(songs,total);
  return 0;
}

void randomize(int a[], int total){
  srand(time(0));
  for (int i = 0; i < total; i++) {
    swap(a, i ,rand()%total);
  }
  for (int i = 0; i < total; i++) {
    printf("%4d",a[i] );
  }
  printf("\n");
}

void swap(int a[], int i ,int j){
  int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

void reorder(int a[], int size){
  for (int i = 0; i < size; i++) {
    while(a[i] != i+1){
      swap(a, i , a[i]-1);
    }
  }
  for (int i = 0; i < size; i++) {
    printf("%4d",a[i] );
  }
  printf("\n");
}