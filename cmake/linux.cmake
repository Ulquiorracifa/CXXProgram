
# cxx flag
# if (CMAKE_LIBRARY_ARCHITECTURE MATCHES "centos")
#     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC")
# else()
#     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC") # -Wall 
# endif()
#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wall -O0 -g -ggdb -fsanitize=address") # -Wno-error=unused-variable -Wno-error=class-memaccess -Wno-error=catch-value= -Wno-error=pessimizing-move 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall") #  -Wextra
set(CMAKE_INSTALL_RPATH ".")
# static compile, so bigger
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# def
add_definitions(-DASIO_HAS_EPOLL)
set(DLL_SUFFIX ".so")
# thread
set(THREAD_LIBRARY pthread)
# filesystem
set(FILE_SYSTEM_LIBRARY stdc++fs)
