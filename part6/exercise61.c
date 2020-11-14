#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUMWORD 3000
#define ERROR(M){fprintf(stderr,"%s\n",M);}

typedef struct dictionary{
  char* words[NUMWORD];
  int length;
}dictionary;

void getdictionary(dictionary* dic,char* filename);
void insert(dictionary* dic,char* str);

int main(int argc, char const *argv[]) {
  int i ;
  dictionary dic;
  dic.length = 0;
  for (i = 0; i < NUMWORD; i++) {
    dic.words[i] = (char*)malloc(sizeof(char)*5);
  }
  getdictionary(&dic,"34words.txt");
  for (i = 0; i < dic.length; i++) {
    printf("%s\n",dic.words[i] );
  }
  for (i = 0; i < NUMWORD; i++) {
    free(dic.words[i]);
  }
  return 0;
}

void getdictionary(dictionary* dic,char* filename){
  int len, index;
  //int i,j;
  char str[6] ;
  index = 0;
  FILE* file = fopen(filename,"r");
  if (file == NULL) {
    ERROR("file not found");
    exit(1);
  }
  while (fgets(str,6,file)!=NULL) {
    len = strlen(str);
    str[len - 1] = 0;
    insert(dic,str);
    // for (i = 0; i < dic->length; i++) {
    //   printf("%s.",dic->words[i] );
    // }
    // printf("\n" );
  }
}

void insert(dictionary* dic,char* str){
  int i = 0,j;
  char* temp;
  while(i < dic->length&&strcmp(str,dic->words[i])>0){
    i++;
  }
  for (j = dic ->length; j > i; j--) {
    memcpy(dic -> words[j],dic -> words[j-1],5);
    //dic -> words[j] = dic -> words[j-1];
    //this line showed above is wrong, cause it change the address of pointer rather than the content
  }
  strcpy(dic -> words[i], str);
  dic -> length ++;
}