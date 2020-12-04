#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define STRSIZE 128

typedef struct treenode{
  //if not a leaf node then c = 0
  char c;
  int weight;
  struct treenode *left;
  struct treenode *right;
  struct treenode *next;
}treenode;

/*priority queue*/
typedef struct queue{
  struct treenode* head;
  int size;
} queue;


void push(queue* q, treenode* t);
treenode* pull(queue* q);
void print_list(queue* q);
void freeall(queue* q);
void free_tree(treenode* t);
treenode* merge(treenode* t1, treenode* t2);
char *print_tree(treenode *t);
void test();
void get_code(treenode* t,char* str);

int main() {
  //test();
  int i ;
  treenode* t;
  char str[STRSIZE];
  queue* q = (queue*)malloc(sizeof(struct queue));
  for (i = 0; i < STRSIZE; i++) {
    str[i] = 0;
  }
  q -> head = NULL;
  q -> size = 0;
  srand(time(0));
  for (i = 0; i < 10; i++) {
    t = (treenode*)malloc(sizeof(struct treenode));
    t -> weight = rand()%10;
    t -> c = 'a'+ i;
    t -> next = NULL;
    t -> left = NULL;
    t -> right = NULL;
    push(q,t);
  }
  print_list(q);
  while(q->size > 1){
    t = merge(pull(q), pull(q));
    push(q,t);
  }
  t = pull(q);
  printf("root node :(char:%c  weight:%d)\n",t-> c,t->weight );
  printf("weight=%d child-left =%d child-right = %d\n", t -> weight,t->left->weight,t->right->weight);
  get_code(t,str);
  free_tree(t);
  free(q);
  return 0;
}
void test(){
  queue* q = (queue*)malloc(sizeof(struct queue));
  q -> head = NULL;
  q -> size = 0;
  treenode t1 = {'a',2,NULL,NULL,NULL};
  push(q , &t1);
  print_list(q);
  treenode t2 = {'b',2,NULL,NULL,NULL};
  push(q , &t2);
  print_list(q);
  treenode t3 = {'c',5,NULL,NULL,NULL};
  push(q , &t3);
  treenode t4 = {'d',4,NULL,NULL,NULL};
  push(q , &t4);
  print_list(q);
  treenode* t;
  t = pull(q);
  printf("(char:%c  weight:%d)\n",t-> c,t->weight );
  printf("size of q is %d\n",q -> size );
  print_list(q);
  t = merge(pull(q),pull(q));
  push(q,t);
  printf("(char:%c  weight:%d)\n",t-> c,t->weight );
  printf("left child(char:%c  weight:%d)\n",t->left-> c,t->left->weight );
  printf("right child(char:%c  weight:%d)\n",t->right-> c,t->right->weight );
  printf("size of q%d\n",q -> size );
  print_list(q);
  printf("============test over\n" );
}

void push(queue* q, treenode* t){
  treenode* pre = q -> head;
  treenode* temp = pre;
  q -> size ++;
  if (q -> head == NULL||q -> head -> weight > t ->weight) {
    t -> next = q -> head;
    q -> head = t;
    return;
  }
  while(temp != NULL&&t -> weight >= temp -> weight){
    pre = temp;
    temp = temp -> next;
  }
  t -> next = temp;
  pre -> next = t;
}

void print_list(queue* q){
  treenode* temp = q -> head;
  while(temp != NULL) {
    printf("(char:%c  weight:%d)\n",temp -> c,temp ->weight );
    printf("->");
    temp = temp -> next;
  }
  printf("NULL\n");
}

void freeall(queue* q){
  treenode* temp = q -> head;
  while(temp != NULL){
    temp = temp -> next;
    free(q -> head);
    q -> head = temp;
  }
  free(q);
}

treenode* pull(queue* q){
  treenode* temp = q -> head;
  q -> size --;
  if (q -> size< 0) {
    fprintf(stderr,"pull from empty queue\n" );
    exit(1);
  }
  q ->head = q ->head -> next;
  temp -> next = NULL;
  return temp;
}

treenode* merge(treenode* t1, treenode* t2){
  treenode* root  = (treenode*)calloc(sizeof(struct treenode),1);
  root -> c = 0;
  root -> weight = t1 -> weight + t2 -> weight;
  root -> next = NULL;
  if (t1 -> weight >= t2 -> weight) {
    root -> left = t1;
    root -> right = t2;
  }else{
    root -> left = t2;
    root -> right = t1;
  }
  // printf("left %d right %d\n", root->left->weight,root -> right->weight );
  // printf("parent %d\n", root -> weight);
  return root;
}

void free_tree(treenode* t){
  treenode* temp;
  queue* q ;
  if(t == NULL){
    return;
  }
  q = (queue*)malloc(sizeof(struct queue));
  q -> size = 0;
  q -> head = NULL;
  push(q,t);
  while(q -> size != 0){
      temp = pull(q);
      if (temp -> left != NULL) {
        push(q,temp -> left);
      }
      if (temp -> right != NULL) {
        push(q,temp -> right);
      }
      free(temp);
  }
  free(q);
}

void get_code(treenode* t,char* str){
  if (t -> c != 0) {
    printf("char: %c  code = %s weight = %d\n", t -> c, str, t -> weight);
    return;
  }
  if (t -> left != NULL) {
    str[strlen(str)] = '0';
    str[strlen(str)] = 0;
    get_code(t -> left, str);
    str[strlen(str) - 1] = 0;
  }
  if (t -> right != NULL) {
    str[strlen(str)] = '1';
    str[strlen(str)] = 0;
    get_code(t -> right, str);
    str[strlen(str) - 1] = 0;
  }
}
