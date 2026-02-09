/* based on listTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#ifndef LIBLIST_H
#define LIBLIST_H

#include "EditOperation.h"

typedef struct ListNode* List;

struct ListNode {
  EditOperation item;
  List next;
};

List newEmptyList(void);
int isEmptyList(List li);
List addItem(EditOperation n, List li);
EditOperation firstItem(List li);
List removeFirstNode(List li);
void freeList(List li);
void visit(List li);
void visitList(List li);
void visitListRec(List li);
EditOperation itemAtPos(List li, int p);
List addItemAtPos(List li, EditOperation n, int p);
List addItemAtPosIt(List li, EditOperation n, int p);
List removeItem(List li, EditOperation n);
List removeItemIt(List li, EditOperation n);

#endif
