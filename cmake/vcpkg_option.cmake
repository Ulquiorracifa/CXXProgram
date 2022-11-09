set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()
if(UNIX)
    set(VCPKG_DEFAULT_TRIPLET "x64-linux")
else()
    set(VCPKG_DEFAULT_TRIPLET "x64-windows")
endif()
#此变量设置将安装和使用库的位置。
#在清单模式下，默认值为${CMAKE_BINARY_DIR}/vcpkg_installed.
#在经典模式下，默认为${VCPKG_ROOT}/installed.
set(VCPKG_INSTALLED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vcpkg/installed)
set(VCPKG_FEATURE_FLAGS "versions")
#此变量控制在您的配置步骤中是否会自动运行 vcpkg 以安装您的依赖项
#set(VCPKG_MANIFEST_MODE off)
#message("CMAKE_CURRENT_SOURCE_DIR----------------"  ${CMAKE_CURRENT_SOURCE_DIR} "----------------------")
#message("CMAKE_TOOLCHAIN_FILE----------------"  ${CMAKE_TOOLCHAIN_FILE} "----------------------")
message("VCPKG_INSTALLED_DIR----------------"  ${VCPKG_INSTALLED_DIR} "----------------------")
