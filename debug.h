// Manmeet Singh - msingh11@ucsc.edu
// $Id: debug.h,v 1.5 2014-01-24 18:33:47-08 - - $

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdbool.h>

//
// DESCRIPTION
//    Debugging library containing miscellaneous useful things.
//

//
// Program name and exit status.
//
extern char *program_name;
extern int exit_status;

//
// Support for STUB statements.
//
#define STUB(STMT) STMT

//
// Sets a string of debug flags to be used by DEBUGF and DEBUGS.
// If a particular debug flag has been set, messages are printed.
// The flag "@" turns on all flags.
//
void set_debug_flags (char *flags);

//
// Check if a debug flag is set.
//
bool get_debug_flag (char flag);

//
// DEBUGF takes printf-like arguments.
// DEBUGS takes any fprintf(stderr...) statement as an argument.
//
#define DEBUGF(FLAG,...) \
        if (get_debug_flag (FLAG)) { \
           __show_debug (FLAG, __FILE__, __LINE__, __func__); \
           fprintf (stderr, __VA_ARGS__); \
           fflush (NULL); \
        }
#define DEBUGS(FLAG,STMT) \
        if (get_debug_flag (FLAG)) { \
           __show_debug (FLAG, __FILE__, __LINE__, __func__); \
           STMT; \
           fflush (NULL); \
        }
void __show_debug (char flag, char *file, int line, const char *func);

#endif
