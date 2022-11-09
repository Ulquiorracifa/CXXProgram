#pragma once

#if defined(WIN32) || defined(WIN64)
#define OS_PLATFORM_WINDOWS
#elif defined(LINUX32) || defined(LINUX64)
#define OS_PLATFORM_LINUX
#endif

#include <errno.h>
#include <stdint.h>
#include <assert.h>
#ifdef OS_PLATFORM_WINDOWS
#include <io.h>
#include <direct.h>
#include <process.h>
#include <time.h>
#include <stdarg.h>
//#include <Windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <string.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <netdb.h>
#endif

#if defined(ENGINE_SHARED_LIB)
#    if defined(OS_PLATFORM_WINDOWS)
#        ifdef ENGINE_EXPORTS
#            define EngineAPI __declspec(dllexport)
#        else // !ENGINE_EXPORTS
#            define EngineAPI __declspec(dllimport)
#        endif
#    else // !defined(OS_PLATFORM_WINDOWS)
#        define EngineAPI __attribute__((visibility("default")))
#    endif
#else // !defined(ENGINE_SHARED_LIB)
#    define EngineAPI
#endif


//#define ASSERT_FAIELD (assert(false))
//#define ASSERT(x)     (assert(x))
