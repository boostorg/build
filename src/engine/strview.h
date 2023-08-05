#ifndef BOOST_CORE_STRING_VIEW_HPP_INCLUDED
#define BOOST_CORE_STRING_VIEW_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// boost::core::basic_string_view<Ch>
//
// Copyright 2021 Peter Dimov
// Copyright 2023 René Ferdinand Rivera Morell
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <string>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cstring>
#include <climits>
#include <iosfwd>
#include <ios>
#include <type_traits>
#include <cassert>

namespace boost
{

namespace core
{
namespace detail
{

template<class Ch> struct sv_to_uchar
{
    typedef Ch type;
};

template<> struct sv_to_uchar<char>
{
    typedef unsigned char type;
};

#if defined(__GNUC__) && __GNUC__ * 100 + __GNUC_MINOR__ >= 406
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wtype-limits"
#endif

template<class Ch> std::size_t find_first_of( Ch const* p_, std::size_t n_, Ch const* s, std::size_t pos, std::size_t n ) noexcept
{
    typedef typename sv_to_uchar<Ch>::type UCh;

    unsigned char table[ 256 ] = {};

    bool use_table = true;

    for( std::size_t j = 0; j < n; ++j )
    {
        UCh ch = s[ j ];

        if( ch >= 0 && ch < 256 )
        {
            table[ ch ] = 1;
        }
        else
        {
            use_table = false;
            break;
        }
    }

    if( use_table )
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            UCh ch = p_[ i ];
            if( ch >= 0 && ch < 256 && table[ ch ] ) return i;
        }
    }
    else if( n >= 16 )
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            Ch ch = p_[ i ];
            if( std::char_traits<Ch>::find( s, n, ch ) != 0 ) return i;
        }
    }
    else
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            Ch ch = p_[ i ];

            for( std::size_t j = 0; j < n; ++j )
            {
                if( s[ j ] == ch ) return i;
            }
        }
    }

    return static_cast<std::size_t>( -1 );
}

template<class Ch> std::size_t find_last_of( Ch const* p_, Ch const* s, std::size_t pos, std::size_t n ) noexcept
{
    typedef typename sv_to_uchar<Ch>::type UCh;

    unsigned char table[ 256 ] = {};

    bool use_table = true;

    for( std::size_t j = 0; j < n; ++j )
    {
        UCh ch = s[ j ];

        if( ch >= 0 && ch < 256 )
        {
            table[ ch ] = 1;
        }
        else
        {
            use_table = false;
            break;
        }
    }

    std::size_t const npos = static_cast< std::size_t >( -1 );

    std::size_t i = pos;

    if( use_table )
    {
        do
        {
            UCh ch = p_[ i ];

            if( ch >= 0 && ch < 256 && table[ ch ] ) return i;

            --i;
        }
        while( i != npos );
    }
    else if( n >= 16 )
    {
        do
        {
            Ch ch = p_[ i ];

            if( std::char_traits<Ch>::find( s, n, ch ) != 0 ) return i;

            --i;
        }
        while( i != npos );
    }
    else
    {
        do
        {
            Ch ch = p_[ i ];

            for( std::size_t j = 0; j < n; ++j )
            {
                if( s[ j ] == ch ) return i;
            }

            --i;
        }
        while( i != npos );
    }

    return npos;
}

template<class Ch> std::size_t find_first_not_of( Ch const* p_, std::size_t n_, Ch const* s, std::size_t pos, std::size_t n ) noexcept
{
    typedef typename sv_to_uchar<Ch>::type UCh;

    unsigned char table[ 256 ] = {};

    bool use_table = true;

    for( std::size_t j = 0; j < n; ++j )
    {
        UCh ch = s[ j ];

        if( ch >= 0 && ch < 256 )
        {
            table[ ch ] = 1;
        }
        else
        {
            use_table = false;
            break;
        }
    }

    if( use_table )
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            UCh ch = p_[ i ];
            if( !( ch >= 0 && ch < 256 && table[ ch ] ) ) return i;
        }
    }
    else if( n >= 16 )
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            Ch ch = p_[ i ];
            if( std::char_traits<Ch>::find( s, n, ch ) == 0 ) return i;
        }
    }
    else
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            Ch ch = p_[ i ];

            bool r = false;

            for( std::size_t j = 0; j < n; ++j )
            {
                if( s[ j ] == ch )
                {
                    r = true;
                    break;
                }
            }

            if( !r ) return i;
        }
    }

    return static_cast<std::size_t>( -1 );
}

template<class Ch> std::size_t find_last_not_of( Ch const* p_, Ch const* s, std::size_t pos, std::size_t n ) noexcept
{
    typedef typename sv_to_uchar<Ch>::type UCh;

    unsigned char table[ 256 ] = {};

    bool use_table = true;

    for( std::size_t j = 0; j < n; ++j )
    {
        UCh ch = s[ j ];

        if( ch >= 0 && ch < 256 )
        {
            table[ ch ] = 1;
        }
        else
        {
            use_table = false;
            break;
        }
    }

    std::size_t const npos = static_cast< std::size_t >( -1 );

    std::size_t i = pos;

    if( use_table )
    {
        do
        {
            UCh ch = p_[ i ];

            if( !( ch >= 0 && ch < 256 && table[ ch ] ) ) return i;

            --i;
        }
        while( i != npos );
    }
    else if( n >= 16 )
    {
        do
        {
            Ch ch = p_[ i ];

            if( std::char_traits<Ch>::find( s, n, ch ) == 0 ) return i;

            --i;
        }
        while( i != npos );
    }
    else
    {
        do
        {
            Ch ch = p_[ i ];

            bool r = false;

            for( std::size_t j = 0; j < n; ++j )
            {
                if( s[ j ] == ch )
                {
                    r = true;
                    break;
                }
            }

            if( !r ) return i;

            --i;
        }
        while( i != npos );
    }

    return npos;
}

#if defined(__GNUC__) && __GNUC__ * 100 + __GNUC_MINOR__ >= 406
# pragma GCC diagnostic pop
#endif

} // namespace detail

template<class Ch> class basic_string_view
{
private:

    Ch const* p_;
    mutable std::size_t n_; // mutable to allow mod within C++11 constexpr rules

public:

    // types

    typedef std::char_traits<Ch> traits_type;
    typedef Ch value_type;
    typedef Ch* pointer;
    typedef Ch const* const_pointer;
    typedef Ch& reference;
    typedef Ch const& const_reference;
    typedef Ch const* const_iterator;
    typedef const_iterator iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef const_reverse_iterator reverse_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // npos

    static constexpr size_type npos = static_cast<size_type>( -1 );

public:

    // construction and assignment

    constexpr basic_string_view() noexcept: p_(), n_()
    {
    }

    constexpr basic_string_view( Ch const* str ) noexcept: p_( str ), n_( traits_type::length( str ) )
    {
    }

    constexpr basic_string_view( Ch const* str, size_type len ) noexcept: p_( str ), n_( len )
    {
    }

    template<class End> constexpr basic_string_view( Ch const* first, End last,
        typename std::enable_if<std::is_same<End, Ch const*>::value, void >::type* = 0 ) noexcept: p_( first ), n_( last - first )
    {
    }

    template<class A> basic_string_view( std::basic_string<Ch, std::char_traits<Ch>, A> const& str ) noexcept: p_( str.data() ), n_( str.size() )
    {
    }

    basic_string_view( std::nullptr_t ) = delete;

    // constexpr basic_string_view& operator=( basic_string_view const& ) noexcept & = default;

    // conversions

    template<class A> operator std::basic_string<Ch, std::char_traits<Ch>, A>() const
    {
        return std::basic_string<Ch, std::char_traits<Ch>, A>( data(), size() );
    }

    // iterator support

    constexpr const_iterator begin() const noexcept
    {
        return p_;
    }

    constexpr const_iterator end() const noexcept
    {
        return p_ + n_;
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return p_;
    }

    constexpr const_iterator cend() const noexcept
    {
        return p_ + n_;
    }

    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator( end() );
    }

    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator( begin() );
    }

    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator( end() );
    }

    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator( begin() );
    }

    // capacity

    constexpr size_type size() const noexcept
    {
        return n_;
    }

    constexpr size_type length() const noexcept
    {
        return n_;
    }

    constexpr size_type max_size() const noexcept
    {
        return npos / sizeof( Ch );
    }

    constexpr bool empty() const noexcept
    {
        return n_ == 0;
    }

    // element access

    const_reference operator[]( size_type pos ) const noexcept
    {
        assert( pos < size() );
        return p_[ pos ];
    }

    const_reference at( size_type pos ) const
    {
        if( pos >= size() )
        {
            throw std::out_of_range( "basic_string_view::at" );
        }

        return p_[ pos ];
    }

    const_reference front() const noexcept
    {
        assert( !empty() );
        return p_[ 0 ];
    }

    const_reference back() const noexcept
    {
        assert( !empty() );
        return p_[ n_ - 1 ];
    }

    constexpr const_pointer data() const noexcept
    {
        return p_;
    }

    // modifiers

    void remove_prefix( size_type n ) noexcept
    {
        assert( n <= size() );

        p_ += n;
        n_ -= n;
    }

    void remove_suffix( size_type n ) noexcept
    {
        assert( n <= size() );

        n_ -= n;
    }

    void swap( basic_string_view& s ) noexcept
    {
        std::swap( p_, s.p_ );
        std::swap( n_, s.n_ );
    }

    // string operations

    size_type copy( Ch* s, size_type n, size_type pos = 0 ) const
    {
        if( pos > size() )
        {
            throw std::out_of_range( "basic_string_view::copy" );
        }

        std::size_t rlen = (std::min)( n, size() - pos );

        traits_type::copy( s, data() + pos, rlen );

        return rlen;
    }

    basic_string_view substr( size_type pos = 0, size_type n = npos ) const
    {
        if( pos > size() )
        {
            throw std::out_of_range( "basic_string_view::substr" );
        }

        std::size_t rlen = (std::min)( n, size() - pos );

        return basic_string_view( data() + pos, rlen );
    }

    // compare

    int compare( basic_string_view str ) const noexcept
    {
        std::size_t rlen = (std::min)( size(), str.size() );

        int cmp = traits_type::compare( data(), str.data(), rlen );

        if( cmp != 0 ) return cmp;

        if( size() == str.size() ) return 0;

        return size() < str.size()? -1: +1;
    }

    constexpr int compare( size_type pos1, size_type n1, basic_string_view str ) const
    {
        return substr( pos1, n1 ).compare( str );
    }

    constexpr int compare( size_type pos1, size_type n1, basic_string_view str, size_type pos2, size_type n2 ) const
    {
        return substr( pos1, n1 ).compare( str.substr( pos2, n2 ) );
    }

    constexpr int compare( Ch const* s ) const noexcept
    {
        return compare( basic_string_view( s ) );
    }

    constexpr int compare( size_type pos1, size_type n1, Ch const* s ) const
    {
        return substr( pos1, n1 ).compare( basic_string_view( s ) );
    }

    constexpr int compare( size_type pos1, size_type n1, Ch const* s, size_type n2 ) const
    {
        return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) );
    }

    // starts_with

    constexpr bool starts_with( basic_string_view x ) const noexcept
    {
        return size() >= x.size() && traits_type::compare( data(), x.data(), x.size() ) == 0;
    }

    constexpr bool starts_with( Ch x ) const noexcept
    {
        return !empty() && front() == x;
    }

    constexpr bool starts_with( Ch const* x ) const noexcept
    {
        return starts_with( basic_string_view( x ) );
    }

    // ends_with

    constexpr bool ends_with( basic_string_view x ) const noexcept
    {
        return size() >= x.size() && traits_type::compare( data() + size() - x.size(), x.data(), x.size() ) == 0;
    }

    constexpr bool ends_with( Ch x ) const noexcept
    {
        return !empty() && back() == x;
    }

    constexpr bool ends_with( Ch const* x ) const noexcept
    {
        return ends_with( basic_string_view( x ) );
    }

    // find

    constexpr size_type find( basic_string_view str, size_type pos = 0 ) const noexcept
    {
        return find( str.data(), pos, str.size() );
    }

    size_type find( Ch c, size_type pos = 0 ) const noexcept
    {
        if( pos >= size() ) return npos;

        Ch const* r = traits_type::find( data() + pos, size() - pos, c );

        return r? r - data(): npos;
    }

    size_type find( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( n == 1 ) return find( s[0], pos );

        if( pos + n > size() ) return npos;
        if( n == 0 ) return pos;

        Ch const* p = data() + pos;
        Ch const* last = data() + size() - n + 1;

        for( ;; )
        {
            p = traits_type::find( p, last - p, s[0] );

            if( p == 0 ) break;

            if( traits_type::compare( p + 1, s + 1, n - 1 ) == 0 ) return p - data();

            ++p;
        }

        return npos;
    }

    constexpr size_type find( Ch const* s, size_type pos = 0 ) const noexcept
    {
        return find( s, pos, traits_type::length( s ) );
    }

    // rfind

    constexpr size_type rfind( basic_string_view str, size_type pos = npos ) const noexcept
    {
        return rfind( str.data(), pos, str.size() );
    }

    size_type rfind( Ch c, size_type pos = npos ) const noexcept
    {
        size_type n = size();

        if( n == 0 )
        {
            return npos;
        }

        if( pos > n - 1 )
        {
            pos = n - 1;
        }

        do
        {
            if( p_[ pos ] == c ) return pos;
            --pos;
        }
        while( pos != npos );

        return npos;
    }

    size_type rfind( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( n > size() ) return npos;

        if( pos > size() - n )
        {
            pos = size() - n;
        }

        if( n == 0 ) return pos;

        for( ;; )
        {
            size_type xpos = rfind( s[0], pos );

            if( xpos == npos ) return npos;

            if( traits_type::compare( data() + xpos, s, n ) == 0 ) return xpos;

            if( xpos == 0 ) return npos;

            pos = xpos - 1;
        }
    }

    constexpr size_type rfind( Ch const* s, size_type pos = npos ) const noexcept
    {
        return rfind( s, pos, traits_type::length( s ) );
    }

    // find_first_of

    constexpr size_type find_first_of( basic_string_view str, size_type pos = 0 ) const noexcept
    {
        return find_first_of( str.data(), pos, str.size() );
    }

    constexpr size_type find_first_of( Ch c, size_type pos = 0 ) const noexcept
    {
        return find( c, pos );
    }

    size_type find_first_of( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( n == 0 || pos >= size() ) return npos;
        if( n == 1 ) return find( s[0], pos );

        return detail::find_first_of( data(), size(), s, pos, n );
    }

    constexpr size_type find_first_of( Ch const* s, size_type pos = 0 ) const noexcept
    {
        return find_first_of( s, pos, traits_type::length( s ) );
    }

    // find_last_of

    constexpr size_type find_last_of( basic_string_view str, size_type pos = npos ) const noexcept
    {
        return find_last_of( str.data(), pos, str.size() );
    }

    constexpr size_type find_last_of( Ch c, size_type pos = npos ) const noexcept
    {
        return rfind( c, pos );
    }

    size_type find_last_of( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( n == 1 )
        {
            return rfind( s[0], pos );
        }

        size_type m = size();

        if( m == 0 )
        {
            return npos;
        }

        if( pos > m - 1 )
        {
            pos = m - 1;
        }

        return detail::find_last_of( data(), s, pos, n );
    }

    constexpr size_type find_last_of( Ch const* s, size_type pos = npos ) const noexcept
    {
        return find_last_of( s, pos, traits_type::length( s ) );
    }

    // find_first_not_of

    constexpr size_type find_first_not_of( basic_string_view str, size_type pos = 0 ) const noexcept
    {
        return find_first_not_of( str.data(), pos, str.size() );
    }

    size_type find_first_not_of( Ch c, size_type pos = 0 ) const noexcept
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            if( p_[ i ] != c ) return i;
        }

        return npos;
    }

    size_type find_first_not_of( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( pos >= size() ) return npos;
        if( n == 1 ) return find_first_not_of( s[0], pos );

        return detail::find_first_not_of( data(), size(), s, pos, n );
    }

    constexpr size_type find_first_not_of( Ch const* s, size_type pos = 0 ) const noexcept
    {
        return find_first_not_of( s, pos, traits_type::length( s ) );
    }

    // find_last_not_of

    constexpr size_type find_last_not_of( basic_string_view str, size_type pos = npos ) const noexcept
    {
        return find_last_not_of( str.data(), pos, str.size() );
    }

    size_type find_last_not_of( Ch c, size_type pos = npos ) const noexcept
    {
        size_type m = size();

        if( m == 0 )
        {
            return npos;
        }

        if( pos > m - 1 )
        {
            pos = m - 1;
        }

        do
        {
            if( p_[ pos ] != c ) return pos;
            --pos;
        }
        while( pos != npos );

        return npos;
    }

    size_type find_last_not_of( Ch const* s, size_type pos, size_type n ) const noexcept
    {
        if( n == 1 )
        {
            return find_last_not_of( s[0], pos );
        }

        size_type m = size();

        if( m == 0 )
        {
            return npos;
        }

        if( pos > m - 1 )
        {
            pos = m - 1;
        }

        return detail::find_last_not_of( data(), s, pos, n );
    }

    constexpr size_type find_last_not_of( Ch const* s, size_type pos = npos ) const noexcept
    {
        return find_last_not_of( s, pos, traits_type::length( s ) );
    }

    // contains

    constexpr bool contains( basic_string_view sv ) const noexcept
    {
        return find( sv ) != npos;
    }

    bool contains( Ch c ) const noexcept
    {
        Ch const* p = data();
        size_type n = size();

        if( n >= 16 )
        {
            return traits_type::find( p, n, c ) != 0;
        }
        else
        {
            for( size_type i = 0; i < n; ++i )
            {
                if( p[ i ] == c ) return true;
            }

            return false;
        }
    }

    constexpr bool contains( Ch const* s ) const noexcept
    {
        return find( s ) != npos;
    }

    // relational operators

    constexpr friend bool operator==( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return sv1.size() == sv2.size() && traits_type::compare( sv1.data(), sv2.data(), sv1.size() ) == 0;
    }

    constexpr friend bool operator!=( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return !( sv1 == sv2 );
    }

    constexpr friend bool operator<( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return sv1.compare( sv2 ) < 0;
    }

    constexpr friend bool operator<=( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return sv1.compare( sv2 ) <= 0;
    }

    constexpr friend bool operator>( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return sv1.compare( sv2 ) > 0;
    }

    constexpr friend bool operator>=( basic_string_view sv1, basic_string_view sv2 ) noexcept
    {
        return sv1.compare( sv2 ) >= 0;
    }
};

// stream inserter

template<class Ch> std::basic_ostream<Ch>& operator<<( std::basic_ostream<Ch>& os, basic_string_view<Ch> str )
{
    Ch const* p = str.data();
    std::streamsize n = str.size();

    std::streamsize m = os.width();

    if( n >= m )
    {
        os.write( p, n );
    }
    else if( ( os.flags() & std::ios_base::adjustfield ) == std::ios_base::left )
    {
        os.write( p, n );

        os.width( m - n );
        os << "";
    }
    else
    {
        os.width( m - n );
        os << "";

        os.write( p, n );
    }

    os.width( 0 );
    return os;
}

#if defined(BOOST_NO_CXX17_INLINE_VARIABLES)
template<class Ch> constexpr_OR_CONST std::size_t basic_string_view<Ch>::npos;
#endif

// typedef names

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;
typedef basic_string_view<char16_t> u16string_view;
typedef basic_string_view<char32_t> u32string_view;

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
typedef basic_string_view<char8_t> u8string_view;
#endif

} // namespace core
} // namespace boost

namespace b2 {

using string_view = boost::core::string_view;

}

#endif  // #ifndef BOOST_CORE_STRING_VIEW_HPP_INCLUDED
