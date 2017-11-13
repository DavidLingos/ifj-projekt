/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    symtable.c
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */


 /* ---------- TODO------------*/
 /**
 * Definice funkcí pro tabulku symbolů.
 * Definice ákladních funkcí nad datovou strukturou BVS.
 */

#include <stdio.h>
#include "symtable.h"
#include <string.h>

void stInit()
{
    st = NULL;
}

void symbolInit(tSymbolPtr symbol)
{
    if(symbol != NULL)
    {
        symbol->name = NULL;
        symbol->type = NULL;
    }
}

void stInsert(char *key,tSymbol data)
{
    btInsert(st,key,data);
}

void btInsert(tNodePtr root, char *key, tSymbol data)
{
    if(root==NULL)
    {
        root = malloc(struct tNode);
        if(root == NULL)
        {
            err = INTERN_ERR;
            return;
        }

        root->key = key;
        root->data = data;
        root->lPtr = root->rPtr = NULL;
    }
    else
    {
        int i = strcmp(root->key, key);

        if(i<0) return stInsert(root->rPtr, key, data);
        else if(i > 0) stInsert(root->lPtr, key, data);
        else root->data = data;
    }
}

void stDispose()
{
    btDispose(st);
}

tNodePtr stSearch(char *key)
{
    return btSearch(st, key);
}

void btDispose(tNodePtr root)
{
    if(root != NULL)
    {
        btDispose(root->lPtr);
        btDispose(root->rPtr);

        free(root);
    }
}

tNodePtr btSearch(tNodePtr root, char *key)
{
    if(root != NULL)
    {
        if(!strcmp(root->key,key)) return root;
        else if(strcmp(root->key,key) > 0) return btSearch(root->lPtr, key);
        else return btSearch(root->rPtr, key);
    }
    else return NULL;
}



