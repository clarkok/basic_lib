#include <string.h>
#include "define.h"
#include "string.h"
#include "stdlib.h"

/**
 * To calc the highist one in x, and stored the index in res
 * @param int x the number to calc
 * @param u32 res the result
 */
#define highist_one( x, res )   \
    {                           \
        __asm mov ebx, x        \
        __asm bsr eax, ebx      \
        __asm mov res, eax      \
    }

/**
 * The constructer of String
 * @return String the new String
 */
String *string () {
    String *s = malloc_string();
    s->_data = malloc( INIT_LENGTH );
    s->length = 0;
    s->capacity = INIT_LENGTH;
    s->unused = 0;

    *( s->_data ) = 0;

    return s;
}

/**
 * The constructer of String from c-style string
 * @paran char *p the c-style string
 * @return String the new String
 */
String *string_from_chars ( const char *p ) {
    String *s = malloc_string();
    u32 len = strlen( p );
    u32 capacity = len + 1;

    highist_one( capacity, capacity );

    capacity = 1 << ( capacity + 1 );

    s->_data = malloc( capacity );
    s->length = len;
    s->capacity = capacity;
    s->unused = 0;

    strcpy( s->_data, p );

    return s;
}

/**
 * The constructer of String from an existing String
 * @param String *l the String
 * @return String the new String
 */
String *string_from_string ( const String *l ) {
    String *s = malloc_string();

    s->length = l->length;
    s->capacity = l->capacity;
    s->_data = malloc( s->capacity );
    s->unused = 0;
    strcpy( s->_data, l->_data );

    return s;
}

/**
 * The destructer of String
 * @param String *s the String to be destroyed
 */
void _string( String *s ) {
    free( s->_data );
    free( s );
}

/**
 * To expand the allocated memory to n, if n is larger than current capacity of the String
 * @param String *s the String under modifying
 * @param u32 n the new capacity
 * @return String the original String but after modifying
 */
String *string_reserve ( String *s, u32 n ) {
    int capacity;
    if ( n > s->capacity ) {
        capacity = n;
        highist_one( capacity, capacity );
        s->capacity = capacity = 1 << ( capacity + 1);
        realloc( s->_data, capacity );
    }
    return s;
}

/**
 * To clear a String, which means to make the length of the String 0
 * @param String *s the String under modifying
 * @return String the original String but after modifying
 */
String *string_clear ( String *s ) {
    s->length = 0;
    *(s->_data) = 0;

    return s;
}

/**
 * To append one String to another
 * @param String *s the String to be appended to
 * @param String *a the String to be appended to s
 * @return String the original String but after modifying
 */
String *string_append_string ( String *s, const String *a ) {
    u32 length = (s->length + a->length);
    u32 capacity = length + 1;
    
    highist_one( capacity, capacity );

    s->capacity = capacity = 1 << ( capacity + 1 );

    realloc( s->_data, capacity );

    strcpy( s->_data + s->length, a->_data );

    s->length = length;

    return s;
}

/**
 * To append one c-style to a String
 * @param String *s the String to be appended to
 * @param char *a the c-style string to be appended to s
 * @return String the original String but after modifying
 */
String *string_append_chars ( String *s, const char *a ) {
    u32 length = s->length + strlen( a );
    u32 capacity = length + 1;

    highist_one( capacity, capacity );

    s->capacity = capacity = 1 << ( capacity + 1 );

    realloc( s->_data, capacity );

    strcpy( s->_data + s->length, a );

    s->length = length;

    return s;
}

/**
 * To push a character in the end of a String
 * @param String *s the String to be pushed in
 * @param char a the character to be pushed
 * @return String the original String but after modifying
 */
String *string_push_back ( String *s, const char a ) {
    char *p;
    if ( ++(s->length) >= (s->capacity)) {
        s->capacity <<= 1;
        realloc( s->_data, s->capacity );
    }
    p = s->_data + s->length;
    *p-- = 0;
    *p = a;

    return s;
}

/**
 * To assign a String's content to another, but the two String will not be connected.
 * Each of them has it own buffer
 * @param String *s the String to be re-assigned
 * @param String *a the String to give its content
 * @return String the original String but after modifying
 */
String *string_assign_string ( String *s, const String *a ) {
    u32 capacity = a->length + 1;

    highist_one( capacity, capacity );

    s->capacity = capacity = 1 << ( capacity + 1 );

    s->length = a->length;
    free(s->_data);
    s->_data = malloc( s->capacity );
    strcpy( s->_data, a->_data );

    return s;
}

/**
 * To assign a c-style string's content to a String, but the two String will not be connected.
 * Each of them has it own buffer
 * @param String *s the String to be re-assigned
 * @param char *a the String to give its content
 * @return String the original String but after modifying
 */
String *string_assign_chars ( String *s, const char *a ) {
    u32 len = strlen( a );
    u32 capacity = len + 1;

    free( s->_data );

    highist_one( capacity, capacity );

    capacity = 1 << ( capacity + 1 );

    s->_data = malloc( capacity );
    s->length = len;
    s->capacity = capacity;
    s->unused = 0;

    strcpy( s->_data, a );

    return s;
}

/**
 * To insert a String into another
 * @param String *s the String to be inserted to
 * @param u32 pos the position the insertion will append at
 * @param String *i the String to be inserted with
 * @return String the original String but after modifying
 */
String *string_insert_string ( String *s, u32 pos, const String *i ) {
    u32 length = s->length + i->length;
    u32 capacity = length + 1;

    if (pos > s->length) {
        return NULL;
    }

    highist_one( capacity, capacity );

    s->capacity = capacity = 1 << ( capacity + 1 );

    realloc( s->_data, capacity );

    strcpy( s->_data + (pos + i->length), s->_data + pos );
    memcpy( s->_data + pos, i->_data, i->length );

    s->length = length;

    return s;
}

/**
 * To insert a c-style string into a String
 * @param String *s the String to be inserted to
 * @param u32 pos the position the insertion will append at
 * @param char *i the String to be inserted with
 * @return String the original String but after modifying
 */
String *string_insert_chars ( String *s, u32 pos, const char *i ) {
    u32 il = strlen(i);
    u32 length = s->length + il;
    u32 capacity = length + 1;

    if (pos > s->length) {
        return NULL;
    }

    highist_one( capacity, capacity );

    s->capacity = capacity = 1 << ( capacity + 1 );
    
    realloc( s->_data, capacity );

    strcpy( s->_data + (pos + il), s->_data + pos);
    memcpy( s->_data + pos, i, il );

    s->length = length;

    return s;
}

/**
 * To erase a substring in a String
 * @param String *s the String to deal with
 * @param u32 pos the beginning position of the erasing
 * @param u32 len the length of the erased substring
 * @return String the original String but after modifying
 */
String *string_erase ( String *s, u32 pos, u32 len ) {
    if ( !len || pos + len > s->length ) {
        s->_data[pos] = 0;
        s->length = pos;
        return s;
    }
    strcpy( s->_data + pos, s->_data + pos + len );
    s->length -= len;
    return s;
}

/**
 * To replace a substring in a String with another String
 * @param String *s the container of the substring to be replaced
 * @paran u32 pos the beginning position of the substring
 * @param u32 len the length of the substring
 * @param String *r the new String to replace the substring
 * @return String the original String but after modifying
 */
String *string_replace_string ( String *s, u32 pos, u32 len, const String *r ) {
    int capacity = s->length - len + r->length + 1;

    if ( pos > s->length )
        return NULL;

    if ( pos + len > s->length ) {
        len = s->length - pos;
    }

    if ( capacity > s->capacity ) {
        highist_one( capacity, capacity );
        s->capacity = capacity = 1 << ( capacity + 1 );
        realloc( s->_data, capacity );
    }

    strcpy( s->_data + pos + r->length, s->_data + pos + len );
    memcpy( s->_data + pos, r->_data, r->length );

    s->length -= len;
    s->length += r->length;

    return s;
}

/**
 * To replace a substring in a String with a c-style string
 * @param String *s the container of the substring to be replaced
 * @paran u32 pos the beginning position of the substring
 * @param u32 len the length of the substring
 * @param char *r the new c-style string to replace the substring
 * @return String the original String but after modifying
 */
String *string_replace_chars ( String *s, u32 pos, u32 len, const char *r ) {
    int rl = strlen(r);
    int capacity = s->length - len + rl + 1;

    if ( pos > s->length )
        return NULL;

    if ( pos + len > s->length ) {
        len = s->length - pos;
    }

    if ( capacity > s->capacity ) {
        highist_one( capacity, capacity );
        s->capacity = capacity = 1 << ( capacity + 1 );
        realloc( s->_data, capacity );
    }

    strcpy( s->_data + pos + rl, s->_data + pos + len );
    memcpy( s->_data + pos, r, rl );

    s->length -= len;
    s->length += rl;

    return s;
}

/**
 * To swap the two String
 * @param String *s one of them
 * @param String *p the other of them
 * @return String *p
 */
String *string_swap ( String *s, String *p ) {
    String t = *s;
    *s = *p;
    *p = t;
    return s;
}

/**
 * To get the c-style string of a String
 * @param String* the String
 * @return char* the c-style string
 */
const char *string_c_str ( String *s ) {
    return s->_data;
}

/**
 * To copy a substring of a String
 * @param String *s the original String
 * @param u32 len the length of the substring, NOTE: I know it is strange, but it is same with C++
 * @param u32 pos the beginning position of the substring
 * @return String* the copied String
 */
String *string_copy ( String *s, u32 len, u32 pos ) {
    String *ret;

    if (pos > s->length) {
        return NULL;
    }

    ret = string_reserve( string(), len + 1 );

    if ( pos + len > s->length) {
        len = s->length - pos;
    }

    strncpy( ret->_data, s->_data + pos, len );

    ret->length = len;
    
    return ret;
}

/**
 * Find String in a String
 * @param String *s the String to be searched
 * @param String *str the target String
 * @param u32 pos the first position to search
 * @param u32 n the max length of the Strings to compare
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_find_string ( String *s, const String *str, u32 pos, u32 n ) {
    return string_find_chars( s, str->_data, pos, n );
}

/**
 * Find c-style string in a String
 * @param String *s the String to be searched
 * @param char *str the target String
 * @param u32 pos the first position to search
 * @param u32 n the max length of the Strings to compare
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_find_chars ( String *s, const char *str, u32 pos, u32 n ) {
    u32 i;
    u32 l = s->length - n;

    for ( i=pos; i<l; i++ ) {
        if ( string_compare_chars(s, i, n, str) == 0 )
            return i;
    }
    return -1;
}

/**
 * Find a character in a String
 * @param String *s the String to be searched
 * @param char c the target character 
 * @param u32 pos the first position to be searched
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_find_char ( String *s, char c, u32 pos ) {
    char *p = s->_data + pos;
    char *end = s->_data + s->length;

    if ( pos > s->length ) {
        return -1;
    }

    for ( ; p!=end; p++) {
        if ( *p==c ) {
            return (p-s->_data);
        }
    }
    return -1;
}

/**
 * Find String in a String, reversely
 * @param String *s the String to be searched
 * @param String *str the target String
 * @param u32 pos the first position to search
 * @param u32 n the max length of the Strings to compare
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_rfind_string ( String *s, const String *str, u32 pos, u32 n ) {
    return string_rfind_chars( s, str->_data, pos, n );
}

/**
 * Find c-style string in a String, reversely
 * @param String *s the String to be searched
 * @param char *str the target String
 * @param u32 pos the first position to search
 * @param u32 n the max length of the Strings to compare
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_rfind_chars ( String *s, const char *str, u32 pos, u32 n ) {
    u32 i;

    for ( i=pos; i!=0; i-- ) {
        if ( string_compare_chars(s, i, n, str) == 0 )
            return i;
    }
    return ( string_compare_chars(s, 0, n, str)==0 ? 0 : -1 );
}

/**
 * Find a character in a String, reversely
 * @param String *s the String to be searched
 * @param char c the target character 
 * @param u32 pos the first position to be searched
 * @return u32 the index of the found String, if none is found, -1 is returned
 */
u32 string_rfind_char ( String *s, char c, u32 pos ) {
    char *p = s->_data + pos;
    char *begin = s->_data;

    if ( pos > s->length ) {
        return -1;
    }

    for ( ; p!=begin; p--) {
        if ( *p==c ) {
            return (p - s->_data);
        }
    }
    return ( *p==c ? 0 : -1);
}

/**
 * Compare two String
 * @param String *s one of them
 * @param u32 pos the beginning position of the comparation
 * @param u32 len the max length to be compared, when set s->length + 1, the method will work as C++ s.compare(c)
 * @param String *c the other of them
 * @return if positive, then s > c, if negetive, the s < c, if zero, the s = c
 */
int string_compare_string ( String *s, u32 pos, u32 len, const String *c ) {
    return strncmp( s->_data + pos, c->_data, len );
}

/**
 * Compare a String and a c-style string
 * @param String *s one of them
 * @param u32 pos the beginning position of the comparation
 * @param u32 len the max length to be compared, when set s->length + 1, the method will work as C++ s.compare(c)
 * @param char *c the other of them
 * @return if positive, then s > c, if negetive, the s < c, if zero, the s = c
 */
int string_compare_chars ( String *s, u32 pos, u32 len, const char *c ) {
    return strncmp( s->_data + pos, c, len );
}

/**
 * Get the substring of a String, almost same as copy but with different arguments order
 * @param String *s the original String
 * @param u32 pos the beginning position of the substring
 * @param u32 len the length of the substring, NOTE: I know it is strange, but it is same with C++
 * @return String* the copied String
 */
String *string_substr ( String *s, u32 pos, u32 len ) {
    return string_copy( s, len, pos );
}
