/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    vest.c
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */



int Length()
{
    token = getToken();
    if(token.type!=LEFT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type == STRING_LITERAL)
    {
        char *str = token.value;
    }
    else if(token.type == ID)
    {
        tNodePtr sym;
        char *symVarName = (char *)malloc((strlen(token.value)+strlen(funcName)+3));
        if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
        if(sym = stSearch(symVarName) == NULL) return SEM_ERR;
        if(sym.data.type != tString) return SEM_ERR;
        char *str = token.value;
    }
    else
    {
        return SEM_ERR;
    }
    token = getToken();
    if(token.type != RIGHT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    opInsert(&opTable,oStrlen,varName,str,NULL);
}

int chr()
{
    token = getToken();
    if(token.type!=LEFT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type == INT_LITERAL)
    {
        char *str = token.value;
    }
    else if(token.type == ID)
    {
        tNodePtr sym;
        char *symVarName = (char *)malloc((strlen(token.value)+strlen(funcName)+3));
        if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
        if(sym = stSearch(symVarName) == NULL) return SEM_ERR;
        if(sym.data.type != tInt) return SEM_ERR;
        char *str = token.value;
    }
    token = getToken();
    if(token.type != RIGHT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    opInsert(&opTable,oInt2Char,varName,str,NULL);
}

int ord()
{
    token = getToken();
    if(token.type!=LEFT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type == INT_LITERAL)
    {
        char *str = token.value;
    }
    else if(token.type == ID)
    {
        tNodePtr sym;
        char *symVarName = (char *)malloc((strlen(token.value)+strlen(funcName)+3));
        if(sscanf(symVarName,"*%s*%s",funcName,token.value) != 2) return INTERN_ERR;
        if(sym = stSearch(symVarName) == NULL) return SEM_ERR;
        if(sym.data.type != tInt) return SEM_ERR;
        char *str = token.value;
    }
    token = getToken();
    if(token.type != RIGHT_BRACKET) return SYNTAX_ERR;
    token = getToken();
    if(token.type != SEMICOLON) return SYNTAX_ERR;
    opInsert(&opTable,oInt2Char,varName,str,NULL);
}
