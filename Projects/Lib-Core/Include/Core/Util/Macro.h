#pragma once

#ifndef SHINKIRO_CORE_MACRO_H
#    define SHINKIRO_CORE_MACRO_H

#    include <Core/_Defs.h>

#    include <Core/_Common.h>

#    ifdef SHINKIRO_DEBUG
#        if defined( SHINKIRO_PLATFORM_WINDOWS )
#            define SHINKIRO_DEBUGBREAK() __debugbreak()
#        else
#            error "Platform doesn't support debugbreak yet!"
#        endif
#        ifndef SHINKIRO_ENABLE_ASSERTS
#            define SHINKIRO_ENABLE_ASSERTS
#        endif
#    else
#        define SHINKIRO_DEBUGBREAK()
#    endif

#    define SHINKIRO_EXPAND_MACRO( x )    x
#    define SHINKIRO_STRINGIFY_MACRO( x ) #x

#    define BIT( x ) ( 1 << x )

#    define SHINKIRO_BIND_EVENT_FN( fn ) [this]( auto &&... args ) -> decltype( auto ) { \
        return this->fn( std::forward<decltype( args )>( args )... );                    \
    }

namespace Shinkiro::Core
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope( Args &&... args )
    {
        return std::make_unique<T>( std::forward<Args>( args )... );
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef( Args &&... args )
    {
        return std::make_shared<T>( std::forward<Args>( args )... );
    }
}

#    include <Core/Util/Assert.h>
#    include <Core/Util/Log.h>

#endif
