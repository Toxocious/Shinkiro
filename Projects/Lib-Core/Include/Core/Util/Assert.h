#pragma once

#ifndef SHINKIRO_CORE_ASSERT_H
#    define SHINKIRO_CORE_ASSERT_H

#    include <Core/_Defs.h>

#    include <Core/_Common.h>

#    include <Core/Util/Log.h>
#    include <Core/Util/Macro.h>

#    ifdef SHINKIRO_ENABLE_ASSERTS
#        define SHINKIRO_INTERNAL_ASSERT_IMPL( type, check, msg, ... ) \
            {                                                          \
                if ( !( check ) )                                      \
                {                                                      \
                    SHINKIRO##type##ERROR( msg, __VA_ARGS__ );         \
                    SHINKIRO_DEBUGBREAK();                             \
                }                                                      \
            }
#        define SHINKIRO_INTERNAL_ASSERT_WITH_MSG( type, check, ... ) SHINKIRO_INTERNAL_ASSERT_IMPL( type, check, "Assertion failed: {0}", __VA_ARGS__ )
#        define SHINKIRO_INTERNAL_ASSERT_NO_MSG( type, check )        SHINKIRO_INTERNAL_ASSERT_IMPL( type, check, "Assertion '{0}' failed at {1}:{2}", SHINKIRO_STRINGIFY_MACRO( check ), std::filesystem::path( __FILE__ ).filename().string(), __LINE__ )

#        define SHINKIRO_INTERNAL_ASSERT_GET_MACRO_NAME( arg1, arg2, macro, ... ) macro
#        define SHINKIRO_INTERNAL_ASSERT_GET_MACRO( ... )                         SHINKIRO_EXPAND_MACRO( SHINKIRO_INTERNAL_ASSERT_GET_MACRO_NAME( __VA_ARGS__, SHINKIRO_INTERNAL_ASSERT_WITH_MSG, SHINKIRO_INTERNAL_ASSERT_NO_MSG ) )

// Currently accepts at least the condition, with one additional parameter (the message) being optional
#        define SHINKIRO_ASSERT( ... )      SHINKIRO_EXPAND_MACRO( SHINKIRO_INTERNAL_ASSERT_GET_MACRO( __VA_ARGS__ )( _, __VA_ARGS__ ) )
#        define SHINKIRO_CORE_ASSERT( ... ) SHINKIRO_EXPAND_MACRO( SHINKIRO_INTERNAL_ASSERT_GET_MACRO( __VA_ARGS__ )( _CORE_, __VA_ARGS__ ) )
#    else
#        define SHINKIRO_ASSERT( ... )
#        define SHINKIRO_CORE_ASSERT( ... )
#    endif

#endif
