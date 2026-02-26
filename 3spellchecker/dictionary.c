// implements a dictionary

#include "dictionary.h"
#include "memory.h"

#include <stdlib.h>
#include <string.h>

Dictionary *newEmptyDictionary() {
  Dictionary *d = safeMalloc(sizeof(Dictionary));
  d->wordsSize = 0;
  d->wordsCapacity = 1;
  d->words = safeMalloc(d->wordsCapacity * sizeof(char*));
  return d;
}

// add word to the dictionary if it is is not already known
void addWord(char word[MAX_WORD_LENGTH + 1], Dictionary *d) {
  if (!wordIsInDictionary(word, d)) {
    // if we need more space before adding the word, double the size
    if (d->wordsSize == d->wordsCapacity) {
      d->wordsCapacity *= 2;
      d->words = safeRealloc(d->words, d->wordsCapacity * sizeof(char*));
    }

    // now we actually add the word
    d->words[d->wordsSize] = safeMalloc((MAX_WORD_LENGTH + 1) * sizeof(char));
    strcpy(d->words[d->wordsSize], word);
    d->wordsSize++;
  }
}

// check whether word is in dictionary
bool wordIsInDictionary(const char *word, Dictionary *d) {
  for (int i = 0; i < d->wordsSize; i++) {
    if (strcmp(d->words[i], word) == 0) {
      return true;
    }
  }
  return false;
}

void freeDictionary(Dictionary *d) {
  for (int i = 0; i < d->wordsSize; i++) {
    free(d->words[i]);
  }
  free(d->words);
  free(d);
}
