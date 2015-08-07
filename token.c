// Manmeet Singh - msingh11@ucsc.edu
// $Id: token.c,v 1.8 2013-05-16 15:14:31-07 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "debug.h"

#define INIT_CAPACITY 16

struct token {
   FILE *file;
   size_t capacity;
   size_t size;
   int token;
   char *buffer;
};

token *new_token (FILE *file) {
   token *this = malloc (sizeof (token));
   assert (this != NULL);
   this->file = file;
   this->capacity = INIT_CAPACITY;
   this->buffer = malloc (this->capacity);
   assert (this->buffer != NULL);
   this->buffer[0] = '\0';
   this->size = 0;
   this->token = 0;
   DEBUGS ('t', show_token (this));
   return this;
}

void free_token (token *this) {
   free (this->buffer);
   free (this);
}

char *peek_token (token *this) {
   DEBUGS ('t', show_token (this));
   return this->buffer;
}


void ensure_capacity (token *this, size_t capacity) {
   if (capacity > this->capacity) {
      size_t double_capacity = this->capacity * 2;
      this->capacity = capacity > double_capacity
                     ? capacity : double_capacity;
      this->buffer = realloc (this->buffer, this->capacity);
      assert (this->buffer);
   }
}

int scan_token (token *this) {
   this->size = 0;
   this->buffer[this->size] = '\0';
   int result = EOF;
   int nextchar = 0;
   do {
      nextchar = fgetc (this->file);
   } while (isspace (nextchar));
   if (nextchar == EOF) {
      result = EOF;
   }else if (nextchar == '_' || isdigit (nextchar)) {
      do {
         this->buffer[this->size++] = nextchar;
         ensure_capacity (this, this->size + 1);
         nextchar = fgetc (this->file);
      } while (isdigit (nextchar));
      this->buffer[this->size] = '\0';
      int ungetchar = ungetc (nextchar, this->file);
      assert (ungetchar == nextchar);
      result = NUMBER;
   }else {
      result = nextchar;
   }
   DEBUGS ('t', show_token (this));
   return result;
}

void show_token (token *this) {
   fprintf (stderr, "token@%p->{%lu,%lu,%d,%p->\"%s\"}\n",
            this, this->capacity, this->size, this->token,
            this->buffer, this->buffer);
}
