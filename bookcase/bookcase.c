#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>

#define MAXROW 9
#define MAXCOLUMN 9
#define INITSIZE 10
#define RESIZEFACTOR 2
#define MAXCAPACITY 30000000
#define NUMALPHA 26
#define MAXSTEP 100
#define OFFSET 'A'
#define ERROR(M) {fprintf(stderr, "%s\n", M); exit(EXIT_FAILURE);}
#define SWAP(A,B) {char temp; temp = A; A = B; B = temp;}

typedef struct listnode{
  int parent;
  char grid[MAXROW][MAXCOLUMN];
}listnode;

typedef struct arraylist{
  struct listnode* bookcases;
  int size;
  int capacity;
}arraylist;

typedef enum bool{false, true} bool;

void test();
arraylist* create_array();
void free_array(arraylist* list);
void list_add(arraylist* list,char bookcase[][MAXCOLUMN],int parent);
int find_solution(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column);
int get_children(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column,int parent);
bool switch_line(char grid[][MAXCOLUMN],int column,int from,int to);
void readfile(char* filename,char grid[][MAXCOLUMN],int *row,int *column);
void setzero(char grid[][MAXCOLUMN]);
void print_route(arraylist* list,int result_index,int row,int column);
int get_route_length(arraylist* list,int result_index);
bool ishappy(arraylist* list,int index, int row,int column);
bool canhappy(char bookcase[][MAXCOLUMN],int row, int column);
bool check_grid(char bookcase[][MAXCOLUMN],int row, int column);

int main(int argc, char *argv[]) {
  int result_index;
  int row ,column;
  arraylist* list;
  char bookcase[MAXROW][MAXCOLUMN];
  test();
  list = create_array();
  if (argc < 2) {
    ERROR("please enter enough arguments");
  }
  setzero(bookcase);
  readfile(argv[1],bookcase,&row,&column);
  if(!check_grid(bookcase,row,column)){
    ERROR("invalid input file");
  }
  /*if cant be happy or fail to find solution*/
  if (!canhappy(bookcase,row,column)||
  (result_index = find_solution(list,bookcase,row,column))== -1) {
    printf("%s\n","no solution" );
    free_array(list);
    return -1;
  }
  printf("%d\n\n", get_route_length(list,result_index));
  if (argc == 3 && strcmp(argv[2],"verbose") == 0) {
    print_route(list,result_index,row,column);
  }
  free_array(list);
  return 0;
}

void test(){
  int i;
  /*int j,k;*/
  arraylist* list;
  char bookcase[MAXROW][MAXCOLUMN] = {
    {'R','G','.'},
    {'G','R','.'},
    {'Y','B','.'},
    {'.','.','.'}
  };
  char happy_bookcase[MAXROW][MAXCOLUMN] = {
    {'R','R','.'},
    {'G','G','.'},
    {'.','.','.'},
    {'.','.','.'}
  };
  assert(check_grid(bookcase,4,3));
  assert(!check_grid(bookcase,5,3));
  assert(!check_grid(bookcase,4,4));
  bookcase[1][1] = 0;
  assert(!check_grid(bookcase,4,3));
  bookcase[1][1] = 'R';
  assert(canhappy(bookcase,4,3));
  bookcase[1][1] = 'G';
  assert(canhappy(bookcase,4,3));
  bookcase[1][2] = 'G';
  /*4 'G' exists*/
  assert(!canhappy(bookcase,4,3));
  bookcase[1][2] = '.';
  bookcase[3][0] = 'A';
  /*5 color exists*/
  assert(!canhappy(bookcase,4,3));
  /*reset bookcase*/
  bookcase[1][1] = 'R';
  bookcase[3][0] = '.';

  list = create_array();
  assert(list != NULL);
  assert(list -> size == 0);
  assert(list -> capacity == INITSIZE);

  list_add(list, bookcase, 1);
  assert(list -> bookcases[0].grid[0][0] == 'R');
  assert(list -> size == 1);
  assert(list -> capacity == INITSIZE);
  /*test resize can be done correctly*/
  for (i = 0; i < 1000; i++) {
    list_add(list,bookcase,1);
  }
  assert(list -> size == 1001);
  assert(list -> capacity == 1280);
  /*test switch line*/
  assert(!switch_line(bookcase,3,3,0));
  assert(switch_line(bookcase,3,0,1));
  assert(bookcase[1][2] == 'G');
  assert(bookcase[0][1] == '.');
  /*reset*/
  assert(switch_line(bookcase,3,1,0));
  bookcase[2][0] = '.';
  bookcase[2][1] = '.';
  /*curent bookcase looks like below, and 6 children expected
  {
    {'R','G','.'},
    {'G','R','.'},
    {'.','.','.'},
    {'.','.','.'}
  }*/
  free_array(list);
  assert(canhappy(bookcase,4,3));
  bookcase[0][1] = 'R';
  assert(!canhappy(bookcase,2,2));
  bookcase[0][1] = 'G';
  list = create_array();
  assert(get_children(list,bookcase,4,3,0)==6);
  assert(list -> size == 6);
  /*test ishappy*/
  assert(!ishappy(list,0,4,3));
  assert(!ishappy(list,1,4,3));
  assert(!ishappy(list,2,4,3));
  assert(!ishappy(list,3,4,3));
  assert(!ishappy(list,4,4,3));
  assert(!ishappy(list,5,4,3));
  /*get a happybookcase manually*/
  SWAP(bookcase[0][1],bookcase[1][1]);
  list_add(list, bookcase, -1);
  assert(ishappy(list,6,4,3));
  assert(get_route_length(list,6) == 1);
  free_array(list);

  list = create_array();
  SWAP(bookcase[1][1],bookcase[0][1]);
  i = find_solution(list, bookcase,4,3);
  assert(get_route_length(list,i) == 4);
  free_array(list);
  list = create_array();
  i = find_solution(list, happy_bookcase,4,3);
  assert(i == 0);
  assert(get_route_length(list,i) == 1);
  free_array(list);
}

arraylist* create_array(){
  arraylist* list = (arraylist*)malloc(sizeof(struct arraylist));
  if (list == NULL) {
    ERROR("malloc failed");
  }
  list -> bookcases = (listnode*)malloc(sizeof(struct listnode)*INITSIZE);
  list -> size = 0;
  list -> capacity = INITSIZE;
  return list;
}

void list_add(arraylist* list,char bookcase[][MAXCOLUMN],int parent){
  int index;
  index = list -> size;
  /*check memory is enough*/
  list -> size++;
  if (list -> size >= list -> capacity) {
    if (list -> capacity > MAXCAPACITY) {
      free_array(list);
      ERROR("no solution found, out of memory");
    }
    list -> bookcases = (listnode*)realloc(list -> bookcases,
      sizeof(struct listnode) * list -> capacity * RESIZEFACTOR);
    if (list -> bookcases == NULL) {
      ERROR("realloc failed");
    }
    list -> capacity *= RESIZEFACTOR;
  }
  /*create new listnode , add it to list*/
  list -> bookcases[index].parent = parent;
  memcpy(list -> bookcases[index].grid,bookcase,MAXROW*MAXCOLUMN);
}

void free_array(arraylist* list){
  free(list -> bookcases);
  free(list);
}

int find_solution(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column){
  int i,parent,count;
  char temp[MAXROW][MAXCOLUMN];
  /*parent index -1 to mark current bookcase is the origin one*/
  list_add(list, bookcase, -1);
  if (ishappy(list,0,row,column)) {
    return 0;
  }
  i = 0;
  while(i < list -> size){
    parent = i;
    /*in case realloc() change the address of current bookcase,
    do a memcpy to secure the current bookcase grid*/
    memcpy(temp,list -> bookcases[i].grid,MAXROW*MAXCOLUMN);
    count = get_children(list, temp, row, column, parent);
    /*if find a happy bookcase*/
    if (count < 0) {
      return list -> size -1;
    }
    i ++;
  }
  /*if i == list size, means cant generate more children bookcases, so reports no solution*/
  if (i == list -> size) {
    return -1;
  }
  return i;
}
/*based on a bookcase, find all possible move to reduce doc*/
int get_children(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column, int parent){
  int count, i, j;
  count = 0;
  /*go through all combination of lines to switch*/
  for (i = 0; i < row; i++) {
    for (j = 0 ; j < row; j++) {
      if (i!=j&&switch_line(bookcase,column,i,j)) {
        list_add(list,bookcase,parent);
        count ++;
        /*end loop if find a happy bookcase*/
        if (ishappy(list, list -> size -1,row,column)) {
          return -1;
        }
        /*switch back*/
        switch_line(bookcase,column,j,i);
      }
    }
  }
  return count;
}
/*move book from shelf "from", to shelf "to"*/
bool switch_line(char grid[][MAXCOLUMN],int column,int from,int to){
  int i, j ;
  /*check "from" shelf is not empty and "to" shelf has at least one empty place*/
  if (grid[from][0] == '.'||grid[to][column-1] != '.') {
    return false;
  }
  i = column -1;
  j = 0;
  while(grid[from][i] == '.'){
    i --;
  }
  while(grid[to][j] != '.'){
    j ++;
  }
  SWAP(grid[from][i],grid[to][j]);
  return true;
}

void readfile(char* filename,char grid[][MAXCOLUMN],int *row,int *column){
  int r,c;
  /*give two more space for '\n' and '\0'*/
  char str[MAXCOLUMN+2];
  FILE* file;
  file  = fopen(filename,"r");
  if (file == NULL) {
    ERROR("file not found");
  }
  /*read file head*/
  if(fgets(str,MAXCOLUMN+2,file)!= NULL){
    if(sscanf(str,"%d%d",row,column)!=2||*row > 9||*column > 9){
      ERROR("invalid file format");
    }
  }
  /*read bookcase*/
  r = 0;
  while (fgets(str,MAXCOLUMN+2,file)!=NULL) {
    c = 0;
    while(c < *column){
      grid[r][c] = str[c];
      c++;
    }
    r ++;
  }
  fclose(file);
}
/*initialise array to be filled with 0*/
void setzero(char grid[][MAXCOLUMN]){
  int r ,c;
  for (r = 0; r < MAXROW; r++) {
    for (c = 0; c < MAXCOLUMN; c++) {
      grid[r][c] = 0;
    }
  }
}
/*print result*/
void print_route(arraylist* list,int result_index,int row,int column){
  int steps[MAXSTEP];
  int index, temp, r, c;
  int print_order;
  print_order = 1;
  index = 0;
  temp = result_index;
  steps[index++] = result_index;
  /*records evrey index in the route*/
  while((temp = list -> bookcases[temp].parent)!=-1){
    steps[index++] = temp;
  }
  /*print each bookcase in the route reversely*/
  while(index > 0){
    printf("step %d pattern:\n", print_order++);
    for (r = 0; r < row; r++) {
      for (c = 0; c < column; c++) {
        printf("%c", list -> bookcases[steps[index - 1]].grid[r][c] );
      }
      printf("\n");
    }
    index --;
  }
}

int get_route_length(arraylist* list,int result_index){
  int length,temp;
  temp = result_index;
  length = 1;
  while((temp = list -> bookcases[temp].parent)!=-1){
    length++;
  }
  return length;
}

bool ishappy(arraylist* list,int index, int row,int column){
  int r,c,i;
  listnode* cur;
  char temp;
  cur = &list -> bookcases[index];
  for (r = 0; r < row; r++) {
    temp = cur -> grid[r][0];
    if (temp != '.') {
      for (c = 1; c < column; c++) {
        /*if find an valid element which is different from the first one in a row, return false*/
        if (cur -> grid[r][c]!= temp&&cur -> grid[r][c]!= '.') {
          return false;
        }
      }
    }
  }
  /*if find two shelves hold the same color,return false;*/
  for (r = 0; r < row; r++) {
    for (i = 0; i < r; i++) {
      if (cur->grid[r][0] != '.'&&cur -> grid[r][0]==cur ->grid[i][0]) {
        return false;
      }
    }
  }
  return true;
}
/*check if there is a color of book that cant be held in one shelf,
or number of color is bigger than number of row*/
bool canhappy(char bookcase[][MAXCOLUMN],int row, int column){
  int r,c,i;
  int count_color;
  int count_alpha[NUMALPHA];
  /*initialise*/
  for (i = 0; i < NUMALPHA; i++) {
    count_alpha[i] = 0;
  }
  for (r = 0; r < row; r++) {
    for (c = 0; c < column; c++) {
      if (bookcase[r][c] != '.') {
        count_alpha[toupper(bookcase[r][c]) - OFFSET]++;
      }
    }
  }
  count_color = 0;
  for (i = 0; i < NUMALPHA; i++) {
    if (count_alpha[i]) {
      count_color ++;
      /*number of book of same color should be smaller than column*/
      if (count_alpha[i] > column) {
        return false;
      }
    }
  }
  /*number of color should be smaller than row*/
  if (count_color > row) {
    return false;
  }
  return true;
}
/*check bookcase doesnt have invalid char thus has right row and column*/
bool check_grid(char bookcase[][MAXCOLUMN],int row, int column){
  int r,c;
  for (r = 0; r < row; r++) {
    for (c = 0; c < column; c++) {
      if (bookcase[r][c] != '.'&&!isalpha(bookcase[r][c])) {
        return false;
      }
    }
  }
  return true;
}
