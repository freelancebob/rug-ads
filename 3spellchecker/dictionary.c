// implements a dictionary

#include "dictionary.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Dictionary *newEmptyDictionary() {
  Dictionary *d = safeMalloc(sizeof(Dictionary));
  d->is_word = 0;

  // ensures that d.children is filled with null/zeroes
  // this being unitialized was a problem pointed out by valgrind which is fixed by this
  memset(d->children, 0, sizeof(d->children));
  
  return d;
}

// private function that actually adds the word to the dictionary
// recursively adds each letter to the current dictionary node, creating a new node if it doesnt exist yet
void addWordRecursive(char word[MAX_WORD_LENGTH + 1], Dictionary *d) {
  if (word[0] == '\0') {
    d->is_word = 1;
    return;
  }

  // index is 0 for a, 26 for z
  int index = word[0] - 'a';

  // if no node for this letter exists then create one
  if (d->children[index] == NULL) {
    d->children[index] = newEmptyDictionary();
  }

  // removes the first character from "word" by shifting everything from 1:25 to 0:24
  for (int i = 1; i < MAX_WORD_LENGTH + 1; i ++) {
    word[i - 1] = word[i];
  }

  addWordRecursive(word, d->children[index]);
}

// add word to the dictionary if it is is not already known
// this calls the actual recursive function so that the "wordIsInDictionary" check is only done once
void addWord(char word[MAX_WORD_LENGTH + 1], Dictionary *d) {
  if (!wordIsInDictionary(word, d)) {
    addWordRecursive(word, d);
  }
}

// check whether word is in dictionary
bool wordIsInDictionary(const char *word, Dictionary *d) {  
  int wordIndex = 0;
  int index = word[wordIndex] - 'a';

  //Checks if the order of the letters of the word is in the dictionary
  while (d->children[index] != NULL) {
    d = d->children[index];

    //Goes to the next letter in the word
    wordIndex += 1;

    //Checks if the word has ended
    if (word[wordIndex] == '\0') {
      break;
    }

    //Puts the alphabet value of the next letter in the index
    index = word[wordIndex] - 'a';
  }

  //Checks if the last node is a leaf node and the word has ended
  if (d->is_word && word[wordIndex] == '\0') {
    return true;
  }
  return false;
}

void freeDictionary(Dictionary *d) {
  //Frees the dictionary for every child until the root nodes
  for (int i = 0; i < ALPHABET_N; i ++) {
    if (d->children[i] != NULL) {
      freeDictionary(d->children[i]);
    }
  }
  free(d);
}
