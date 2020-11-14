#include<stdio.h>
#include<stdlib.h>


typedef struct listnode{
  int data;
  struct listnode* next;
} listnode;

void add(listnode* head, int index, int value);
listnode* createlist();
int getlength(listnode* head);
int getvalue(listnode* head, int index);
listnode* getnode(listnode* head, int index);
void changevalue(listnode* head, int index,int value);
void delete(listnode* head, int index);
void deleteall(listnode* head);
void printall(listnode* head);
void push(listnode* head, int value);
listnode* poll(listnode* head);