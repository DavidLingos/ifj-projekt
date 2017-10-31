/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    parser.c
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "parser.h"


 /* ---------- TODO------------*/
 /**
 * Definice funkci použitých při syntaktické analýze.
 * Načítání tokenů a kontrola správných sémantických pravidel.
 * int parser(); vrací 0, pokud ok jinak číslo chyby z errorCodes.h
 */

 char *funcName;


int parser()
{

    int error;


    token = get_token();

    if(token.type == END_OF_FILE) return SYNTAX_ERR; //prázdný soubor

    error = all(); //začíná kontrola
    if(error != NO_ERR)
    {
        return error;
    }

    token = get_token();
    if(token.type != END_OF_FILE)
    //End Scope neni posledni
    {
        return SYNTAX_ERR;
    }

    return NO_ERR;
}

int all()
{
    int error;
    error = handleFunc(); //Definice a deklarace funkci

    if(error != NO_ERR) return error;

    return NO_ERR;
}

int handleFunc()
{
    int error;
    if(token.type != KEY) return SYNTAX_ERR; //Neni klicove slovo
    if(!strcmp(token.value,"Declare"))
    {
        error = funcDeclare(); // Deklarace funkce
        if(error == ERR_OḰ) return handleFunc();
        else return error;
    }
    else if(!strcmp(token.value,"Function"))
    {
        error = funcDefine(); //definice funkce
        if(error == ERR_OḰ) return handleFunc();
        else return error;
    }
    else if(!strcmp(token.value,"Scope"))
    {
        return scope(); // hlavní tělo programu
    }
    else
    {
        return SYNTAX_ERR;
    }
}

int funcDeclare()
{
    token = get_token();

    if(strcmp(token.value,"Function")) return SYNTAX_ERR; //Musí být Declare Function

    token = get_token();

    if(token.type != ID) return SYNTAX_ERR; //Musí být Declare Function id
    tSymbol newSymbol; //novy zaznam do tabulky symbolu
    symbolInit(&symbol);
    newSymbol.name = token.value;
    newSymbol.isFunc = true; // symbol je funkce

    if(stSearch(newSymbol.name) != NULL) return SEM_ERR; // již byla deklarována

    params(); // Pořeším parametry funkce

    token = get_token();

    if(strcmp(token.value,"As")) return SYNTAX_ERR; //Musí být Declare Function id(params) As


    token = get_token();
    if(token.type != KEY) return SYNTAX_ERR; // Nepodporujici návratový typ
    else
    {
        if(!strcmp(token.value,"Integer")) newSymbol.type = tInt;
        else if(!strcmp(token.value,"Double")) newSymbol.type = tDouble;
        else if(!strcmp(token.value,"String")) newSymbol.type = tString;
        else return SYNTAX_ERR;
    }
    token = get_token();
    if(token.type != SEMICOLON) return SYNTAX_ERR; // Není středník, chyba

    stInsert(newSymbol.name,newSymbol); // Vložím do tabulky symbolu

    token = get_token(); // Připravím další token pro funkci handleFunc

}

int endFunc()
{
    token = get_token();
    if(strcmp(token.value,"Function")) //Musí být End Function
    {
        return SYNTAX_ERR;
    }
    token = get_token();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    return NO_ERR;
}

void funcDefine()
{
    token = get_token();

    if(token.type != ID) return SYNTAX_ERR; // Musí být Function id
    funcName = token.value; //Nastavím jméno funkce

    tNodePtr fcDec;
    if(fcDec = stSearch(funcName) == NULL) return SEM_ERR; //Nebyla deklarována

    //TODO kontrola stejných parametru
    checkParams();

    token = get_token();

    if(strcmp(token.value,"As")) return SYNTAX_ERR; // Musí být Functionn id(params) As

    token = get_token();
    //TODO KONTROLA stejného type s deklarací
    if(token.type != KEY) return SYNTAX_ERR;
    else
    {
        if(!strcmp(token.value,"Integer"))
        {
            if(fcDec.data.type != tInt) return SEM_ERR; //Neni stejny navraatový typ jako při deklaraci
        }
        else if(!strcmp(token.value,"Double"))
        {
            if(fcDec.data.type != tDouble) return SEM_ERR; //Neni stejny navraatový typ jako při deklaraci
        }
        else if(!strcmp(token.value,"String"))
        {
            if(fcDec.data.type != tString) return SEM_ERR; //Neni stejny navraatový typ jako při deklaraci
        }
        else return SYNTAX_ERR;
    }

    token = get_token();

    if(strcmp(token.value,"End")) //Tělo není prázdné
    {
        error = varDeclare(); //Podívám se, jestli mám nějaké proměnné na deklarování
        if(!strcmp(token.value,"End"))
        {
            return endFunc(); // Musí být End Function;
        }
        if(error = NO_ERR) return body(1); //Zkontroluji tělo funkce
        else return error;
    }
    else //Prázdné tělo funkce
    {
        return endFunc(); //Musí být End Function;
    }
    token = get_token(); //Další token pro handleFunc()
    return NO_ERR;
}

int varDeclare()
{
    if(token.type != KEY) return NO_ERR; // zadne deklarace
    if(!strcmp(token.value,"Dim")) //Dim
    {
        token = get_token();
        if(token.type != ID) return SYNTAX_ERR; // Musí být Dim id
        if(stSearch(token.value) != NULL) //KDyž se ID = nazvu nadeklarované funkce
        {
            fprintf(stderr,"Identifikator je stejny jako jmeno funkce.");
            return SEM_ERR;
        }
        char *symVarName = (char *)malloc((strlen(funcName))+strlen(token.value)+3); // *func*var\0 jméno lokální proměnné
        if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
        if(stSearch(symVarName) != NULL) //Nadeklarovaný identifikátor
        {
            fprintf(stderr,"Identifikator %s byl již deklarován.", symVarName);
            return SEM_ERR;
        }
        tSymbol newSymbol; //Nový symbol v symtable
        symbolInit(&newSymbol);
        newSymbol.isFunc = false;
        newSymbol.name = symVarName;
        token = get_token();
        if(strcmp(token.value,"As")) return SYNTAX_ERR; // Musí být Dim id As
        token = get_token();
        if(token.type != KEY) return SYNTAX_ERR;
        else
        {
            if(!strcmp(token.value,"Integer"))
            {
                newSymbol.type = tInt;
                newSymbol.value.in = 0;
            }
            else if(!strcmp(token.value,"Double"))
            {
                newSymbol.type = tDouble;
                newSymbol.value.db = 0.0;
            }
            else if(!strcmp(token.value,"String"))
            {
                newSymbol.type = tString;
                newSymbol.value.st = "";
            }
            else return SYNTAX_ERR; //Není zadán podporovaný datový typ
        }
        token = get_token();
        if(token.type == SEMICOLON)
        {
            stInsert(symVarName,newSymbol);
            token = get_token(); //Další token pro varDeclare
            return varDeclare();
        }
        else if(token.type == EQUAL) //Nastavení hodnoty
        {
            token = get_token();
            switch(newSymbol.type)
            {
                case tInt:
                    if(token.type == STRING_LITERAL) return SYNTAX_ERR; //Chci int nebo double ale nedostal jsem ho
                    else
                    {
                        newSymbol.value.in = atoi(token.value);
                    }
                case tDouble:
                    if(token.type == STRING_LITERAL) return SYNTAX_ERR; // Chci double nebo int ale nedostal jsem ho
                    else
                    {
                        newSymbol.value.db = atof(token.value);
                    }
                case tString:
                    if(token.type != STRING_LITERAL) return SYNTAX_ERR; // Chci string, ale nedostal jsem ho
                    else
                    {
                        newSymbol.value.st = token.value;
                    }
            }
            stInsert(symVarName,newSymbol); //Vložím do tabulky symbolů
        }
        else return SYNTAX_ERR; //Nemám ani středník ani =
        token = get_token(); //Nachystám daší token pro varDeclare
        return varDeclare();
    }
    return NO_ERR;
}

int body(bool isFunc)
{
    switch(token.type)
    {
    case ID:
        error = assignment();
        if(error != NO_ERR) return error;
    case INPUT:
        error = input();
        //najdu proměnnou id, jestli nemám, chyba,
        //zjistím typ
        //vypíšu ?
        //pkusím se načíst hodnotu
        //pokud nejde, uložím zakladní hodnotu.
        //zkontroluji středník a čus
    case PRINT:
        //TODO
    case IF:
        //spustím if
    case DO:
        //spustím for
    case RETURN:
        // zkontroluji jestli jsem ve funkci nebo scope pomocí funcName
        // pokud jsem ve Funkci
    }
}

int assignment()
{
    tNodePtr symPtr;
    if(symPtr = stSearch(token.value) != NULL)
    {
        return SEM_ERR;
    }
    char *symVarName = (char *)malloc((strlen(funcName))+strlen(token.value)+3); // *func*var\0 jméno lokální proměnné
    if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    if(symPtr = stSearch(symVarName) == NULL) return SEM_ERR; // id nebylo deklarováno
    else
    {
        token = get_token();
        if(token.type != EQUAL) return SYNTAX_ERR; //Není rovná se
        else
        {
            token = get_token();
            if(token.type == ID) {
                    //pořesím fci
                if(symPtr = stSearch(token.value) == NULL) return SEM_ERR; //neznámá funkce
                if(!(symPtr.data.type.tInt && token.type == INT_LITERAL) ||
                    (symPtr.data.type.tDouble && token.type == DOUBLE_LITERAL) ||
                    (symPtr.data.type.tString && token.type == STRING_LITERAL)) return SEM_ERR; // nesedí typ operatoru a funkce
                token = get_token();
                if(token.type != RIGHT_BRACKET) return SYNTAX_ERR;
                //parametry
                token = get_token();
                if(token.type != LEFT_BRACKET) return SYNTAX_ERR;
                token = get_token();
                if(token.type != SEMICOLON) return SYNTAX_ERR;
                //dostat nějak hodnotu te funkce a uložit ji
                return fceValue();
            }
            if(symPtr.data.type == tInt){
                if(token.type == STRING_LITERAL) return SYNTAX_ERR; //Chci int nebo double ale nedostal jsem ho
                else
                {
                    symPtr.data.value.in = atoi(token.value);
                }
            }
            if(symPtr.data.type == tDouble){
                if(token.type == STRING_LITERAL) return SYNTAX_ERR; // Chci double nebo int ale nedostal jsem ho
                else
                {
                    symPtr.data.value.db = atof(token.value);
                }
            }
            if(symPtr.data.type == tString){
                if(token.type != STRING_LITERAL) return SYNTAX_ERR; // Chci double nebo int ale nedostal jsem ho
                else
                {
                    symPtr.data.value.st = atof(token.value);
                }
            }
        }
    }
}

int input()
{
    tNodePtr nodePtr;
    token = get_token();
    if(token.type != ID) SEM_ERR; //Neodstal jsem Input id
    char *symVarName = (char *)malloc((strlen(funcName))+strlen(token.value)+3); // *func*var\0 jméno lokální proměnné
    if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    token = get_token();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    if(nodePtr = stSearch(symVarName) == NULL) return SEM_ERR; //Identifikator neni deklarovany
    else{
        printf("? ");
        if(nodePtr.data.type == tInt)
        {
            if(!scanf("%d",&(nodePtr.data.value.in)) return TYPE_ERR; //Nepodařilo se načíst int
        }
        if(nodePtr.data.type == tDouble)
        {
            if(!scanf("%g",&(nodePtr.data.value.db)) return TYPE_ERR; //Nepodařilo se načíst double
        }
        if(nodePtr.data.type == tString)
        {
            if(!scanf("%s",nodePtr.data.value.st)) return TYPE_ERR; //Nepodařilo se načíst double
        }
    }
}

int scope()
{
    body(0);
}


