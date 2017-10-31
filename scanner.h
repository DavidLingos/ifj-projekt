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

//identifikatory
#define ID 0
//klicova slova
#define AS 1
#define ASC 2
#define DECLARE 3
#define DIM 4
#define DO 5
#define DOUBLE 6
#define ELSE 7
#define END 8
#define CHR 9
#define FUNCTION 10
#define IF 11
#define INPUT 12
#define INTEGER 13
#define LENGTH 14
#define LOOP 15
#define PRINT 16
#define RETURN 17
#define SCOPE 18
#define STRING 19
#define SUBSTR 20
#define THEN 21
#define WHILE 22

//literaly
#define INT_LITERAL 23
#define DOUBLE_LITERAL 24
#define STRING_LITERAL 25

//Komentare
#define COM_ONE_LINE 26     // '''
#define COM_START 27        // '/''
#define COM_END 28          // ''/'

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
#define END_OF_FILE 43      // 'EOF'
#define LEX_ERR 100         // pri chybe

//Klicova slova - rozsireni
/*
#define AND 50
#define BOOLEAN 51
#define CONTINUE 52
#define ELSEIF 53
#define EXIT 54
#define FALSE 55
#define FOR 56
#define NEXT 57
#define NOT 58
#define OR 59
#define SHARED 60
#define STATIC 61
#define TRUE 62
 */

//struktura typu tokenu
typedef struct {
    int type;      //typ tokenu
    char *value;    //pokud je literal, pak jeho hodnota, jinak NULL
} tToken;

extern tToken token;

//deklarace funkci

//funkce vraci ciselny kod tokenu a do value ulozi hodnotu literalu (nebo null pokud token neni literal)

tToken get_token(); //TODO knihovnu pro praci s retezci

