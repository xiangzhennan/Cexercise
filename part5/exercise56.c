#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define SWAP(A,B) {char temp; temp = A; A = B; B = temp;}
#define LENDIC 2891

bool isword(char* dictionary[], char* instance);
void getdictionary(char* dictionary[], char* filename);
void findladder(char str[],char target[],int len,char* dictionary[],char* result);
int edit_distance(char *s, char *t);

int main(int argc, char *argv[]) {
  int i;
  char* dictionary[LENDIC];
  char* result = (char*)malloc(sizeof(char)*26);
  getdictionary(dictionary,"small_sort.txt");
  char str[] = "cold";
  char str2[] = "warm";
  strcat(result,str);
  findladder(str,str2,edit_distance(str,str2),dictionary,result);
  for(i = 0;i< LENDIC;i++){
    free(dictionary[i]);
  }
  free(result);
  return 0;
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

int edit_distance(char *s, char *t){
  int i,len,cnt;
  cnt = 0;
  len = strlen(s);
  for (i = 0; i < len; i++) {
    cnt += (s[i]==t[i]?0:1);
  }
  return cnt;
}

void findladder(char str[],char target[],int len,char* dictionary[],char* result){
  int i;
  int j;//to store length of result before change
  char temp;
  printf("len is %d str is %s\n", len, str);
  printf("%s\n",result );
  if (len == 0) {
    // for(i = 0; i< 26;i++){
    //   printf("%c",result[i] );
    // }
    // printf("\n\n");
    return;
  }
  for(i =0;i<4;i++){
    if (str[i] != target[i]) {
      temp = str[i];
      str[i] = target[i];
      if(isword(dictionary,str)){
        j = strlen(result);
        strcat(result,"\n");//just want to make output more clearly
        strcat(result,str);//add current str to result
        findladder(str,target,len-1,dictionary,result);
        result[j] = 0;
      }
      str[i] = temp;
    }
  }
}

void getdictionary(char* dictionary[], char* filename){
  int i;
  char str[6];
  i = 0;
  FILE* file = fopen(filename,"r");
  if (file == NULL) {
    fprintf(stderr,"file not found");
    exit(1);
  }
  while(fgets(str,6,file)!=NULL){
    str[strlen(str)-1] = 0;
    dictionary[i] = (char*)malloc(sizeof(char)*5);
    strcpy(dictionary[i],str);
    i++;
  }
  fclose(file);
}