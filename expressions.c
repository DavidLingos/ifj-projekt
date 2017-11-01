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

void StackInit(tStack* st)
{
    if(st != NULL)
    {
        st->top == NULL;
    }
}

void StackPush(tStack *st, tData data)
{
    tStackItemPtr newItem = (tStackItemPtr)malloc(sizeof(struct tStackItem));
    if(newItem == NULL)
    {
        error = INTERN_ERR;
        return;
    }
    newItem->data = data;
    newItem->next = st->top;
    st->top = newItem;
}

void StackPop(tStack *st)
{
    tStackItemPtr pom;
    pom = st->top;
    if(!StackEmpty(st))
    {
        st->top = st->top->next;
    }
    free(pom);
}

void StackTop(tStack *st, tData *data)
{
    *data = st->top->data;
}

bool StackEmpty(tStack *st)
{
    return st->top == NULL;
}
