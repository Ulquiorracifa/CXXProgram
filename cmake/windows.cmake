#设置运行库编译选项
# ( MT MTd windows lib) ( MD MDd windows dll)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
# SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
# SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
#禁用特定警告
add_definitions(
    /wd4018 # 'expression' : signed/unsigned mismatch
    /wd4065 # switch statement contains 'default' but no 'case' labels
    /wd4146 # unary minus operator applied to unsigned type, result still unsigned
    /wd4244 # 'conversion' conversion from 'type1' to 'type2', possible loss of data
    /wd4251 # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    /wd4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
    /wd4305 # 'identifier' : truncation from 'type1' to 'type2'
    /wd4307 # 'operator' : integral constant overflow
    /wd4309 # 'conversion' : truncation of constant value
    /wd4334 # 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
    /wd4355 # 'this' : used in base member initializer list
    /wd4506 # no definition for inline function 'function'
    /wd4800 # 'type' : forcing value to bool 'true' or 'false' (performance warning)
    /wd4819 # The file contains a character that can`t be represented in the current code page(936)
    /wd4996 # The compiler encountered a deprecated declaration.
    /wd4200 # nostandard extension used: zero-sized array in struct/union
    /wd4514
    /wd4820
    /wd4127
    /wd4710
    /wd4711
    /wd4577
    /wd4996
)
if (MSVC)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    # foreach(CompilerFlag ${CompilerFlags})
    #     string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    # endforeach()
endif(MSVC)
message("CMAKE_CXX_FLAGS_DEBUG -------------------------------- ${CMAKE_CXX_FLAGS_DEBUG}")
add_definitions(-DWIN32)
add_definitions(-D_WIN32_WINDOWS)

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-D_DEBUG)
    add_definitions(-DWIN64)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(-DRENDER_SNIPPET)
    add_definitions(-DPX_DEBUG=1)
    add_definitions(-DPX_CHECKED=1)
    add_definitions(-DPX_NVTX=0)
    add_definitions(-DPX_SUPPORT_PVD=1)
    add_definitions(-DCMAKE_INTDIR="debug")
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
    add_definitions(-DNDEBUG)
    add_definitions(-DWIN64)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(-DRENDER_SNIPPET)
    add_definitions(-DPX_SUPPORT_PVD=0)
    add_definitions(-DCMAKE_INTDIR="release")
ENDIF()


