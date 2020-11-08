#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

void test();
int anagram(char s1[], char s2[]);
int derange(char s1[], char s2[]);

int main(int argc, char const *argv[]) {
  test();

  return 0;
}

void test(){
  assert(anagram("elvis", "lives") == 1);
  assert(anagram("dreads", "sadder") == 1);
  assert(anagram("replays", "parsley") == 1);
  assert(anagram("listen", "silent") == 1);
  assert(anagram("orchestra", "carthorse") == 1);
  /* Two identical words are not anagrams */
  assert(anagram("elvis", "elvis") == 0);
  assert(anagram("neill", "neil") == 0);
  assert(anagram("neil", "neill") == 0);
  assert(anagram("horse", "short") == 0);
  assert(derange("elvis", "lives") == 0);
  assert(derange("dreads", "sadder") == 1);
  assert(derange("replays", "parsley") == 1);
  assert(derange("listen", "silent") == 0);
  assert(derange("orchestra", "carthorse") == 1);

  printf("test passed\n");
}
int anagram(char s1[], char s2[]){
  if (strcmp(s1,s2) == 0) {
    return 0;
  }
  if (strlen(s1)!=strlen(s2)) {
    return 0;
  }
  int count1[26];
  int count2[26];
  for (size_t i = 0; i < 26; i++) {
    count1[i] = 0;
    count2[i] = 0;
  }
  for (size_t i = 0; i < strlen(s1); i++) {
    count1[s1[i]-'a']++;
    count2[s2[i]-'a']++;
  }
  for (size_t i = 0; i < 26; i++) {
    if (count1[i]!= count2[i]) {
      return 0;
    }
  }
  return 1;
}
int derange(char s1[], char s2[]){
  if (!anagram(s1,s2)) {
    return 0;
  }
  for (size_t i = 0; i < strlen(s1); i++) {
    if (s1[i] == s2[i]) {
      return 0;
    }
  }
  return 1;
}
