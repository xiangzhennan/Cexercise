#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUMWORD 3000
#define ERROR(M){fprintf(stderr,"%s\n",M);}

typedef struct node{
  char word[5];
  struct node* next;
}node;

typedef struct dictionary{
  node* head;
  int length;
}dictionary;

void getdictionary(dictionary* dic,char* filename);
void insert(dictionary* dic,char* str);

int main(int argc, char const *argv[]) {
  int i = 0;
  dictionary dic;
  dic.length = 0;
  dic.head = NULL;

  getdictionary(&dic,"34words.txt");
  printf("length is %d\n",dic.length );
  node* temp = dic.head;
  while(temp!=NULL){
    printf("%s\n",temp->word );
    temp = temp -> next;
  }
  while(dic.head != NULL){
    temp = dic.head -> next;
    free(dic.head);
    dic.head = temp;
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
  }
}

void insert(dictionary* dic,char* str){
  node* temp = dic->head;
  node* pre = temp;//the previous node
  node* new;
  //find the place to insert
  while(temp!=NULL&&strcmp(str,temp->word)>0){
    pre = temp;
    temp = temp->next;
  }
  //if it is head
  if (pre == temp) {
    new = (node*)malloc(sizeof(struct node));
    strcpy(new->word, str);
    new -> next = (pre == NULL? NULL:pre);
    dic -> head = new;
  }else{
    new = (node*)malloc(sizeof(struct node));
    strcpy(new->word, str);
    new -> next = temp;
    pre -> next = new;
  }
  dic -> length ++;
}