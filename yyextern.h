// Manmeet Singh - msingh11@ucsc.edu
// $Id: yyextern.h,v 1.1 2015-02-03 18:11:58-08 - - $

#ifndef __YYEXTERN_H__
#define __YYEXTERN_H__

//
// DESCRIPTION
//    Definitions of external names used by flex-generated code.
//

#define YYEOF 0
#define YYNUMBER 256

extern char *yytext;          // Pointer to the string that was found
extern int yy_flex_debug;     // yylex's verbose tracing flag
extern int yylex (void);      // Read next word from opened file yyin
extern void yycleanup (void); // Cleans up flex's buffers when done

#endif
