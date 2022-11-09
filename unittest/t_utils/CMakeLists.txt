cmake_minimum_required (VERSION 3.8)
project (t_utils)
# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp ../inc/*.h)
message(STATUS ${DIR_SRCS})

add_executable (cond_test "t_condition.cpp")
add_executable (thread_test "t_thread.cpp")
add_executable (utils_test "t_utils.cpp")
add_executable (templete_test "t_template.cpp")
add_executable (lockfree_test "t_lockfree.cpp")

include_directories(
	${LOGGER_INC}
	${UTILS_INC}
)
target_link_libraries(thread_test 
	logger
)
target_link_libraries(cond_test 
	logger
)
target_link_libraries(utils_test 
	logger
)
target_link_libraries(templete_test 
	logger
)

target_link_libraries(lockfree_test 
	logger
	unofficial::concurrentqueue::concurrentqueue
)
