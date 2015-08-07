// Manmeet Singh - msingh11@ucsc.edu
// $Id: bigint.c,v 1.15 2015-02-03 18:11:58-08 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *cmp( bigint *this, bigint* that){
	if (this->size == that->size){
		int i;
		for(i=0; i<this->size; ++i){
			return (this->digits[i] > that->digits[i] ? this:that);
		}
	}
	else if(this -> size > that->size) return this;
	else return that;

	return NULL;
}

void normal_print (bigint *this){
	if(this->size == 0) printf ("0\n");
	else{
	if (this->negative) printf("-");
	char *byte;
	   for (byte = &this->digits[this->size - 1];
	        byte >= this->digits; --byte) {
	      printf ("%d", *byte);
	   }
	   printf ("\n");
	}

}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}


bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

static bigint *do_add (bigint *this, bigint *that) {
	int digit = 0;
	int carry = 0;
	 bigint *ans = new_bigint(cmp(this,that) -> capacity + 1);
	 //printf("%i", ans->capacity);
	 int index;
	for (index=0; index<cmp(this,that)->capacity; index++){
		digit = this->digits[index] + that->digits[index] + carry;
		ans->digits[index] = digit %10;
		carry = digit/10;
		//printf("%i", carry);
		ans->size++;

	}
	trim_zeros(ans);
	return ans;
}

static bigint *do_sub (bigint *this, bigint *that) {
	int borrow = 0;
	int digit = 0;
	bigint *ans = new_bigint((this->size > that->size ? this:that) -> capacity + 1);
	ans->size = this->size;
	//printf("%i", ans->size);
	int index;
	for (index=0; index<(this->size > that->size ? this:that)->size; ++index){
		digit = this->digits[index] - that->digits[index] - borrow + 10;
		ans->digits[index] = digit % 10;
		//printf("%i", ans->digits[index]);
		borrow = 1 - digit/10;
}
	trim_zeros(ans);
		return ans;
}
void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this) {
   //DEBUGS ('b', show_bigint (this));
   //show_bigint(this); //added --- stack functional
	if (this == NULL) fprintf(stderr, "%s: The stack is empty\n", program_name);
	else normal_print(this);
}

bigint *add_bigint (bigint *this, bigint *that) {
	bigint* ans;
	if ((this->negative && that->negative) ||
			(!this->negative && !that->negative)){
		ans = do_add(this,that);
		ans->negative = this -> negative;
	}
	else {
		bigint* tmp = cmp(this,that);
		bigint* tmp2;
		if(tmp == this){
			tmp2 = that;
		}else{
			tmp2 = this;
		}
			ans = do_sub(tmp,tmp2);
			ans->negative = tmp->negative;

		/* PREV ATTEMPT*********
		 * printf("%i _ big",(this->size > that->size ? this:that));
		printf("%i _ sm",(this->size < that->size ? this:that));

		ans = do_sub((this->size > that->size ? this:that),
				(this->size < that->size ? this:that));
		ans->negative = (this->size > that->size ? this:that)->negative;

		if(this->size > that->size){
					ans = do_sub(this,that);
					ans->negative = this->negative;
				}else{
					ans = do_sub(that,this);
					ans->negative = that->negative;
				}

		*/
	}
   return ans;
}

bigint *sub_bigint (bigint *this, bigint *that) {
	bigint* ans;
	if((that->negative != this->negative)){
		ans = do_add (this, that);
	}else{
		bigint* tmp = cmp(this,that);
				bigint* tmp2;
				if(tmp == this){
					tmp2 = that;
				}else{
					tmp2 = this;
				}
					ans = do_sub(tmp,tmp2);
					//printf("%i - first", ans->negative);
					if(this->digits[0]<that->digits[0])
						ans->negative = true;
					//printf("%i - sec", ans->negative);

	}
   return ans;
}


bigint *mul_bigint (bigint *this, bigint *that) {
	   bigint *ans = new_bigint(this->size + that->size +1);
	   //int maxCap = final->capacity;
	   int i;
	   for (i = 0; i < ans->capacity; i++) {
		   ans->digits[i] = 0;
	   }
	   int digit = 0;
	   int carry = 0;
	   int j;
	   int k;
	   int size_this = this->size;
	   int size_that = that->size;
	   for (j = 0; j < size_this; j++) {
	      for (k = 0; k < size_that; k++) {
	         digit = ans->digits[k+j] +
	                 this->digits[j] * that->digits[k] + carry;
	         ans->digits[j+k] = digit % 10;
	         carry = digit / 10;
	      }
	      ans->digits[j+that->size] = carry;
	   }
	   ans->size = ans->capacity;
	   if(this->negative != that->negative)
	  		   ans->negative = true;
	   trim_zeros(ans);
	   return ans;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   char *byte;
   for (byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

