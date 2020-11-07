#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>

char* encoding(const char* s);
/*check if the current letter should be encoded*/
bool check(const char* s,int codes[],int i);

int main(int argc, char const *argv[]) {
  if( argc==2 ){
    printf("The soundex coding of %s is equal to %s\n",
    argv[1], encoding(argv[1]));
  }else{
    printf("ERROR: Incorrect usage, try e.g. %s Name\n", argv[0]);
  }
  return 0;
}

char* encoding(const char* s){
  int codes[26] = {-1,1,2,3,-1,1,2,-1,-1,2,2,4,5,5,-1,1,2,6,2,3,-1,1,-1,2,-1,2};
  static char code[5];
  int i = 0;//cur char in s
  int index = 0;
  code[index++] = toupper(s[0]);
  code[4] = 0;
  i++;
  while(s[i]!= '\0'&&index < 4) {
    if(codes[tolower(s[i])-'a'] != -1&&check(s,codes,i)) {
      code[index++] = codes[tolower(s[i])-'a']+'0';
    }
    i++;
  }
  /*implement with 0*/
  if (index < 4) {
    for (; index < 4; index++) {
      code[index] = '0';
    }
  }
  code[4] = 0;
  return code;
}

bool check(const char* s,int codes[],int i){
  if (codes[tolower(s[i])-'a']==codes[tolower(s[i-1])-'a']) {
    return false;
  }
  if (tolower(s[i-1])=='h'||tolower(s[i-1])=='w') {
    if (i - 2 >=0) {
      return codes[tolower(s[i])-'a']!=codes[tolower(s[i-2])-'a'];
    }
  }
  return true;
}