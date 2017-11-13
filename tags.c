/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    tags.c
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */

#include "tags.h"

void opInit(opItem **opTable)
{
    opTable = NULL;
    lastOp = 0;
}

void opInsert(opItem **opTable, size_t operation, tNodePtr op1. tNodePtr op2, tNodePtr result)
{
    if(lastOp % MOREOPS == 0)
    {
        *opTable = (opItem *)realloc((*opTable,lastOp+MOREOPS)*sizeof(opItem));
        if(opTable == NULL)
        {
            error = INTERN_ERR;
            return;
        }
    }

    (*opTable)[lastOp].op1 = op1;
    (*opTable)[lastOp].op2 = op2;
    (*opTable)[lastOp].operation = operation;
    (*opTable)[lastOp].result = result;

    lastOp++;
}

void opDestroy(opItem **opTable)
{
    free(opTable);
}
