// declares a dictionary

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
#define MAX_WORD_LENGTH 45
#define ALPHABET_N 26

// a dictionary is an array
typedef struct Dictionary {
  bool is_word;
  struct Dictionary *children[ALPHABET_N];
} Dictionary;

Dictionary *newEmptyDictionary();
void addWord(char word[MAX_WORD_LENGTH + 1], Dictionary *d);
bool wordIsInDictionary(const char *word, Dictionary *d);
void freeDictionary(Dictionary *d);

#endif // DICTIONARY_H
