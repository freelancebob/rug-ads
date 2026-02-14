// By Kevin Poeze (S NUM TODO) and Dylan Todorov (S6277438)

// NOTE!!!
// This is one of the files for us to submit
// Based on "recognizeExp.c", however instead of just accepting expressions
// eg. 3 x + 7
// this file must accept equations
// eg. 3 x + 7 = 4

#include "recognizeEq.h"

#include "scanner.h"

#include <stdio.h>  /* printf */
#include <stdlib.h> /* NULL, free */
#include <string.h> /* strcmp */

int max(int a, int b) {
  return (a > b) ? a : b;
}

/* The functions acceptNumber, acceptIdentifier and acceptCharacter have as
 * (first) argument a pointer to an token list; moreover acceptCharacter has as
 * second argument a character. They check whether the first token
 * in the list is a number, an identifier or the given character, respectively.
 * When that is the case, they yield the value 1 and the pointer points to the rest of
 * the token list. Otherwise they yield 0 and the pointer remains unchanged.
 */

// Do not change! (according to the PDF)
int acceptNumber(List *lp) {
  if (*lp != NULL && (*lp)->tt == Number) {
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

// Do not change! (according to the PDF)
int acceptIdentifier(List *lp) {
  if (*lp != NULL && (*lp)->tt == Identifier) {
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

// Do not change! (according to the PDF)
int acceptCharacter(List *lp, char c) {
  if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c) {
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

/* The functions acceptFactor, acceptTerm and acceptExpression have as
 * argument a pointer to a token list. They check whether the token list
 * has an initial segment that can be recognized as factor, term or expression, respectively.
 * When that is the case, they yield the value 1 and the pointer points to the rest of
 * the token list. Otherwise they yield 0 and the pointer remains unchanged.
 */

int acceptFactor(List *lp) {
  return
    (  acceptNumber(lp)
    || acceptIdentifier(lp)
    );

    // no longer accepts parentheses as per the PDF
    // || ( acceptCharacter(lp,'(')
    //    && acceptExpression(lp)
    //    && acceptCharacter(lp,')')
    //   )
}

// Can be changed!
// Note: the definition of terms in the grammar in the PDF do NOT allow a "-" before the term. only expressions can have a single minus before them
int acceptTerm(List *lp) {
  // accept possible number at beginning, but dont break if it isnt present
  int hasNumber = acceptNumber(lp);

  // if there is an identifier then an exponent is allowed
  if (acceptIdentifier(lp)) {
    if (acceptCharacter(lp, '^')) {
      if (!acceptNumber(lp)) {
        return 0;
      }
    }
  } else {
    if (!hasNumber) {
      // if there is no identifier, then there must at least be a number, otherwise break
      return 0;
    }
  }
  
  // this is no longer used, because in this grammer terms can only have ^ in them
  // while (acceptCharacter(lp, '*') || acceptCharacter(lp, '/') || acceptCharacter(lp, '^')) {
  //   if (!acceptFactor(lp)) {
  //     return 0;
  //   }
  // }
  return 1;
}

// Can be changed!
// For some reason the original implementation of this didnt allow expressions like "2x" or "-x" so i had to add that
int acceptExpression(List *lp) {
  // accept the potential "-" before the term
  acceptCharacter(lp, '-');

  if (!acceptTerm(lp)) {
    return 0;
  }
  while (acceptCharacter(lp, '+') || acceptCharacter(lp, '-')) {
    if (!acceptTerm(lp)) {
      return 0;
    }
  } /* no + or -, so we reached the end of the expression */
  return 1;
}

// new method: acceptEquation which just accepts an expression, an '=' char and another expression
int acceptEquation(List *lp) {
  if (!acceptExpression(lp)) {
    // printf("no first expression\n");
    return 0;
  }
  if (!acceptCharacter(lp, '=')) {
    // printf("no equals\n");
    return 0;
  }
  if (!acceptExpression(lp)) {
    // printf("no second expression\n");
    return 0;
  }
  // <expression> '=' <expression> format was accepted, return 1
  return 1;
}

// new function: recognizeEquations:
void recognizeEquations() {
  char *ar;
  List tl, tl1;
  printf("give an equation: ");
  ar = readInput();

  // keep reading inputs until a '!' is found
  while (ar[0] != '!') {
    tl = tokenList(ar);
    List originalTl = tl;
    
    // printf("the token list is ");
    printList(tl);
    tl1 = tl;

    int result = acceptEquation(&tl1);
    
    // traverse the token list and determine the highest exponent and unique variable
    // since only one variable is accepted, its easiest to make sure only one unique variable exists
    char *lastVar = "0";
    int degree = 0;
    int wasExp = 0;
    int wasId = 0;
    int wasVar = 0;
    int uniqueVar = 0;

    while (tl != NULL) {
      // if a symbol is found...
      if (tl->tt == Symbol) {
        if (tl->t.symbol == '^') {
          wasExp = 1;
        } else if (wasId) {
          degree = max(degree, 1);
        }

      // if a number is found..
      } else if (tl->tt == Number && wasExp) {
        degree = max(degree, tl->t.number);

      // if an identifier is found..
      } else if (tl->tt == Identifier) {
        wasId = 1;

        if (!wasVar) {
          wasVar = 1;
          uniqueVar = 1;
          lastVar = tl->t.identifier;
        } else {
          if (strcmp(lastVar, tl->t.identifier) != 0) {
            uniqueVar = 0;
          }
        }
      }

      // clear previously found token flags
      if (tl->tt != Identifier) {
        wasId = 0;
      }
      if (tl->tt != Symbol || tl->t.symbol != '^') { //
        wasExp = 0;
      }

      tl = tl->next;
    }

    // if there was an identifier at the very end, then the degree must be at least 1
    // this doesnt get tracked in the while loop above so we do it manually
    if (wasId) {
      degree = max(degree, 1);
    }

    if (result && tl1 == NULL) {
      if (uniqueVar != 1) {
        printf("this is an equation, but not in 1 variable\n");
      } else {
        printf("this is an equation in 1 variable of degree %d\n", degree);
      }
    } else {
      printf("this is not an equation\n");
    }

    free(ar);
    freeTokenList(originalTl);

    printf("\ngive an equation: ");
    ar = readInput();
  }
  free(ar);
  printf("good bye\n");
}
