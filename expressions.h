/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    symtable.h
 * Datum:    1. 11. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
*/

#include "scanner.h"
#include "parser.h"
#include "errorcodes.h"
#include "symtable.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    tPLUS, // +
    tMINUS, // -
    tKRAT, // *
    tDELENO, // /
    tCELDELENO, // '\'
    tMENSI, // <
    tVETSI, // >
    tMENSIROVNO, // <=
    tVETSIROVNO, // >=
    tROVNO, // ==
    tNEROVNO, // <>
    tLEVA, // (
    tPRAVA,// )
} tOperator;

typedef struct
{
    tSymbol symbol;
    tOperator op;
} tData;

typedef struct tStackItem{
    struct tStackItem *next;
    tData data;
} *tStackItemPtr;

typedef struct
{
    tStackItemPtr top;
} tStack;


void StackInit(tStack *st);
void StackPush(tStack *st, tData data);
void StackPop(tStack *st);
void StackTop(tStack *st, tData *data);
bool StackEmpty(tStack *st);
