/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    scanner.c
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */


 /* ---------- TODO------------*/
 /**
 * Definice funkci použitých při lexikální analýze
 * Scanner naimplementovaný jako konečný automat (switch/case).
 */
/* state:
 * 0: init
 *
 */
typedef enum{
    INIT,
    IDENTIFIER,
    COMMENT_OR_DIVIDE,
    STRING,
    NUMBER,
    ONE_LINE_COMMENT,
    COMPARISON_LESS,
    COMPARISON_MORE,
    MULTI_LINE_COMMENT,
    LOAD_STRING
};

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "str.h"
#include "scanner.h"

FILE * sourceFile;
char c;
void setSourceFile(FILE * f){
    sourceFile = f;
}


int getToken(string * value)
{
    int com = 0;
    int state = INIT;
    while (1){
        c = (char)getc(sourceFile);
        switch (state){
            case INIT:
                if (c == '(')
                    return LEFT_BRACKET;
                else if (c == ')')
                    return RIGHT_BRACKET;
                else if (c == ';')
                    return SEMICOLON;
                else if (c == EOF)
                    return END_OF_FILE;
                else if (c == '+')
                    return PLUS;
                else if (c == '-')
                    return MINUS;
                else if (c == *)
                    return MUL;
                else if (c == '\\')
                    return DIV_INT;
                else if (c == '=')
                    return EQUAL;
                else if (isalpha(c) || c == '_')
                {
                    state = IDENTIFIER;
                    c = (char)tolower(c);
                    strAddChar(value, c);
                }
                else if (c == '/' )
                    state = COMMENT_OR_DIVIDE;
                else if (c == '\'')
                    state = ONE_LINE_COMMENT;
                else if (c == '<')
                    state = COMPARISON_LESS;
                else if (c == '>')
                    state = COMPARISON_MORE;
                else if (c == '!')
                    state = STRING;
                else if (isdigit(c)) {
                    state = NUMBER;
                    strAddChar(value, c);
                }
                else if (isspace(c))
                    state = INIT;
                else
                    return LEX_ERR;
                break;
            case IDENTIFIER:
                if (isalpha(c) || isdigit(c) || c == '_')
                {
                    state = IDENTIFIER;
                    c = (char)tolower(c);
                    strAddChar(value, c);
                }
                else
                {
                    fseek(sourceFile, -1, SEEK_CUR);
                    if (strCmpConstStr(value, "as") || strCmpConstStr(value, "asc") || strCmpConstStr(value, "declare")
                        || strCmpConstStr(value, "dim") || strCmpConstStr(value, "do") || strCmpConstStr(value, "double")
                        || strCmpConstStr(value, "else") || strCmpConstStr(value, "end") || strCmpConstStr(value, "chr")
                        || strCmpConstStr(value, "function") || strCmpConstStr(value, "if")
                        || strCmpConstStr(value, "input") || strCmpConstStr(value, "integer")
                        || strCmpConstStr(value, "length") || strCmpConstStr(value, "loop")
                        || strCmpConstStr(value, "print") || strCmpConstStr(value, "return")
                        || strCmpConstStr(value, "scope") || strCmpConstStr(value, "string")
                        || strCmpConstStr(value, "substr") || strCmpConstStr(value, "then")
                        || strCmpConstStr(value, "while"))
                        return KEYWORD;
                    else
                        return ID;
                }
                break;
            case COMMENT_OR_DIVIDE:
                if (c == '\'')
                    state = MULTI_LINE_COMMENT;
                else{
                    fseek(sourceFile, -1, SEEK_CUR);
                    return DIV;
                }
                break;
            case MULTI_LINE_COMMENT:
                if (c == '\'') {
                    com = 1;
                    state = MULTI_LINE_COMMENT;
                }
                else if (c == '/' && com == 1) {
                    state = INIT;
                    com = 0;
                }
                else {
                    state = MULTI_LINE_COMMENT;
                    com = 0;
                }
                break;
            case ONE_LINE_COMMENT:
                if (c == '\n')
                    state = INIT;
                else
                    state = ONE_LINE_COMMENT;
                break;
            case COMPARISON_LESS:
                if (c == '=')
                    return LESS_OR_EQ;
                else if (c == '>')
                    return NOT_EQUAL;
                else{
                    fseek(sourceFile, -1, SEEK_CUR);
                    return LESS;
                }
            case COMPARISON_MORE:
                if (c == '=')
                    return MORE_OR_EQ;
                else {
                    fseek(sourceFile, -1, SEEK_CUR);
                    return MORE;
                }
            case STRING:
                if (c != '\"')
                    return LEX_ERR;
                else
                    state = LOAD_STRING;
                break;
            case LOAD_STRING:
                if (c == '\"')
                    return STRING_LITERAL;
                else{
                    strAddChar(value, c);
                    state = LOAD_STRING;
                }
                break;
            case NUMBER:
                if (isdigit(c))









        }


    }

}