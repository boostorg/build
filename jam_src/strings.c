#include "strings.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef NDEBUG
static void assert_invariants( string* self )
{
    assert( self->size < self->capacity );
    assert( ( self->capacity <= sizeof(self->opt) ) == ( self->value == self->opt ) );
    assert( strlen( self->value ) == self->size );
}
#else
# define assert_invariants(x) do {} while (0)
#endif

void string_new( string* s )
{
    s->value = s->opt;
    s->size = 0;
    s->capacity = sizeof(s->opt);
    s->opt[0] = 0;
    assert_invariants( s );
}

void string_free( string* s )
{
    assert_invariants( s );
    if ( s->value != s->opt )
        free( s->value );
}

static void string_reserve_internal( string* self, size_t capacity )
{
    if ( self->value == self->opt )
    {
        self->value = malloc( capacity );
        self->value[0] = 0;
        strcat( self->value, self->opt );
    }
    else
    {
        self->value = realloc( self->value, capacity );
    }
    self->capacity = capacity;
}

void string_reserve( string* self, size_t capacity )
{
    assert_invariants( self );
    if ( capacity <= self->capacity )
        return;
    string_reserve_internal( self, capacity );
    assert_invariants( self );
}

static void extend_full( string* self, char* start, char *finish )
{
    size_t new_size = self->capacity + ( finish - start );
    size_t new_capacity = self->capacity;
    size_t old_size = self->capacity;
    while ( new_capacity < new_size + 1)
        new_capacity <<= 1;
    string_reserve_internal( self, new_capacity );
    memcpy( self->value + old_size, start, new_size - old_size );
    self->value[new_size] = 0;
    self->size = new_size;
}

void string_append( string* self, char* rhs )
{
    char* p = self->value + self->size;
    char* end = self->value + self->capacity;
    assert_invariants( self );
    
    while ( *rhs && p != end)
        *p++ = *rhs++;
    
    if ( p != end )
    {
        *p = 0;
        self->size = p - self->value;
    }
    else
    {
        extend_full( self, rhs, rhs + strlen(rhs) );
    }
    assert_invariants( self );
}

void string_append_range( string* self, char* start, char* finish )
{
    char* p = self->value + self->size;
    char* end = self->value + self->capacity;
    assert_invariants( self );
    
    while ( p != end && start != finish )
        *p++ = *start++;
    
    if ( p != end )
    {
        *p = 0;
        self->size = p - self->value;
    }
    else
    {
        extend_full( self, start, finish );
    }
    assert_invariants( self );
}

void string_copy( string* s, char* rhs )
{
    string_new( s );
    string_append( s, rhs );
}

void string_truncate( string* self, size_t n )
{
    assert_invariants( self );
    assert( n <= self->capacity );
    self->value[self->size = n] = 0;
    assert_invariants( self );
}

void string_pop_back( string* self )
{
    string_truncate( self, self->size - 1 );
}

void string_push_back( string* self, char x )
{
    assert_invariants( self );
    string_append_range( self, &x, &x + 1 );
    assert_invariants( self );
}

char string_back( string* self )
{
    assert_invariants( self );
    return self->value[self->size - 1];
}
