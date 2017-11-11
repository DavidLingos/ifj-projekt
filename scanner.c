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


typedef enum{
    INIT,
    IDENTIFIER,
    COMMENT_OR_DIVIDE,
    STRING_START,
    NUMBER,
    ONE_LINE_COMMENT,
    COMPARISON_LESS,
    COMPARISON_MORE,
    MULTI_LINE_COMMENT,
    LOAD_STRING,
    DECIMAL,
    EXPONENT,
    DECIMAL_LOAD,
    EXPONENT_LOAD
}TState;

#include <stdio.h>
#include <ctype.h>

#include "str.h"
#include "scanner.h"

FILE * sourceFile;
char c;

tToken getToken()
{
    int com = 0;
    token.type = NOTOKEN;
    TState state = INIT;

    while (1){
        c = (char)getc(sourceFile);
        switch (state){
            case INIT:
                //tokeny s delkou 1
                if (c == '(')
                    token.type = LEFT_BRACKET;
                else if (c == ')')
                    token.type = RIGHT_BRACKET;
                else if (c == ';')
                    token.type = SEMICOLON;
                else if (c == EOF)
                    token.type = END_OF_FILE;
                else if (c == '+')
                    token.type = PLUS;
                else if (c == '-')
                    token.type = MINUS;
                else if (c == '*')
                    token.type = MUL;
                else if (c == '\\')
                    token.type = DIV_INT;
                else if (c == '=')
                    token.type = EQUAL;
                else if (isalpha(c) || c == '_')
                {
                    state = IDENTIFIER;
                    c = (char)tolower(c);
                    strAddChar(token.value, c);
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
                    state = STRING_START;
                else if (isdigit(c)) {
                    state = NUMBER;
                    strAddChar(token.value, c);
                }
                else if (isspace(c))
                    state = INIT;
                else if (c == EOF)
                    token.type = END_OF_FILE;
                else
                    token.type = LEX_ERR;
                if (token.type != NOTOKEN)
                    return token;
                break;
            case IDENTIFIER:
                //povolene znaky identifikatoru
                if (isalpha(c) || isdigit(c) || c == '_')
                {
                    state = IDENTIFIER;
                    c = (char)tolower(c);
                    strAddChar(token.value, c);
                }
                else
                {
                    //kontrola, jestli nacteny identifikator neni klicove slovo
                    if (strCmpConstStr(token.value, "as") || strCmpConstStr(token.value, "asc")
                        || strCmpConstStr(token.value, "declare")
                        || strCmpConstStr(token.value, "dim") || strCmpConstStr(token.value, "do")
                        || strCmpConstStr(token.value, "else") || strCmpConstStr(token.value, "end")
                        || strCmpConstStr(token.value, "chr")
                        || strCmpConstStr(token.value, "function") || strCmpConstStr(token.value, "if")
                        || strCmpConstStr(token.value, "input")
                        || strCmpConstStr(token.value, "length") || strCmpConstStr(token.value, "loop")
                        || strCmpConstStr(token.value, "print") || strCmpConstStr(token.value, "return")
                        || strCmpConstStr(token.value, "scope")
                        || strCmpConstStr(token.value, "substr") || strCmpConstStr(token.value, "then")
                        || strCmpConstStr(token.value, "while"))
                        token.type = KEYWORD;
                    else if (strCmpConstStr(token.value, "integer"))
                        token.type = INTEGER;
                    else if (strCmpConstStr(token.value, "double"))
                        token.type = DOUBLE;
                    else if (strCmpConstStr(token.value, "string"))
                        token.type = STRING;
                    else
                        token.type = ID;
                }
                if (token.type != NOTOKEN)
                    return token;
                break;
            case COMMENT_OR_DIVIDE:
                //byl nacten znak '/', muze byt deleni nebo zacatek komentare
                if (c == '\'')
                    state = MULTI_LINE_COMMENT;
                else{
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = DIV;
                    return token;
                }
                break;
            case MULTI_LINE_COMMENT:
                //Text v komentari - nacitame, dokud nejsou symboly pro konec komentare nebo EOF
                //TODO EOF
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
                //jednoradkovy komentar, znaky ignorujeme, dokud neni znak konec radku nebo EOF
                //TODO: EOF
                if (c == '\n')
                    state = INIT;
                else
                    state = ONE_LINE_COMMENT;
                break;
            case COMPARISON_LESS:
                //nacten znak '<', na zaklade dalsiho znaku jednoznacne urcime token
                if (c == '=')
                    token.type = LESS_OR_EQ;
                else if (c == '>')
                    token.type = NOT_EQUAL;
                else{
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = LESS;
                }
                return token;

            case COMPARISON_MORE:
                //nacten znak '>', na zaklade dalsiho znaku jednoznacne urcime token
                if (c == '=')
                    token.type = MORE_OR_EQ;
                else {
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = MORE;
                }
                return token;

            case STRING_START:
                //nacten '!', kontrola jestli je dalsi znak '"', pote zacina retezcovy literal
                if (c != '\"')
                    token.type = LEX_ERR;
                else
                    state = LOAD_STRING;
                if (token.type != NOTOKEN)
                    return token;
                break;
            case LOAD_STRING:
                //scanner prochazi znaky retzcoveho literalu, uklada je do token.value
                //TODO: escape sequences, eof
                if (c == '\"') {
                    token.type = STRING_LITERAL;
                    return token;
                }
                else{
                    strAddChar(token.value, c);
                    state = LOAD_STRING;
                }
                break;
            case NUMBER:
                if (isdigit(c)) {
                    strAddChar(token.value, c);
                    state = NUMBER;
                }
                else if(c == '.') {
                    strAddChar(token.value, c);
                    state = DECIMAL;
                }
                else if (c == 'e' || c == 'E') {
                    strAddChar(token.value, c);
                    state = EXPONENT;
                }
                else {
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = INT_LITERAL;
                    return token;
                }
                break;
            case DECIMAL:
                //Po desetine tecce musi nasledovat alespon jedna cislice
                if (isdigit(c)){
                    strAddChar(token.value, c);
                    state = DECIMAL_LOAD;
                }
                else{
                    token.type = LEX_ERR;
                    return token;
                }
                break;
            case DECIMAL_LOAD:
                //Uz byla nactena cislice po tecce, muze tedy byt navracen token typu DOUBLE_LITERAL
                if (isdigit(c)){
                    strAddChar (token.value, c);
                    state = DECIMAL_LOAD;
                }
                else if (c == 'e' || c == 'E'){
                    strAddChar(token.value, c);
                    state = EXPONENT;
                }
                else {
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = DOUBLE_LITERAL;
                    return token;
                }
                break;
            case EXPONENT:
                //Po e/E musi nasledovat alespon jedna cislice nebo +/- a pak cislice
                if (isdigit(c)){
                    strAddChar (token.value, c);
                    state = EXPONENT_LOAD;
                }
                else if (c == '+' || c == '-'){
                    strAddChar(token.value, c);
                    state = EXPONENT;
                }
                else{
                    token.type = LEX_ERR;
                    return token;
                }
                break;
            case EXPONENT_LOAD:
                //Uz byla nactena cislice po e/E, muze tedy byt navracen token typu DOUBLE_LITERAL
                if (isdigit(c)){
                    strAddChar (token.value, c);
                    state = EXPONENT_LOAD;
                }
                else {
                    fseek(sourceFile, -1, SEEK_CUR);
                    token.type = DOUBLE_LITERAL;
                    return token;
                }

        }


    }

}
