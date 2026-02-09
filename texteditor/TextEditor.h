#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "LibStack.h"

typedef struct TextEditor {
  char *text;
  int length;
  int capacity;

  Stack undoStack;
  Stack redoStack;
} TextEditor;

TextEditor* createTextEditor(void);
void destroyTextEditor(TextEditor *editor);

void insertCharacter(TextEditor *editor, int pos, char character);
void deleteCharacter(TextEditor *editor, int pos);
void undo(TextEditor *editor);
void redo(TextEditor *editor);
void printText(TextEditor *editor);

#endif
