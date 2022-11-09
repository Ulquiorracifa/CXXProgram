cmake_minimum_required (VERSION 3.8)
project (redis)

# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp)
message(STATUS ${DIR_SRCS})
# 将源代码添加到此项目的可执行文件。
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

include_directories(
	${REDIS_INC}
	${UTILS_INC}
	${LOGGER_INC}
)
target_link_libraries(${PROJECT_NAME} 
	hiredis::hiredis
	logger
)
