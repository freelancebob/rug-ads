// Primary texteditor.c file

#include "TextEditor.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TEXT_EDITOR_INITIAL_CAPACITY 10

TextEditor* createTextEditor(void) {
  TextEditor *editor = malloc(sizeof(*editor));
  
  editor->text = malloc(TEXT_EDITOR_INITIAL_CAPACITY * sizeof(*editor->text));
  assert(editor->text != NULL);

  editor->length = 0;
  editor->capacity = TEXT_EDITOR_INITIAL_CAPACITY;

  editor->undoStack = newStack(8);
  editor->redoStack = newStack(8);

  return editor;
}

void insertCharacter(TextEditor *editor, int pos, char character) {
  // double the size of the editor array if the capacity is reached
  if (editor->length >= editor->capacity) { 
    editor->capacity *= 2;
    editor->text = realloc(editor->text, editor->capacity * sizeof(*editor->text));
    assert(editor->text != NULL);
  }

  // shift characters after position to the right
  for (int i = pos; i < editor->length; i ++) {
    editor->text[i + 1] = editor->text[i];
  }

  // build editoperation struct
  EditOperation thisOp;
  enum EditOperationType type = INSERT;
  thisOp.character = character;
  thisOp.position = pos;
  thisOp.type = type;

  // store operation in undo stack
  push(thisOp, &(editor->undoStack));

  // write character and increase length
  editor->text[pos] = character;
  editor->length += 1;
}

void deleteCharacter(TextEditor *editor, int pos) {
  if (editor->length == 0) {
    // do nothing
    return;
  }

  if (pos > editor->length) {
    // do nothing
    return;
  }

  // build editoperation struct
  EditOperation thisOp;
  enum EditOperationType type = DELETE;
  thisOp.character = editor->text[pos];
  thisOp.position = pos;
  thisOp.type = type;

  // store operation in undo stack
  push(thisOp, &(editor->undoStack));

  // if last character is targetted, reduce length and this character will be skipped anyway
  if (pos == editor->length) {
    editor->length -= 1;
    return;
  }

  // just shift chars after pos to the left, this will overrwrite character at pos too
  for (int i = pos + 1; i < editor->length; i ++) {
    editor->text[i - 1] = editor->text[i];
  }
  editor->length -= 1;
}

void undo(TextEditor *editor) {
  // if no undo steps then do nothing
  if (isEmptyStack(editor->undoStack)) {
    return;
  }

  EditOperation lastOp = pop(&(editor->undoStack));

  // do the opposite of the last operation
  if (lastOp.type == INSERT) {
    deleteCharacter(editor, lastOp.position);
  } else if (lastOp.type == DELETE) {
    insertCharacter(editor, lastOp.position, lastOp.character);
  }

  // push the operation just saved in the undo stack to the redo stack
  // this effectively saves the undo as a step that can be undone
  push(pop(&(editor->undoStack)), &(editor->redoStack));
}

void redo(TextEditor *editor) {
  // Optional for the bonus exercise
  if (isEmptyStack(editor->redoStack)) {
    return;
  }

  EditOperation lastOp = pop(&(editor->redoStack));

  // do the opposite of the last operation
  if (lastOp.type == INSERT) {
    deleteCharacter(editor, lastOp.position);
  } else if (lastOp.type == DELETE) {
    insertCharacter(editor, lastOp.position, lastOp.character);
  }

  // this time don't do anything because it correctly saved the undo step
  // push(pop(&(editor->undoStack)), &(editor->redoStack));
}

void destroyTextEditor(TextEditor *editor) {
  free(editor->text);
  freeStack(editor->undoStack);
  freeStack(editor->redoStack);
  free(editor);
}

void printText(TextEditor *editor) {
  if (editor->length == 0) {
    printf("EMPTY\n");
    return;
  }

  // Simply loop through text array
  for (int i = 0; i < editor->length; i ++) {
    printf("%c", editor->text[i]);
  }
  printf("\n");
}

int main(void) {
  TextEditor *editor = createTextEditor();
  char command;
  int pos;
  char character;

  while(1) {
    scanf(" %c", &command);
    switch (command) {
    case 'i':
      scanf("%d %c", &pos, &character);
      insertCharacter(editor, pos, character);
      break;
    case 'd':
      scanf("%d", &pos);
      deleteCharacter(editor, pos);
      break;
    case 'u':
      undo(editor);
      break;
    case 'r':
      redo(editor);
      break;
    case 'q':
      printText(editor);
      destroyTextEditor(editor);
      return EXIT_SUCCESS;
    default:
      printf("Unknown command.\n");
      break;
    }
  }
}
