////////////////////////////////////////////////////////////////////////////////
///
/// \file file.inl
/// --------------
///
/// Copyright (c) Domagoj Saric 2010.-2011.
///
///  Use, modification and distribution is subject to the Boost Software License, Version 1.0.
///  (See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt)
///
/// For more information, see http://www.boost.org
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#include "file.hpp"

#include "flags.hpp"
#include "../../../detail/impl_inline.hpp"

#include "boost/assert.hpp"

#ifdef BOOST_MSVC
    #pragma warning ( disable : 4996 ) // "The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name."
    #include "io.h"
#else
    #include "sys/mman.h"      // mmap, munmap.
    #include "sys/stat.h"
    #include "sys/types.h"     // struct stat.
    #include "unistd.h"        // sysconf.
#endif // BOOST_MSVC
#include "errno.h"
#include "fcntl.h"
//------------------------------------------------------------------------------
namespace boost
{
//------------------------------------------------------------------------------
namespace mmap
{
//------------------------------------------------------------------------------

BOOST_IMPL_INLINE
posix_handle create_file( char const * const file_name, posix_file_flags const & flags )
{
    BOOST_ASSERT( file_name );

    int const current_mask( ::umask( 0 ) );
    int const file_handle ( ::open( file_name, flags.oflag, flags.pmode ) );
    //...zzz...investigate posix_fadvise, posix_madvise, fcntl for the system hints...
    BOOST_VERIFY( ::umask( current_mask ) == 0 );

    return posix_handle( file_handle );
}


#ifdef BOOST_HAS_UNISTD_H
BOOST_IMPL_INLINE
bool set_file_size( posix_handle::handle_t const file_handle, std::size_t const desired_size )
{
    return ::ftruncate( file_handle, desired_size ) != -1;
}
#endif // BOOST_HAS_UNISTD_H


BOOST_IMPL_INLINE
std::size_t get_file_size( posix_handle::handle_t const file_handle )
{
    struct stat file_info;
    BOOST_VERIFY( ::fstat( file_handle, &file_info ) == 0 );
    return file_info.st_size;
}

//------------------------------------------------------------------------------
} // mmap
//------------------------------------------------------------------------------
} // boost
//------------------------------------------------------------------------------