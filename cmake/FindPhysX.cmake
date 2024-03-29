include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

if(CMAKE_SIZEOF_VOID_P EQUAL "8")
    set(_PHYSX_LIBRARY_SUFFIX "64")
else()
    set(_PHYSX_LIBRARY_SUFFIX "32")
endif()

set(_PHYSX_COMMON_LIBRARY_NAMES
    PhysXCommon_${_PHYSX_LIBRARY_SUFFIX}
    PhysXCommon_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCommon_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCommon_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_PHYSICS_LIBRARY_NAMES
    PhysX_${_PHYSX_LIBRARY_SUFFIX}
    PhysX_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysX_${_PHYSX_LIBRARY_SUFFIX}
    libPhysX_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_FOUNDATION_LIBRARY_NAMES
    PhysXFoundation_${_PHYSX_LIBRARY_SUFFIX}
    PhysXFoundation_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXFoundation_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXFoundation_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_COOKING_LIBRARY_NAMES
    PhysXCooking_${_PHYSX_LIBRARY_SUFFIX}
    PhysXCooking_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCooking_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCooking_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_EXTENSIONS_LIBRARY_NAMES
    PhysXExtensions_${_PHYSX_LIBRARY_SUFFIX}
    PhysXExtensions_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXExtensions_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXExtensions_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_CHARACTER_LIBRARY_NAMES
    PhysXCharacterKinematic_${_PHYSX_LIBRARY_SUFFIX}
    PhysXCharacterKinematic_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCharacterKinematic_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXCharacterKinematic_static_${_PHYSX_LIBRARY_SUFFIX}
)
set(_PHYSX_PVD_LIBRARY_NAMES
    PhysXPvdSDK_${_PHYSX_LIBRARY_SUFFIX}
    PhysXPvdSDK_static_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXPvdSDK_${_PHYSX_LIBRARY_SUFFIX}
    libPhysXPvdSDK_static_${_PHYSX_LIBRARY_SUFFIX}
)

# set(_SNIPPET_UTILS_LIBRARY_NAMES
#     SnippetUtils_${_PHYSX_LIBRARY_SUFFIX}
#     SnippetUtils_static_${_PHYSX_LIBRARY_SUFFIX}
#     libSnippetUtils_${_PHYSX_LIBRARY_SUFFIX}
#     libSnippetUtils_static_${_PHYSX_LIBRARY_SUFFIX}
# )

# set(_SNIPPET_RENDER_LIBRARY_NAMES
#     SnippetRender_${_PHYSX_LIBRARY_SUFFIX}
#     SnippetRender_static_${_PHYSX_LIBRARY_SUFFIX}
#     libSnippetRender_${_PHYSX_LIBRARY_SUFFIX}
#     libSnippetRender_static_${_PHYSX_LIBRARY_SUFFIX}
# )
# Stoopid vcpkg build debug and optimized libraries with the same name but in different directories.
foreach(prefix_path ${CMAKE_PREFIX_PATH})
    if(${prefix_path} MATCHES "[Dd][Ee][Bb][Uu][Gg]\/?$")
        list(APPEND _PHYSX_DEBUG_PATHS ${prefix_path})
    else()
        list(APPEND _PHYSX_RELEASE_PATHS ${prefix_path})
    endif()
    list(APPEND _PHYSX_PREFIX ${prefix_path})
endforeach()

# For non-vcpkg builds... *Gulp*
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(MSVC)
        set(_PHYSX_BIN_DIR "win.")
        if(CMAKE_SIZEOF_VOID_P EQUAL "8")
            string(APPEND _PHYSX_BIN_DIR "x86_64.")
        else()
            string(APPEND _PHYSX_BIN_DIR "x86.")
        endif()
        string(APPEND _PHYSX_BIN_DIR "vc${MSVC_TOOLSET_VERSION}.mt")
    endif()
    message("MSVC_TOOLSET_VERSION  ${MSVC_TOOLSET_VERSION}")
    if(MSVC_TOOLSET_VERSION EQUAL 120)
        set(_PHYSX_DIR "vc12win${_PHYSX_LIBRARY_SUFFIX}")
    # There is no toolset version 130...
    elseif(MSVC_TOOLSET_VERSION EQUAL 140)
        set(_PHYSX_DIR "vc14win${_PHYSX_LIBRARY_SUFFIX}")
    elseif(MSVC_TOOLSET_VERSION EQUAL 141)
        set(_PHYSX_DIR "vc15win${_PHYSX_LIBRARY_SUFFIX}")
    elseif(MSVC_TOOLSET_VERSION EQUAL 142)
        set(_PHYSX_DIR "vc16win${_PHYSX_LIBRARY_SUFFIX}")
    elseif(NOT VCPKG_TOOLCHAIN)
        message(WARNING "PhysX: Unhandled MSVC Toolset ${MSVC_TOOLSET_VERSION}. You may need to manually specify artifacts.")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(_PHYSX_DIR "linux")
    set(_PHYSX_BIN_DIR "linux.clang")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(_PHYSX_DIR "mac64")
    set(_PHYSX_BIN_DIR "mac64.xcode")
elseif(NOT VCPKG_TOOLCHAIN)
    message(WARNING "PhysX: Unhandled system: ${CMAKE_SYSTEM_NAME}. You may need to manually specify artifacts.")
endif()

if(_PHYSX_DIR)
    foreach(prefix_path ${CMAKE_PREFIX_PATH})
        if(_PHYSX_BIN_DIR)
            list(APPEND _PHYSX_DEBUG_PATHS "${prefix_path}/${_PHYSX_DIR}/PhysX/bin/${_PHYSX_BIN_DIR}/debug")
            list(APPEND _PHYSX_RELEASE_PATHS "${prefix_path}/${_PHYSX_DIR}/PhysX/bin/${_PHYSX_BIN_DIR}/release")
        elseif(NOT VCPKG_TOOLCHAIN)
            list(APPEND _PHYSX_DEBUG_PATHS ${prefix_path})
            list(APPEND _PHYSX_RELEASE_PATHS ${prefix_path})
        endif()
        list(APPEND _PHYSX_PREFIX "${prefix_path}/${_PHYSX_DIR}/PhysX")
        list(APPEND _PHYSX_PREFIX "${prefix_path}/${_PHYSX_DIR}/PxShared")
    endforeach()
elseif(NOT VCPKG_TOOLCHAIN)
    set(_PHYSX_DEBUG_PATHS ${CMAKE_PREFIX_PATH})
    set(_PHYSX_RELEASE_PATHS ${CMAKE_PREFIX_PATH})
endif()
message("--------------------------------------------------")
message("_PHYSX_DEBUG_PATHS  ${_PHYSX_DEBUG_PATHS}")
message("_PHYSX_EXTENSIONS_LIBRARY_NAMES ${_PHYSX_EXTENSIONS_LIBRARY_NAMES}")
message("PATH_SUFFIXES ${PATH_SUFFIXES}")
message("_PHYSX_PVD_LIBRARY_NAMES ${_PHYSX_PVD_LIBRARY_NAMES}")
macro(_find_physx_library VAR_NAME)
    find_library(${VAR_NAME}_LIBRARY_RELEASE
                 NAMES ${_${VAR_NAME}_LIBRARY_NAMES}
                 PATHS ${_PHYSX_RELEASE_PATHS}
                 PATH_SUFFIXES lib
                 NO_CMAKE_PATH
    )
    find_library(${VAR_NAME}_LIBRARY_DEBUG
                 NAMES ${_${VAR_NAME}_LIBRARY_NAMES}
                 PATHS ${_PHYSX_DEBUG_PATHS}
                 PATH_SUFFIXES lib
                 NO_CMAKE_PATH
    )
    select_library_configurations(${VAR_NAME})
endmacro()

_find_physx_library(PHYSX_COMMON)
_find_physx_library(PHYSX_PHYSICS)
_find_physx_library(PHYSX_FOUNDATION)
_find_physx_library(PHYSX_COOKING)
_find_physx_library(PHYSX_EXTENSIONS)
_find_physx_library(PHYSX_CHARACTER)
_find_physx_library(PHYSX_PVD)
find_library(GLUT glut32.lib ${PROJECT_SOURCE_DIR}/third_party/vcpkg/buildtrees/physx/src/be49a99479-d1b16eed08.clean/physx/snippets/graphics/lib/win64/glut)

find_path(PHYSX_INCLUDE_DIR NAMES PxPhysicsAPI.h
          PATHS ${_PHYSX_PREFIX}
          PATH_SUFFIXES include/physx include
          NO_CMAKE_PATH
)
mark_as_advanced(PHYSX_INCLUDE_DIR)

# PhysX has two include directories, but vcpkg merges them together. In case of a manual build,
# check and compensate for the lack of merging here.
if(PHYSX_INCLUDE_DIR)
    if(NOT EXISTS ${PHYSX_INCLUDE_DIR}/foundation/Px.h)
        find_path(PHYSX_FOUNDATION_INCLUDE_DIR NAMES foundation/Px.h
                  PATHS ${_PHYSX_PREFIX}
                  PATH_SUFFIXES include/physx include
                  NO_CMAKE_PATHS
        )
        mark_as_advanced(PHYSX_FOUNDATION_INCLUDE_DIR)
    else()
        set(PHYSX_FOUNDATION_INCLUDE_DIR ${PHYSX_INCLUDE_DIR} CACHE PATH "Path to a file.")
    endif()
endif()

find_package_handle_standard_args(PhysX
                                  REQUIRED_VARS PHYSX_COMMON_LIBRARY
                                                PHYSX_PHYSICS_LIBRARY
                                                PHYSX_FOUNDATION_LIBRARY
                                                PHYSX_COOKING_LIBRARY
                                                PHYSX_EXTENSIONS_LIBRARY
                                                PHYSX_CHARACTER_LIBRARY
                                                PHYSX_PVD_LIBRARY
                                                PHYSX_INCLUDE_DIR
                                                PHYSX_FOUNDATION_INCLUDE_DIR
                                  REASON_FAILURE_MESSAGE "Be sure that PhysX 4.1 is available."
)

set(PHYSX_LIBRARIES
    ${PHYSX_CHARACTER_LIBRARY}
    ${PHYSX_EXTENSIONS_LIBRARY}
    ${PHYSX_PHYSICS_LIBRARY}
    ${PHYSX_PVD_LIBRARY}
    ${PHYSX_COOKING_LIBRARY}
    ${PHYSX_COMMON_LIBRARY}
    ${PHYSX_FOUNDATION_LIBRARY}
)
include(cmake/snippet.cmake)
set(PHYSX_INCLUDE_DIRS
    ${PHYSX_INCLUDE_DIR}
    ${PHYSX_FOUNDATION_INCLUDE_DIR}
)
