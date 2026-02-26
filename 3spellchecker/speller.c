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

  int index = 0;
  int c = EOF;

  // current word being looked at
  char currentWord[MAX_WORD_LENGTH + 1] = "";
  int currentWordLength = 0;

  while ((c = getchar()) && c != EOF) {
    if (isalpha(c)) {
      // if the character is an alpha character
      currentWord[currentWordLength] = tolower(c);
      currentWordLength += 1;
    } else {
      // a non-alpha character was reached so check the currently stored word
      if (currentWordLength > 0) {
        // indicate end of string
        currentWord[currentWordLength] = '\0';
        if (!wordIsInDictionary(currentWord, dictionary)) {
          numUnknownWords ++;
          printf("%s\n", currentWord);
        }
      }

      // no need to clear, the old word will be overwritten
      currentWordLength = 0;
    }
  }

  // step 3: print number of unknown words
  printf("%d\n", numUnknownWords);

  freeDictionary(dictionary);
  return EXIT_SUCCESS;
}
