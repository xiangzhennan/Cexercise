#include<stdio.h>
#include<stdlib.h>
#include<time.h>

enum bet {odd_even, number_bet};
typedef enum bet bet;
enum guess {even , odd};
typedef enum guess guess;

int getmoney(int* money);
bet choosebet();
int getNumber();
void newround(bet curbet, int* money, int bet_thistime);
int oddevenbet(int guess, int number);
int numberbet(int guess, int number);
int getguess();
int changebet();
void test();

int main(int argc, char const *argv[]) {
  // test();
  int money = 0;
  srand(time(NULL));
  getmoney(&money);//user's total money
  printf("now you have %d money\n", money);
  bet curbet = choosebet();//choose which bet
  printf("you choose %u\n", curbet);
  while(money > 0){
    newround(curbet, &money, changebet());//now the user is in a round of bet
    printf("you now have %d money\n\n", money);
  }
  printf("you have lost all your money\n");
  return 0;
}

void test(){
  // int money = 10;
  // newround(1, &money);
}

int getmoney(int* money){
  printf("now choose your original money:\n");
  scanf("%d", money);
  return *money;
}

bet choosebet(){
  bet curbet;
  printf("which bet do you want(0 for odd_even or 1 for number):\n");
  scanf("%u", &curbet );
  return (bet)curbet;
}

int getNumber(){
  return rand()%35;
}

int oddevenbet(int guess , int number){
  if (number == 0) {
    return 0;
  }
  return number%2 == guess;
}

int numberbet(int guess, int number){
  return guess == number;
}

int getguess(){
  int guess;
  printf("input your guess(0 for even,1 for odd,0-35 for number_bet):\n");
  scanf("%d", &guess);
  return guess;
}
//user try to change the money he want to bet this round;
int changebet(){
  int bet_thistime;
  printf("how much do you want to bet this round?\n");
  scanf("%d",&bet_thistime);
  return bet_thistime;
}

void newround(bet curbet, int* money, int bet_thistime){
  int guess;
  int number = getNumber();
  guess = getguess();//get a guess from user
  printf("the correct number for this round is %d\n",number );
  switch (curbet) {
    case odd_even:{
      if (oddevenbet(guess, number)) {
        *money += 2 * bet_thistime;//that's temporory
      }else{
        *money -= bet_thistime;
      }
      break;
    }
    case number_bet:{
      if (numberbet(guess, number)) {
        *money += 35 * bet_thistime;//that's temporory
      }else{
        *money -= bet_thistime;
      }
      break;
    }
    default:{
      printf("no such bet available\n");
      exit(1);
    }
  }
}