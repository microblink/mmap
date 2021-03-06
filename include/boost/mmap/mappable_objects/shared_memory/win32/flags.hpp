////////////////////////////////////////////////////////////////////////////////
///
/// \file shared_memory/win32/flags.hpp
/// -----------------------------------
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
#ifndef flags_hpp__504C3F9E_97C2_4E8C_82C6_881340C5FBA6
#define flags_hpp__504C3F9E_97C2_4E8C_82C6_881340C5FBA6
#pragma once
//------------------------------------------------------------------------------
#include "boost/config.hpp"

#include <boost/mmap/detail/impl_selection.hpp>
#include <boost/mmap/flags/win32/mapping.hpp>
//------------------------------------------------------------------------------
namespace boost
{
//------------------------------------------------------------------------------
namespace mmap
{
//------------------------------------------------------------------------------
namespace win32
{
//------------------------------------------------------------------------------
namespace flags
{
//------------------------------------------------------------------------------

 struct shared_memory;


struct shared_memory : mapping
{
    enum struct system_hints
    {
        default                    = 0x8000000,
        only_reserve_address_space = 0x4000000
    }; // struct system_hints

    static shared_memory BOOST_CC_REG create
    (
        access_privileges,
        named_object_construction_policy,
        system_hints system_hint
    ) noexcept;
}; // struct shared_memory

//------------------------------------------------------------------------------
} // namespace flags
  //------------------------------------------------------------------------------
} // namespace win32
//------------------------------------------------------------------------------
} // namespace mmap
//------------------------------------------------------------------------------
} // namespace boost
//------------------------------------------------------------------------------

#ifdef BOOST_MMAP_HEADER_ONLY
    #include "flags.inl"
#endif // BOOST_MMAP_HEADER_ONLY

#endif // flags.hpp
