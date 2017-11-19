/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: I
 * Subor:    parser.h
 * Datum:    8. 10. 2017
 * Autori:   David Ling      <xlingd00@stud.fit.vutbr.cz>,
 *           David Zak       <xzakda02@stud.fit.vutbr.cz>,
 *           Michal Beranek  <xberan38@stud.fit.vutbr.cz>
 */


 /* ---------- TODO------------*/
 /**
 * Deklarace funkci použitých při syntaktické analýze.
 */

 #pragma once

 #include "errorcodes.h"
 #include "symtable.h"
 #include "tags.h"
 #include "scanner.h"

 int parser();
 int scope();
 int all();
 int funcDefine();
 int funcDeclare();
 int params();
 int nextParams();
 int checkParams();
 int checkNextParams();
 int endFunc();
 int assignment();
 int input();
 int whileCycle();
 int ifStatement();
 int freturn();
 int body(bool isFunc);
