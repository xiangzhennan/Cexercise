#include"list.h"

void push(listnode* head, int value){
  add(head, getlength(head), value);
}

listnode* poll(listnode* head){
  listnode* temp = getnode(head, 0);
  delete(head, 0);
  return temp;
}

listnode* createlist(){
  listnode* node = (listnode*)malloc(sizeof(struct listnode));
  node -> data = 0;
  node -> next = NULL;
  return node;
}

int getlength(listnode* head){
  if(head == NULL){
    return 0;
  }
  return head -> data;
}

listnode* getnode(listnode* head, int index){
  int i;
  for(i = index; i > 0; i--){
    head = head -> next;
  }
  return head -> next;
}

int getvalue(listnode* head, int index){
  listnode* temp = getnode(head, index);
  return temp -> data;
}

void changevalue(listnode* head, int index, int value){
  listnode* temp = getnode(head, index);
  temp -> data = value;
}

void add(listnode* head, int index, int value){
  head -> data ++;
  if (index != 0) {
    head = getnode(head, index - 1);
  }
  listnode* temp = head -> next;
  listnode* node = (listnode*)malloc(sizeof(struct listnode));
  node -> data = value;
  node -> next = temp;
  head -> next = node;
}

void delete(listnode* head, int index){
  head -> data --;
  if(index != 0){
    listnode* head = getnode(head, index - 1);
  }
  listnode* temp = head -> next;
  head -> next = temp -> next;
  free(temp);
}

void deleteall(listnode* head){
  listnode* temp = NULL;
  if (head == NULL) {
    return;
  }
  while(head != NULL){
    temp = head -> next;
    free(head);
    head = temp;
  }
}

void printall(listnode* head){
  int length = getlength(head);
  printf("length is %d\n", length);
  if (length == 0) {
    return;
  }
  head = head -> next;
  while(head != NULL){
    printf("%d ->", head -> data);
    head = head -> next;
  }
  printf("null\n");
}