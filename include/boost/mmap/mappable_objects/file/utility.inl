////////////////////////////////////////////////////////////////////////////////
///
/// \file mapped_view.inl
/// ---------------------
///
/// Copyright (c) Domagoj Saric 2010 - 2015.
///
/// Use, modification and distribution is subject to the
/// Boost Software License, Version 1.0.
/// (See accompanying file LICENSE_1_0.txt or copy at
/// http://www.boost.org/LICENSE_1_0.txt)
///
/// For more information, see http://www.boost.org
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#include "utility.hpp"

#include "file.hpp"

#include "boost/mmap/flags/flags.hpp"
#include "boost/mmap/detail/impl_inline.hpp"
#include "boost/mmap/detail/impl_selection.hpp"
//------------------------------------------------------------------------------
namespace boost
{
//------------------------------------------------------------------------------
namespace mmap
{
//------------------------------------------------------------------------------

namespace detail
{
    using opening             = flags::opening<BOOST_MMAP_IMPL()>;
    using default_file_handle = file_handle   <BOOST_MMAP_IMPL()>;

    BOOST_IMPL_INLINE
    opening create_rw_file_flags()
    {
        using namespace flags;
        using ap = access_privileges<BOOST_MMAP_IMPL()>;
        return opening::create
        (
            {
                ap::object { ap::readwrite },
                ap::child_process::does_not_inherit,
                ap::system::process_default// ap::system::user( ap::readwrite ) | ap::system::group( ap::read )
            },
            named_object_construction_policy<BOOST_MMAP_IMPL()>::open_or_create,
            system_hints                    <BOOST_MMAP_IMPL()>::sequential_access
        );
    }

    BOOST_IMPL_INLINE
    opening create_r_file_flags()
    {
        using namespace flags;
        using ap = access_privileges<BOOST_MMAP_IMPL()>;
        return opening::create_for_opening_existing_objects
               (
                   ap::object { ap::readwrite },
                   ap::child_process::does_not_inherit,
                   system_hints<BOOST_MMAP_IMPL()>::sequential_access,
                   false
               );
    }

    BOOST_IMPL_INLINE
    err::fallible_result<basic_mapped_view, mmap::error<>> BOOST_CC_REG
    map_file( default_file_handle::reference const file_handle, std::size_t /*const*/ desired_size )
    {
        /// \note There is no need to call get/set_size() on the file_handle
        /// as CreateFileMapping() automatically expands the file as necessary
        /// (but only if the file is opened with write access and the
        /// share_mode::hidden flag is not specified).
        /// https://msdn.microsoft.com/en-us/library/aa366542(v=vs.85).aspx
        ///                                   (30.05.2015.) (Domagoj Saric)
        // memadv http://stackoverflow.com/questions/13126167/is-it-safe-to-ftruncate-a-shared-memory-object-after-it-has-ben-mmaped
    #ifndef _WIN32
        if ( desired_size )
                           set_size( file_handle, desired_size );
        else
            desired_size = get_size( file_handle               );
    #endif // _WIN32

        using ap    = flags::access_privileges<BOOST_MMAP_IMPL()>;
        using flags = flags::mapping<BOOST_MMAP_IMPL()>;
        return basic_mapped_view::map
        (
            create_mapping
            (
                file_handle,
                ap::object { ap::readwrite },
                ap::child_process::does_not_inherit,
                flags::share_mode::shared,
                desired_size
            ),
            0, // no offset
            0/*desired_size*/
        );
    }


    BOOST_IMPL_INLINE
    err::fallible_result<basic_read_only_mapped_view, mmap::error<>>
    map_read_only_file( default_file_handle::reference const file_handle ) noexcept
    {
        using ap    = flags::access_privileges<BOOST_MMAP_IMPL()>;
        using flags = flags::mapping<BOOST_MMAP_IMPL()>;
        return basic_read_only_mapped_view::map
        (
            create_mapping
            (
                file_handle,
                ap::object { ap::read },
                ap::child_process::does_not_inherit,
                flags::share_mode::shared,
                0
            ),
            0, // no offset
        #ifdef _WIN32
            0
        #else
            get_size( file_handle )
        #endif // OS
        );
    }
} // namespace detail

BOOST_IMPL_INLINE BOOST_ATTRIBUTES( BOOST_COLD, BOOST_EXCEPTIONLESS )
err::fallible_result<basic_mapped_view, mmap::error<>> map_file( char const * const file_name, std::size_t const desired_size ) noexcept
{
    return detail::map_file( create_file( file_name, detail::create_rw_file_flags() ), desired_size );
}

BOOST_IMPL_INLINE BOOST_ATTRIBUTES( BOOST_COLD, BOOST_EXCEPTIONLESS )
err::fallible_result<basic_read_only_mapped_view, mmap::error<>> map_read_only_file( char const * const file_name ) noexcept
{
    return detail::map_read_only_file( create_file( file_name, detail::create_r_file_flags() ) );
}

#ifdef _WIN32
    BOOST_IMPL_INLINE BOOST_ATTRIBUTES( BOOST_COLD, BOOST_EXCEPTIONLESS )
    err::fallible_result<basic_mapped_view, mmap::error<>> map_file( wchar_t const * const file_name, std::size_t const desired_size )
    {
        return detail::map_file( create_file( file_name, detail::create_rw_file_flags() ), desired_size );
    }

    BOOST_IMPL_INLINE BOOST_ATTRIBUTES( BOOST_COLD, BOOST_EXCEPTIONLESS )
    err::fallible_result<basic_read_only_mapped_view, mmap::error<>> map_read_only_file( wchar_t const * const file_name )
    {
        return detail::map_read_only_file( create_file( file_name, detail::create_r_file_flags() ) );
    }
#endif // _WIN32

//------------------------------------------------------------------------------
} // mmap
//------------------------------------------------------------------------------
} // boost
//------------------------------------------------------------------------------

#ifndef BOOST_MMAP_HEADER_ONLY
    #include "file.inl"
#endif // BOOST_MMAP_HEADER_ONLY
