// This is one of the files we submit

#ifndef RECOGNIZEEXP_H
#define RECOGNIZEEXP_H

#include "scanner.h"

int acceptNumber(List *lp);
int acceptIdentifier(List *lp);
int acceptCharacter(List *lp, char c);
int acceptExpression(List *lp);
// void recognizeExpressions();
void recognizeEquations();

#endif
