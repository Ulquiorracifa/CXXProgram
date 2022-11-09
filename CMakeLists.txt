cmake_minimum_required (VERSION 3.8)
#------------以下是vcpkg的初始化设置,需要写在project之前-------------#
include(cmake/vcpkg_option.cmake)
project (Server VERSION 1.1.0 LANGUAGES CXX C)

if(UNIX)
    find_file(CENTOS NAMES redhat-release PATHS "/etc" NO_DEFAULT_PATH)
    if (CENTOS MATCHES "CENTOS-NOTFOUND")
        set(CMAKE_LIBRARY_ARCHITECTURE "ubuntu")
    else()
        set(CMAKE_LIBRARY_ARCHITECTURE "centos")
    endif()
    include(cmake/linux.cmake)
else()
    set(CMAKE_LIBRARY_ARCHITECTURE "windows")
    include(cmake/windows.cmake)
endif()
include(cmake/common.cmake)
include(cmake/engine.cmake)


add_subdirectory (engine)
add_subdirectory (unittest)
