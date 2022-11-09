cmake_minimum_required (VERSION 3.8)
project (t_asio)
# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp)
message(STATUS ${DIR_SRCS})
# 将源代码添加到此项目的可执行文件。
#add_executable (${PROJECT_NAME} ${DIR_SRCS})

add_executable (asio_server "t_asio_server.cpp")
add_executable (asio_client "t_asio_client.cpp")
add_executable (asio_context "t_asio_context.cpp")
add_executable (asio_streambuf "t_streambuf.cpp")
include_directories(
	${LOGGER_INC}
	${UTILS_INC}
	${NET_INC}
)

target_link_libraries(asio_server 
	logger
	net
	asio asio::asio
)

target_link_libraries(asio_client 
	logger
	net
	asio asio::asio
)

target_link_libraries(asio_context 
	logger
	net
	asio asio::asio
)

target_link_libraries(asio_streambuf 
	logger
	net
	asio asio::asio
)


#add_definitions(-DWIN32)
#add_definitions(-D_WIN32_WINDOWS)
#add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
#add_definitions(-DASIO_STANDALONE)
#add_definitions(-DASIO_NO_DEPRECATED)
#add_definitions(-DUSE_OPENSSL)
#add_definitions(-DASIO_HAS_IOCP)
#add_definitions(-DASIO_HAS_EPOLL)

#target_link_libraries(${PROJECT_NAME} 
#	logger
#	asio asio::asio
#)

