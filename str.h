#ifndef IFJ_PROJEKT_STRING_H
#define IFJ_PROJEKT_STRING_H


/*Knihovna pro praci s nekonecne dlouhymi retezci prevzata ze "Zjednodusene implementace
 * interpretu jednoducheho jazyka"  */


typedef struct
{
    char* str;		// misto pro dany retezec ukonceny znakem '\0'
    int length;		// skutecna delka retezce
    int allocSize;	// velikost alokovane pameti
} string;

int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);


#endif //IFJ_PROJEKT_STRING_H