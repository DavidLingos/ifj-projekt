/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    tags.h
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */

#pragma once

#include <stdlib.h>
#include "symtable.h"


extern int lastOp;

#define MOREOPS 10

typedef enum
{
    //Práce s rámci
    oMove,
    oCreateFrame,
    oPushFrame,
    oPopFrame,
    oDefVar,
    oCall,
    oReturn,

    //Práce s datovými rámci
    oPushs,
    oPops,
    oClears,

    //Aritmetické, relační, booleovské a konverzní instrukce
    oAdd,
    oSub,
    oMul,
    oDiv,
    oLess,
    oMore,
    oEqual,
    oELess,
    oEMore,
    oInt2Char,
    oStri2Int,
    //In-out
    oRead,
    oWrite,

    //Práce s řetězci
    oConcat,
    oStrlen,
    oGetchar,
    oSetChar,

    //Řízení toku programu
    oLabel,
    oJump,
    oJumpifeq,
    oJumpifneq
} tOp;

typedef struct{
    tOp operation;
    char *op1;
    char *op2;
    char *result;
} opItem;

void opInit(opItem **opTable);
void opInsert(opItem **opTable, size_t operation, char* op1, char* op2, char* result);
void opDestroy(opItem **opTable);


extern opItem *opTable;

