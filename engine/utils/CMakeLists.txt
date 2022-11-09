cmake_minimum_required (VERSION 3.8)
project (utils)

# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp)
#source_group("" FILES ${DIR_SRCS})
message(STATUS ${DIR_SRCS})
# 将源代码添加到此项目的可执行文件。
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})
#强制按照C++语言编译，解决必须要cpp文件才能link的问题
#set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX C)
include_directories(
	${UTILS_INC}
)
