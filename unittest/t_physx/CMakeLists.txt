cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

project (t_physx)

set_property(TARGET sphysx PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# # 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp)
message(STATUS ${DIR_SRCS})
# # 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} ${DIR_SRCS})
include_directories(
	${PHYSX_INC}
	${PHYSX_INCLUDE_DIRS}
	${SNIPPET_INC}
)
target_link_libraries(${PROJECT_NAME} 
  ${GLUT}  
  sphysx
)

# project(t_physx C CXX)

