cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

project (t_physx)
set(RENDER_SNIPPET true)
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
# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#   if(MSVC_TOOLSET_VERSION EQUAL 142)
#     set(GLUTDLL 
#       ${PROJECT_SOURCE_DIR}/third_party/vcpkg/buildtrees/physx/src/be49a99479-d1b16eed08.clean/physx/bin/win.x86_64.vc142.mt/debug/glut32.dll
#     )
#     target_link_libraries(${PROJECT_NAME}
#       ${GLUTDLL}
#     )
#   elseif(NOT VCPKG_TOOLCHAIN)
#     message(WARNING "PhysX: Unhandled MSVC Toolset ${MSVC_TOOLSET_VERSION}. You may need to manually specify artifacts.")
#   endif()
# endif()
target_link_libraries(${PROJECT_NAME} 
  # ${PHYSX_COMMON_LIBRARY}
  # ${PHYSX_PHYSICS_LIBRARY}
  # ${PHYSX_FOUNDATION_LIBRARY}
  # ${PHYSX_COOKING_LIBRARY}
  # ${PHYSX_EXTENSIONS_LIBRARY}
  # ${PHYSX_CHARACTER_LIBRARY}
  # ${PHYSX_PVD_LIBRARY}
  ${GLUT}  
  sphysx
)

# project(t_physx C CXX)

