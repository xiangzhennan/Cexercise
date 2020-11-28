#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

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
  /*doc means degree of chaos, a happy bookcase has 0 doc*/
  int doc;
}listnode;
/*to store bookcases*/
typedef struct arraylist{
  struct listnode* bookcases;
  int size;
  int capacity;
}arraylist;

typedef enum bool{false, true} bool;


void test();
int get_doc(char bookcase[][MAXCOLUMN],int row, int column);
arraylist* create_array();
void free_array(arraylist* list);
void list_add(arraylist* list,char bookcase[][MAXCOLUMN],int parent,int doc);
int find_solution(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column);
int get_children(arraylist* list, int index,int row, int column,int* min_doc);
bool switch_line(char grid[][MAXCOLUMN],int column,int from,int to);
void readfile(char* filename,char grid[][MAXCOLUMN],int *row,int *column);
void setzero(char grid[][MAXCOLUMN]);
void print_route(arraylist* list,int result_index,int row,int column);
int get_route_length(arraylist* list,int result_index);
bool ishappy(arraylist* list,int index);
bool canhappy(char bookcase[][MAXCOLUMN],int row, int column);
int get_nexti(arraylist* list,int i,int min_doc);
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
/*only test new functions*/
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
  assert(get_doc(bookcase,4,3) == 3);
  bookcase[1][2] = 'G';
  assert(get_doc(bookcase,4,3) == 4);
  bookcase[2][0] = 'G';
  /*{'R','G','.'},
  {'G','R','G'},
  {'G','B','.'},
  {'.','.','.'}*/
  assert(get_doc(bookcase,4,3) == 5);
  bookcase[1][2] = '.';

  list = create_array();
  list_add(list, bookcase, 1, 0);
  assert(list -> bookcases[0].grid[0][0] == 'R');
  assert(list -> size == 1);
  assert(list -> capacity == INITSIZE);
  assert(list -> bookcases[0].doc == 0);
  assert(list -> bookcases[0].parent == 1);

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

  list = create_array();
  list_add(list,bookcase,-1,get_doc(bookcase,4,3));
  i = 100;
  /*test doc_min can be updated*/
  assert(get_children(list,0,4,3,&i)==6);
  assert(i == 2);
  assert(list -> size == 7);
  SWAP(bookcase[0][1],bookcase[1][1]);
  /*get a happybookcase manually*/
  list_add(list, bookcase, 0, 0);
  assert(ishappy(list,7));
  free_array(list);
  /*test get_nexti*/
  list = create_array();
  list_add(list,bookcase,-1,5);
  list_add(list,bookcase,-1,4);
  list_add(list,bookcase,-1,4);
  list_add(list,bookcase,-1,3);
  list_add(list,bookcase,-1,4);
  list_add(list,bookcase,-1,4);
  list_add(list,bookcase,-1,0);
  assert(get_nexti(list,-1,5) == 0);
  assert(get_nexti(list,-1,3) == 3);
  assert(get_nexti(list,-1,1) == 6);
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

/*doc means degree of chaos, set the first book in a shelf as a base color,
if it follows a book which has a diffrent color,then add the number of remaining books to doc,
e.g. doc of shelf RGY is 2, RGR is also 2, because R is followed by a G,
if find a second shelf has the same base color as a former shelf,
then add the whole number of books in this shelf to doc,
e.g. {{RGR},{RRR}} has 5 doc instead of 2*/
int get_doc(char bookcase[][MAXCOLUMN],int row, int column){
  int i,r,c,doc;
  bool flag_samebase = false;
  doc = 0;
  for (r = 0; r < row; r++) {
    /*if this shelf has same base color as former ones ,set flag*/
    for(i = 0; i < r; i++){
      if (bookcase[r][0] == bookcase[i][0]&&bookcase[i][0] != '.') {
        flag_samebase = true;
      }
    }
    if (flag_samebase) {
      c = 0;
      while(c < column&&bookcase[r][c]!='.'){
        doc ++;
        c++;
      }
    }else{
      /*deal with book in the same shelf which has diffrent coloc other than the base color*/
      if (column > 1) {
        c = 1;
        while (bookcase[r][c] == bookcase[r][0]) {
          c++;
        }
        while(c<column&&bookcase[r][c]!='.'){
          doc ++;
          c++;
        }
      }
    }
    flag_samebase = false;
  }
  return doc;
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

void list_add(arraylist* list,char bookcase[][MAXCOLUMN],int parent,int doc){
  int index;
  index = list -> size;
  /*check memory is enough*/
  list -> size++;
  if (list -> size >= list -> capacity) {
    if (list -> capacity > MAXCAPACITY) {
      free_array(list);
      ERROR("no solution, out of memory");
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
  list -> bookcases[index].doc = doc;
}

void free_array(arraylist* list){
  free(list -> bookcases);
  free(list);
}

int find_solution(arraylist* list, char bookcase[][MAXCOLUMN],int row, int column){
  int i,min_doc;
  min_doc = get_doc(bookcase,row,column);
  /*add the origin one*/
  list_add(list, bookcase, -1, min_doc);
  i = 0;
  /*get one bookcase from the list*/
  while(i < list -> size && !ishappy(list, i)){
    get_children(list, i, row, column,&min_doc);
    i = get_nexti(list, i, min_doc);
  }
  /*if there is no more element available, reports no solution*/
  if (i >= list -> size) {
    return -1;
  }
  return i;
}
/*based on a bookcase, find all possible move to reduce doc*/
int get_children(arraylist* list, int index,int row, int column,int* min_doc){
  int count, i, j, cur_doc,temp_doc;
  char temp[MAXROW][MAXCOLUMN];
  memcpy(temp,list -> bookcases[index].grid,MAXROW*MAXCOLUMN);
  cur_doc = list -> bookcases[index].doc;
  count = 0;
  /*go through all combination of lines to switch*/
  for (i = 0; i < row; i++) {
    for (j = 0; j < row; j++) {
      if (i!=j&&switch_line(temp,column,i,j)) {
        /*avoid add child that has a bigger doc than its parent,
        which is meaningless*/
        if ((temp_doc = get_doc(temp,row,column))<=cur_doc) {
          /*keep updating the minimum doc*/
          *min_doc = temp_doc;
          list_add(list,temp,index,temp_doc);
          count ++;
          if (min_doc == 0) {
            return count;
          }
        }
        /*switch back*/
        switch_line(temp,column,j,i);
      }
    }
  }
  return count;
}
/*find an index of bookcase that the bookcase has the current minimun doc*/
int get_nexti(arraylist* list,int i,int min_doc){
  i = i + 1;
  while(i < list -> size && list -> bookcases[i].doc > min_doc){
    i++;
  }
  return i;
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

int get_route_length(arraylist* list,int result_index){
  int length,temp;
  temp = result_index;
  length = 1;
  while((temp = list -> bookcases[temp].parent)!=-1){
    length++;
  }
  return length;
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
    printf("current doc is %d\n", list -> bookcases[steps[index - 1]].doc);
    index --;
  }
}

bool ishappy(arraylist* list,int index){
  return list -> bookcases[index].doc  == 0;
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
