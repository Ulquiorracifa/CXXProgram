add_definitions(-DENGINE_SHARED_LIB -DENGINE_EXPORTS)
#add_subdirectory(utils) ##utils 不生成动态链接库，直接include
add_subdirectory(logger)
add_subdirectory(redis)
add_subdirectory(net)
add_subdirectory(sphysx)




