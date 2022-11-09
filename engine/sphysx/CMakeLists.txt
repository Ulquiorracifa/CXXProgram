cmake_minimum_required (VERSION 3.8)
project (sphysx)

message("CMAKE_CXX_FLAGS_DEBUG -------------------------------- ${CMAKE_CXX_FLAGS_DEBUG}")
# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE DIR_SRCS ./*.cpp ./*.cc ./*.c ./*.h ./*.hpp)
message(STATUS ${DIR_SRCS})
# 将源代码添加到此项目的可执行文件。
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})
# add_library(${PROJECT_NAME} GLUT)
# add_library(${PROJECT_NAME} SNIPPETUTILS)
# add_library(${PROJECT_NAME} SNIPPETRENDER)

include_directories(
	${PHYSX_INC}
	${PHYSX_INCLUDE_DIRS}
	${SNIPPET_INC}
)
target_link_libraries(${PROJECT_NAME} 
	# ${PROJECT_SOURCE_DIR}/third_party/physx/debug/SnippetRender_static_64.lib
	# ${PROJECT_SOURCE_DIR}/third_party/physx/debug/SnippetUtils_static_64.lib
	${PHYSX_COMMON_LIBRARY}
	${PHYSX_PHYSICS_LIBRARY}
	${PHYSX_FOUNDATION_LIBRARY}
	${PHYSX_COOKING_LIBRARY}
	${PHYSX_EXTENSIONS_LIBRARY}
	${PHYSX_CHARACTER_LIBRARY}
	${PHYSX_PVD_LIBRARY}
	${GLUT}
)
