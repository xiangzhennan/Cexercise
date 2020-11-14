#include "common.h"
#include "list.h"

int tonum(char* s);
void getfactor(int num, listnode* head);
bool isprime(int a);

int main(int argc, char const *argv[]) {
  listnode* head = createlist();
  if (argc != 2) {
    ERROR("invalid input");
    exit(1);
  }
  getfactor(tonum(argv[1]),head);
  printall(head);
  deleteall(head);
  return 0;
}

int tonum(char* s){
  int len, i;
  int sum = 0;
  len = strlen(s);
  for (i = 0; i < len; i++) {
    sum = sum*10+ s[i] - '0';
  }
  return sum;
}

void getfactor(int num, listnode* head){
  int i;
  if (num < 2) {
    if (getlength(head) == 0) {
      push(head,num);
    }
    return;
  }
  for (i = 2; i <= num; i++) {
    if (isprime(i)&&num%i == 0) {
      push(head,i);
      num /= i;
      getfactor(num,head);
      return;
    }
  }
}

bool isprime(int a) {
	int i;
	if (a <= 1)return false;
	if (a == 2)return true;
	for (i = 2; i < a; i++){
		if (a % i == 0)return false;
	}
	return true;
}