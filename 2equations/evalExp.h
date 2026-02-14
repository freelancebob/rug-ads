/* Based on: evalExp.h, Gerard Renardel, 29 January 2014 */

#ifndef EVALEXP_H
#define EVALEXP_H

#include "scanner.h"

int valueExpression(List *lp, double *wp);
void evaluateExpressions();

#endif
