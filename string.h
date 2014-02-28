/**
 * Module String
 * @author Clarkok
 *
 * The Lib defines the type of String which is same with C++ string.
 * All method of C++ string is rewritten here.
 */

#ifndef _STRING_H_
#define _STRING_H_

#include "define.h"

#define INIT_LENGTH     (16)

#define malloc_string()  (malloc_type(String))

/**
 * the class String
 * the size of it is 64byte
 */
typedef struct String {
    char *_data;
    u32 length;
    u32 capacity;
    u32 unused;
} String;

// Contructers
String *string ();
String *string_from_chars ( const char *p );
String *string_from_string ( const String *s );

// Destructers
void _string ( String *s );

// Capacity
String *string_reserve ( String *s, u32 n );
String *string_clear ( String *s );
#define string_empty( s )  ( !((s)->length) )

// Element Access
#define string_at( s, pos ) ( (s)->_data[pos] )

// Modifier
String *string_append_string ( String *s, const String *a );
String *string_append_chars ( String *s, const char *a );
String *string_push_back ( String *s, const char a );
String *string_assign_string ( String *s, const String *a );
String *string_assign_chars ( String *s, const char *a );
String *string_insert_string ( String *s, u32 pos, const String *i );
String *string_insert_chars ( String *s, u32 pos, const char *i );
String *string_erase ( String *s, u32 pos, u32 len );
String *string_replace_string ( String *s, u32 pos, u32 len, const String *r );
String *string_replace_chars ( String *s, u32 pos, u32 len, const char *r );
String *string_swap ( String *s, String *p );

// string operations
const char *string_c_str ( String *s );
String *string_copy ( String *s, u32 len, u32 pos );
u32 string_find_string ( String *s, const String *str, u32 pos, u32 n );
u32 string_find_chars ( String *s, const char *str, u32 pos, u32 n );
u32 string_find_char ( String *s, char c, u32 pos );
u32 string_rfind_string ( String *s, const String *str, u32 pos, u32 n );
u32 string_rfind_chars ( String *s, const char *str, u32 pos, u32 n );
u32 string_rfind_char ( String *s, char c , u32 pos );
int string_compare_chars ( String *s, u32 pos, u32 len, const char *c );
int string_compare_string ( String *s, u32 pos, u32 len, const String *c );
String *string_substr ( String *s, u32 pos, u32 len );

#endif // _STRING_H_
