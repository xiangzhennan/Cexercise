#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define SWAP(A,B) {char temp; temp = A; A = B; B = temp;}
#define LENDIC 370120

bool isword(char* dictionary[], char* instance);
void quick_sort(char* str[],int l,int r);
void getcombinations(char* str,int l, int r,char* dictionary[]);
void getdictionary(char* dictionary[], char* filename);

int main(void) {
  int i;
  char* dictionary[LENDIC];
  getdictionary(dictionary,"dictionary_sorted.txt");
  char str[] = "sternaig";
  getcombinations(str,0,strlen(str)-1,dictionary);
  for(i = 0;i < LENDIC; i++){
    free(dictionary[i]);
  }
  return 0;
}

void quick_sort(char* strs[],int l,int r){
  int i, j;
  char* temp;
  if (l >= r) {
    return;
  }
  temp = strs[l];
  i = l;
  j = r;
  while(i < j){
    while(i<j && strcmp(strs[j],temp)>=0){
      j--;
    }
    strs[i] = strs[j];
    while(i<j && strcmp(strs[i],temp)<=0){
      i++;
    }
    strs[j] = strs[i];
  }
  strs[i] = temp;
  quick_sort(strs,l,i-1);
  quick_sort(strs,i+1,r);
}

void getcombinations(char* str,int l, int r,char* dictionary[]){
  int i;
  if (l == r) {
    if (isword(dictionary,str)) {
      printf("%s\n",str);
    }
    return;
  }
  for(i = l; i <= r; i++){
    if (i == l||str[l]!=str[i]) {
      SWAP(str[l],str[i]);
      getcombinations(str,l+1,r,dictionary);
      SWAP(str[i],str[l]);
    }
  }
}

bool isword(char* dictionary[], char* instance){
  int i ,j,mid;
  i = 0;
  j = LENDIC;
  while(i <= j){
    mid = i + (j - i)/2;
    if (strcmp(dictionary[mid],instance)==0) {
      return true;
    }
    if (strcmp(dictionary[mid],instance)<0) {
      i = mid + 1;
    }else{
      j = mid - 1;
    }
  }
  return false;
}

void getdictionary(char* dictionary[], char* filename){
  int i;
  char str[40];
  i = 0;
  FILE* file = fopen(filename,"r");
  if (file == NULL) {
    fprintf(stderr,"file not found");
    exit(1);
  }
  while(fgets(str,40,file)!=NULL){
    str[strlen(str)-1] = 0;
    dictionary[i] = (char*)malloc(sizeof(char)*40);
    strcpy(dictionary[i],str);
    i++;
  }
  fclose(file);
}