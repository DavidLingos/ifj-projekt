/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    symtable.h
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */


 /* ---------- TODO------------*/
 /**
 * Deklarace datových struktur a funkcí pro tabulku symbolů.
 * Deklarace datové struktury binární vyhledávací strom a základních funkcí nad ní,
 */

typedef enum{
   tNil,
   tInt,
   tDouble,
   tString
} tType;

typedef union{
    int in;
    double db;
    char *st;
} tVal;

typedef struct tSymbol{
    char *name;
    bool isFunc;
    tType type;
    tVal value;
    struct tSymbol *nextParam;
} tSymbol, *tSymbolPtr;

typedef struct tNode{
   char *key;
   tSymbol data;
   struct tNode *lPtr;
   struct tNode *rPtr;
} *tNodePtr;

/*GLOBAL*/
extern tNodePtr st;


void stInit();
void symbolInit(tSymbolPtr symbol);
void stInsert(char *key, tSymbol data);
void btInsert(tNodePtr root, char *key, tSymbol data);
void stDispose();
void btDispose(tNodePtr root);
tNodePtr stSearch(char *key);
tNodePtr btSearch(tNodePtr root, char *key);


