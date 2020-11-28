#include"queue.h"

linkedlist* createqueue(){
  return createlist();
}

void push(linkedlist* q, int value){
  add(q, getlength(q), value);
}

int poll(linkedlist* q){
  int re = getvalue(q,0);
  delete(q,0);
  return re;
}

bool isempty(linkedlist* q){
  return getlength(q) == 0;
}

void freequeue(linkedlist* q){
  freelist(q);
}
