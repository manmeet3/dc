%{
// $Id: scanner.l,v 1.2 2015-02-03 18:22:49-08 - - $

#include "yyextern.h"
#define YY_NO_INPUT

%}

%option 8bit
%option debug
%option interactive
%option nodefault
%option nounput
%option noyywrap

WHITESPACE  ([ \t\n])
NUMBER      (_?[0-9]*)
OTHER       (.)

%%

[ \t\n]+  { }
_?[0-9]*  { return YYNUMBER; }
.         { return *yytext; }

%%

void yycleanup (void) {
   yy_delete_buffer (YY_CURRENT_BUFFER);
}

