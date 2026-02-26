// a SLOW spell checker

#include "dictionary.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// remove non-alphabetic characters and convert to lower case
void trimWord(char *word) {
  int k = 0;
  int length = strlen(word);
  for (int i = 0; i < length; i++) {
    if (isalpha(word[i])) {
      word[k] = tolower(word[i]);
      k++;
    }
  }
  word[k] = '\0';
}

int main(void) {
  char word[MAX_WORD_LENGTH + 1];

  // step 1: read in the dictionary
  Dictionary *dictionary = newEmptyDictionary();
  while (scanf("%45s", word) && word[0] != '!') {
    trimWord(word);
    addWord(word, dictionary);
  }

  // step 2: read in text
  int numUnknownWords = 0;

  // BUG: This loop is wrong. It will read "one,twwo" as one word "onetwwo".
  while (scanf("%45s", word) != EOF) {
    trimWord(word);
    if (!wordIsInDictionary(word, dictionary)) {
      numUnknownWords++;
      printf("%s\n", word);
    }
  }
  // TODO: Replace the above while loop with a correct solution.
  // Hints:
  // - you should read one character at a time, using getchar()
  // - alphabetical characters should be appended to the current word
  // - any other symbol should terminate the word
  // this code might be useful:
  /*
  int index = 0;
  int c = EOF;
  while ((c = getchar()) && c != EOF) {
    // ...
  }
  */

  // step 3: print number of unknown words
  printf("%d\n", numUnknownWords);

  freeDictionary(dictionary);
  return EXIT_SUCCESS;
}
