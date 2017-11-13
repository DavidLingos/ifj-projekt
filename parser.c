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
 tSymbol *actParam;


int parser()
{

    int error;


    token = getToken();

    if(token.type == END_OF_FILE) return SYNTAX_ERR; //prázdný soubor

    error = all(); //začíná kontrola
    if(error != NO_ERR)
    {
        return error;
    }

    token = getToken();
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
    if(token.type != KEYWORD) return SYNTAX_ERR; //Neni klicove slovo
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
    token = getToken();

    if(strcmp(token.value,"Function")) return SYNTAX_ERR; //Musí být Declare Function

    token = getToken();

    if(token.type != ID) return SYNTAX_ERR; //Musí být Declare Function id
    tSymbol newSymbol; //novy zaznam do tabulky symbolu
    symbolInit(&newSymbol);
    newSymbol.name = token.value;
    newSymbol.isFunc = true; // symbol je funkce
    funcName = newSymbol.name;

    if(stSearch(newSymbol.name) != NULL) return SEM_ERR; // již byla deklarována

    token = getToken();
    if(token.type != LEFT_BRACKET) return SYNTAX_ERR; // Není levá závorka


    error = params(); // Pořeším parametry funkce
    if(error != NO_ERR) return error;

    token = getToken();

    if(strcmp(token.value,"As")) return SYNTAX_ERR; //Musí být Declare Function id(params) As


    token = getToken();
    if(token.type != KEYWORD) return SYNTAX_ERR; // Nepodporujici návratový typ
    else
    {
        if(!strcmp(token.value,"Integer")) newSymbol.type = tInt;
        else if(!strcmp(token.value,"Double")) newSymbol.type = tDouble;
        else if(!strcmp(token.value,"String")) newSymbol.type = tString;
        else return SYNTAX_ERR;
    }
    token = getToken();
    if(token.type != SEMICOLON) return SYNTAX_ERR; // Není středník, chyba

    stInsert(newSymbol.name,newSymbol); // Vložím do tabulky symbolu

    token = getToken(); // Připravím další token pro funkci handleFunc

}

int checkParams()
{
    tNodePtr fce = stSearch(funcName);
    actParam = fce.data.nextArg;

    token = getToken();
    if(token.type == RIGHT_BRACKET) {
        if(actParam == NULL) return NO_ERR; // žádné parametry
        else return SEM_ERR; //Funkce má parametry v deklaraci, ale v definici ne
    }
    if(token.type != ID) return SYNTAX_ERR; //Nemám id, chyba

    tSymbol newSymbol;
    symbolInit(&newSymbol);
    newSymbol.name = (char *)malloc((strlen(nazev)+strlen(funcName)+3));
    if(sscanf(newSymbol.name,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    newSymbol.isFunc = false;
    token = getToken();
    if(strcmp(token.value, "As")) return SYNTAX_ERR; // nedostal jsem As
    token = getToken();
    if(token.type == INTEGER)
    {
        newSymbol.type = tInt;
    }
    else if(token.type == DOUBLE)
    {
        newSymbol.type = tDouble;
    }
    else if(token.type == STRING)
    {
        newSymbol.type = tString;
    }
    else
    {
        return SYNTAX_ERR; //Nedostal jsem podporovaný typ
    }

    if(strcmp(newSymbol.name,actParam.name) || newSymbol.type != actParam.type) return SEM_ERR; //Neshoduje se parametr v definici a deklaraci

    return checkNextParams();
}

int checkNextParams()
{
    actParam = actParam.nextParam;
    token = getToken();
    if(token.type == RIGHT_BRACKET)
    {
        if(actParam == NULL) return NO_ERR;//žádné další parametry
        else return SEM_ERR; // nemám už další parametry v deklaraci, ale v definici ano
    }
    if(token.type != CARKA) return SYNTAX_ERR; //Nenasleduje čárka za parametrem
    token = getToken();
    if(token.type != ID) return SYNTAX_ERR; //Nemám id, chyba

    tSymbol newSymbol;
    symbolInit(&newSymbol);
    newSymbol.name = (char *)malloc((strlen(nazev)+strlen(funcName)+3));
    if(sscanf(newSymbol.name,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    newSymbol.isFunc = false;
    token = getToken();
    if(strcmp(token.value,"As")) return SYNTAX_ERR; // nedostal jsem As
    token = getToken();
    if(token.type == INTEGER)
    {
        newSymbol.type = tInt;
    }
    else if(token.type == DOUBLE)
    {
        newSymbol.type = tDouble;
    }
    else if(token.type == STRING)
    {
        newSymbol.type = tString;
    }
    else
    {
        return SYNTAX_ERR; //Nedostal jsem podporovaný typ
    }

    if(strcmp(newSymbol.name,actParam.name) || newSymbol.type != actParam.type) return SEM_ERR; //Neshoduje se parametr v definici a deklaraci

    return checkNextParams();
}

int endFunc()
{
    token = getToken();
    if(strcmp(token.value,"Function")) //Musí být End Function
    {
        return SYNTAX_ERR;
    }
    token = getToken();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    return NO_ERR;
}

int funcDefine()
{
    token = getToken();

    if(token.type != ID) return SYNTAX_ERR; // Musí být Function id
    funcName = token.value; //Nastavím jméno funkce

    tNodePtr fcDec;
    if(fcDec = stSearch(funcName) == NULL) return SEM_ERR; //Nebyla deklarována

    //TODO kontrola stejných parametru
    error = checkParams();
    if(error != NO_ERR) return error;

    token = getToken();

    if(strcmp(token.value,"As")) return SYNTAX_ERR; // Musí být Functionn id(params) As

    token = getToken();
    //TODO KONTROLA stejného type s deklarací
    if(token.type != KEYWORD) return SYNTAX_ERR;
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

    token = getToken();

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
    token = getToken(); //Další token pro handleFunc()
    return NO_ERR;
}

int params()
{
    token = getToken();
    if(token.type == RIGHT_BRACKET) return NO_ERR; // žádné parametry
    if(token.type != ID) return SYNTAX_ERR; //Nemám id, chyba

    tSymbol newSymbol;
    symbolInit(&newSymbol);
    newSymbol.name = (char *)malloc((strlen(nazev)+strlen(funcName)+3));
    if(sscanf(newSymbol.name,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    newSymbol.isFunc = false;
    token = getToken();
    if(strcmp(token.value, "As")) return SYNTAX_ERR; // nedostal jsem As
    token = getToken();
    if(token.type == INTEGER)
    {
        newSymbol.type = tInt;
    }
    else if(token.type == DOUBLE)
    {
        newSymbol.type = tDouble;
    }
    else if(token.type == STRING)
    {
        newSymbol.type = tString;
    }
    else
    {
        return SYNTAX_ERR; //Nedostal jsem podporovaný typ
    }

    newSymbol.nextParam == NULL;
    stInsert(newSymbol.name,newSymbol);


    //parametry funkce nextParam
    tNodePtr fce = stSearch(funcName);
    if(fce == NULL) return INTERN_ERR;
    fce.data.nextParam = &newSymbol;

    return nextParams();
}

int nextParams()
{
    token = getToken();
    if(token.type == RIGHT_BRACKET) return NO_ERR;//žádné další parametry
    if(token.type != CARKA) return SYNTAX_ERR; //Nenasleduje čárka za parametrem
    token = getToken();
    if(token.type != ID) return SYNTAX_ERR; //Nemám id, chyba

    tSymbol newSymbol;
    symbolInit(&newSymbol);
    newSymbol.name = (char *)malloc((strlen(nazev)+strlen(funcName)+3));
    if(sscanf(newSymbol.name,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    newSymbol.isFunc = false;
    token = getToken();
    if(strcmp(token.value,"As")) return SYNTAX_ERR; // nedostal jsem As
    token = getToken();
    if(token.type == INTEGER)
    {
        newSymbol.type = tInt;
    }
    else if(token.type == DOUBLE)
    {
        newSymbol.type = tDouble;
    }
    else if(token.type == STRING)
    {
        newSymbol.type = tString;
    }
    else
    {
        return SYNTAX_ERR; //Nedostal jsem podporovaný typ
    }

    //Pořeším parametry
    tNodePtr fce = stSearch(funcName);
    if(fce == NULL) return INTERN_ERR;
    tSymbolPtr pomPtr = fce.data.nextParam;
    while(pomPtr.nextParam != NULL)
    {
            pomPtr = pomPtr.nextParam;
    }
    pomPtr.nextParam = &newSymbol;

    stInsert(newSymbol.name,newSymbol);

    return nextParams();
}

int varDeclare()
{
    if(token.type != KEYWORD) return NO_ERR; // zadne deklarace
    if(!strcmp(token.value,"Dim")) //Dim
    {
        token = getToken();
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
        token = getToken();
        if(strcmp(token.value,"As")) return SYNTAX_ERR; // Musí být Dim id As
        token = getToken();
        if(token.type != KEYWORD) return SYNTAX_ERR;
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
        token = getToken();
        if(token.type == SEMICOLON)
        {
            stInsert(symVarName,newSymbol);
            token = getToken(); //Další token pro varDeclare
            return varDeclare();
        }
        else if(token.type == EQUAL) //Nastavení hodnoty
        {
            token = getToken();
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
        token = getToken(); //Nachystám daší token pro varDeclare
        return varDeclare();
    }
    return NO_ERR;
}

int body(bool isFunc)
{
    token = getToken();
    switch(token.type)
    {
    case ID:
        error = assignment();
        if(error != NO_ERR) return error;
        break;
    case KEYWORD:
        if(!strcmp(token.value,"End"))
        {
            token = getToken();
            if(isFunc && strcmp(token.vaue, "Function")) return SYNTAX_ERR; //Je to funkce, takže to muí být End Function;
            if(!isFunc && strcmp(token.vaue, "Scope")) return SYNTAX_ERR; //Je to Scope, takže musí být End Function
            token = getToken();
            if(token.type != SEMICOLON) return SYNTAX_ERR;
            return NO_ERR;
        }
        if(!strcmp(token.value,"Input"))
        {
            error = input();
            if(error != NO_ERR) return error;
            break;
        }
        if(!strcmp(token.value,"Print"))
        {

        }
        if(!strcmp(token.value,"Do"))
        {

        }
        if(!strcmp(token.value,"If"))
        {

        }
        if(!strcmp(token.value,"Return"))
        {
            //if(isFunc) Pořeš vrácení z funkce
            //else // jsem ve Scope a nesmí tam být return
        }
        else // Klíčová slova, která nemají být ve scope, jako např Declare
        {
            return SEM_ERR;
        }
        break;
    case default:
        return SEM_ERR;//Slova, co tam nemají být.
        break;
    }
    return body();
}

int return()
{

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
        token = getToken();
        if(token.type != EQUAL) return SYNTAX_ERR; //Není rovná se
        else
        {
            token = getToken();
            if(token.type == ID) {

                    //TODO pořesím fci

                if(symPtr = stSearch(token.value) == NULL) return SEM_ERR; //neznámá funkce
                if(!(symPtr.data.type.tInt && token.type == INT_LITERAL) ||
                    (symPtr.data.type.tDouble && token.type == DOUBLE_LITERAL) ||
                    (symPtr.data.type.tString && token.type == STRING_LITERAL)) return SEM_ERR; // nesedí typ operatoru a funkce
                token = getToken();
                if(token.type != LEFT_BRACKET) return SYNTAX_ERR;
                //parametry
                token = getToken();
                if(token.type != RIGHT_BRACKET) return SYNTAX_ERR;
                token = getToken();
                if(token.type != SEMICOLON) return SYNTAX_ERR;
                //dostat nějak hodnotu te funkce a uložit ji
                return fceValue();
            }
            else if(symPtr.data.type == tInt){
                if(token.type == STRING_LITERAL) return SYNTAX_ERR; //Chci int nebo double ale nedostal jsem ho
                symPtr.data.value.in = atoi(token.value);
                token = getToken();
                if(token.type != SEMICOLON) return SYNTAX_ERR;
            }
            else if(symPtr.data.type == tDouble){
                if(token.type == STRING_LITERAL) return SYNTAX_ERR; // Chci double nebo int ale nedostal jsem ho
                symPtr.data.value.db = atof(token.value);
                token = getToken();
                if(token.type != SEMICOLON) return SYNTAX_ERR;
            }
            else if(symPtr.data.type == tString){
                if(token.type != STRING_LITERAL) return SYNTAX_ERR; // Chci double nebo int ale nedostal jsem ho
                symPtr.data.value.st = atof(token.value);
                token = getToken();
                if(token.type != SEMICOLON) return SYNTAX_ERR;
            }
            else{
                return SYNTAX_ERR;
            }
            return NO_ERR;
        }
    }
}

int input()
{
    tNodePtr nodePtr;
    token = getToken();
    if(token.type != ID) SEM_ERR; //Neodstal jsem Input id
    char *symVarName = (char *)malloc((strlen(funcName))+strlen(token.value)+3); // *func*var\0 jméno lokální proměnné
    if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
    token = getToken();
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


