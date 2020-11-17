#include"linkedlist.h"


linkedlist* createlist(){
  linkedlist* list = (linkedlist*)malloc(sizeof(struct linkedlist));
  list -> size = 0;
  list -> head = NULL;
  list -> end = NULL;
  return list;
}

int getlength(linkedlist* list){
  if(list == NULL){
    return 0;
  }
  return list -> size;
}
//use static cause i dont think the user would like to get the whole node
static listnode* getnode(linkedlist* list, int index){
  int i;
  listnode* temp = list -> head;
  if (index < 0 || index >= getlength(list)) {
    ERROR("index out of boundary");
  }
  //special treatment of end node to avoid going through the whole list
  if (index == getlength(list) - 1 ) {
    return list -> end;
  }
  for(i = 0; i < index; i++){
    temp = temp -> next;
  }
  return temp;
}

int getvalue(linkedlist* list, int index){
  return getnode(list, index) -> data;
}

void changevalue(linkedlist* list, int index, int value){
  listnode* temp = getnode(list, index);
  temp -> data = value;
}

void add(linkedlist* list, int index, int value){
  listnode* pre , *temp;
  listnode* new = (listnode*)malloc(sizeof(struct listnode));
  new -> data = value;
  list -> size ++;
  //special treatment of head node
  if (index == 0) {
    temp = list -> head;
    list -> head = new;
    //if list is empty before adding ,change the end pointer
    if (list -> end == NULL) {
      list -> end = new;
    }
    new -> next = temp;
  }else if(index == getlength(list)){
    //special treatment of end node to avoid going through the whole list
    list -> end = new;
    new -> next = NULL;
  }else{
    pre = getnode(list, index - 1);
    temp = pre -> next;
    new -> next = temp;
    pre -> next = new;
  }
}

void delete(linkedlist* list, int index){
  listnode* pre, *temp;
  if (getlength(list) == 0) {
    ERROR("cant delete from an empty list");
  }
  list -> size --;
  if (index == 0) {
    temp = list -> head;
    list -> head = list -> head -> next;
  }else{
    pre = getnode(list, index - 1);
    temp = pre -> next;
    pre -> next = temp -> next;
  }
  free(temp);
}

void freelist(linkedlist* list){
  listnode* temp = NULL;
  if (list -> head == NULL) {
    return;
  }
  while(list -> head != NULL){
    temp = list -> head -> next;
    free(list -> head);
    list -> head = temp;
  }
}

void printall(linkedlist* list){
  listnode* temp = list -> head;
  int length = getlength(list);
  printf("length is %d\n", length);
  if (length == 0) {
    return;
  }
  while(temp != NULL){
    printf("%d ->", temp -> data);
    temp = temp -> next;
  }
  printf("null\n");
}
