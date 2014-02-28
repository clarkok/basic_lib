#include <string.h>
#include "define.h"
#include "string.h"
#include "stdlib.h"

#define highist_one( x, res )   \
    {                           \
        __asm mov ebx, x        \
        __asm bsr eax, ebx      \
        __asm mov res, eax      \
    }

String *string () {
    String *s = malloc_string();
    s->_data = malloc( INIT_LENGTH );
    s->length = 0;
    s->capacity = INIT_LENGTH;
    s->unused = 0;

    *( s->_data ) = 0;

    return s;
}

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

String *string_from_string ( const String *l ) {
    String *s = malloc_string();

    s->length = l->length;
    s->capacity = l->capacity;
    s->_data = malloc( s->capacity );
    s->unused = 0;
    strcpy( s->_data, l->_data );

    return s;
}

void _string( String *s ) {
    free( s->_data );
    free( s );
}

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

String *string_clear ( String *s ) {
    s->length = 0;
    *(s->_data) = 0;

    return s;
}

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

String *string_swap ( String *s, String *p ) {
    String t = *s;
    *s = *p;
    *p = t;
    return s;
}

const char *string_c_str ( String *s ) {
    return s->_data;
}

String *string_copy ( String *s, u32 len, u32 pos ) {
    String *ret;

    if (pos > s->length) {
        return NULL;
    }

    ret = string_reserve( string_from_chars(""), len + 1 );

    if ( pos + len > s->length) {
        len = s->length - pos;
    }

    strncpy( ret->_data, s->_data + pos, len );

    ret->length = len;
    
    return ret;
}

u32 string_find_string ( String *s, const String *str, u32 pos, u32 n ) {
    return string_find_chars( s, str->_data, pos, n );
}

u32 string_find_chars ( String *s, const char *str, u32 pos, u32 n ) {
    u32 i;
    u32 l = s->length - n;

    for ( i=pos; i<l; i++ ) {
        if ( string_compare_chars(s, i, n, str) == 0 )
            return i;
    }
    return -1;
}

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

u32 string_rfind_string ( String *s, const String *str, u32 pos, u32 n ) {
    return string_rfind_chars( s, str->_data, pos, n );
}

u32 string_rfind_chars ( String *s, const char *str, u32 pos, u32 n ) {
    u32 i;

    for ( i=pos; i!=0; i-- ) {
        if ( string_compare_chars(s, i, n, str) == 0 )
            return i;
    }
    return ( string_compare_chars(s, 0, n, str)==0 ? 0 : -1 );
}

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

int string_compare_string ( String *s, u32 pos, u32 len, const String *c ) {
    return strncmp( s->_data + pos, c->_data, len );
}

int string_compare_chars ( String *s, u32 pos, u32 len, const char *c ) {
    return strncmp( s->_data + pos, c, len );
}

String *string_substr ( String *s, u32 pos, u32 len ) {
    return string_copy( s, len, pos );
}
