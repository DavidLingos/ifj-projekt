/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    scanner.h
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */


 /* ---------- TODO------------*/
 /**
 * Deklarace funkci použitých při lexikální analýze, hlavní funkce je GetToken(), která vrací další token
 * Definice maker pro tokeny, která bude používat i parser při sémantické analýze.
 * Definice klíčových a rezervovaných slov
 */

#include "str.h"

//identifikatory
#define NOTOKEN 0
#define ID 1
//klicova slova - ulozene ve value
#define KEYWORD 5
//klicova slova datovych typu
#define INTEGER 6
#define DOUBLE 7
#define STRING 8
//literaly
#define INT_LITERAL 23
#define DOUBLE_LITERAL 24
#define STRING_LITERAL 25

//Operatory
#define PLUS 29             // '+'
#define MINUS 30            // '-'
#define MUL 31              // '*'
#define DIV 32              // '/'
#define DIV_INT 33          // '\'
#define LESS 34             // '<'
#define MORE 35             // '>'
#define LESS_OR_EQ 36       // '<='
#define MORE_OR_EQ 37       // '>='
#define EQUAL 38            // '='
#define NOT_EQUAL 39        // '<>'

//Specialni znaky
#define LEFT_BRACKET 40     // '('
#define RIGHT_BRACKET 41    // ')'
#define SEMICOLON 42        // ';'
#define COMMA 43
#define END_OF_FILE 44      // 'EOF'
#define LEX_ERR 100         // pri chybe

//Klicova slova - rozsireni

//struktura typu tokenu
typedef struct {
    int type;      //typ tokenu
    string *value;    //pokud je literal nebo klicove slovo, pak jeho hodnota, jinak NULL
} tToken;

extern tToken token;

//deklarace funkci

//funkce vraci ciselny kod tokenu a do value ulozi hodnotu literalu (nebo null pokud token neni literal)
tToken getToken(); //TODO knihovnu pro praci s retezci


