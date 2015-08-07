// Manmeet Singh - msingh11@ucsc.edu
// $Id: main.c,v 1.12 2015-02-03 18:26:19-08 - - $

#include <assert.h>
#include <ctype.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bigint.h"
#include "debug.h"
#include "stack.h"
#include "token.h"
#include "yyextern.h"

void do_push (stack *stack, char *numstr) {
   DEBUGF ('m', "stack=%p, numstr=%p=\"%s\"\n", stack, numstr, numstr);
   bigint *bigint = new_string_bigint (numstr);
   push_stack (stack, bigint);
}

void do_binop (stack *stack, bigint_binop binop) {
   DEBUGS ('m', show_stack (stack));
   bigint *right = pop_stack (stack);
   bigint *left = pop_stack (stack);
   if (right == NULL || left == NULL){
	   fprintf(stderr, "%s: The stack is empty\n", program_name);
   }else{
   bigint *answer = binop (left, right);
   push_stack (stack, answer);
   free_bigint (left);
   free_bigint (right);
   }
}

void do_clear (stack *stack) {
   DEBUGF ('m', "stack=%p\n", stack);
   while (! empty_stack (stack)) {
      bigint *bigint = pop_stack (stack);
      free_bigint (bigint);
   }
}

void do_print (stack *stack) {
   DEBUGS ('m', show_stack (stack));
   print_bigint (peek_stack (stack, 0));
}

void do_print_all (stack *stack) {
   DEBUGS ('m', show_stack (stack));
   int size = size_stack (stack);
   int index;
   for (index = 0; index < size; ++index) {
      print_bigint (peek_stack (stack, index));
   }
}

void unimplemented (int oper) {
   printf ("%s: ", program_name);
   if (isgraph (oper)) printf ("'%c' (0%o)", oper, oper);
                  else printf ("0%o", oper);
   printf (" unimplemented\n");
}

void scan_options (int argc, char **argv) {
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@': set_debug_flags (optarg);
                   break;
         default : printf ("%s: -%c: invalid option\n",
                           program_name, optopt);
                   break;
      }
   }
}

int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   scan_options (argc, argv);
   stack *stack = new_stack ();
   bool quit = false;
   yy_flex_debug = false;
   while (! quit) {
      int token = yylex();
      if (token == YYEOF) break;
      switch (token) {
         case NUMBER: do_push (stack, yytext); break;
         case '+': do_binop (stack, add_bigint); break;
         case '-': do_binop (stack, sub_bigint); break;
         case '*': do_binop (stack, mul_bigint); break;
         case 'c': do_clear (stack); break;
         case 'f': do_print_all (stack); break;
         case 'p': do_print (stack); break;
         case 'q': quit = true; break;
         default: unimplemented (token); break;
      }
   }
   yycleanup();
   DEBUGF ('m', "EXIT %d\n", exit_status);
   return EXIT_SUCCESS;
}

