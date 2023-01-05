
set(REDIS_INC ${PROJECT_SOURCE_DIR}/engine/redis/..)
set(UTILS_INC ${PROJECT_SOURCE_DIR}/engine/utils/..)
set(LOGGER_INC  ${PROJECT_SOURCE_DIR}/engine/logger/..) 
set(NET_INC  ${PROJECT_SOURCE_DIR}/engine/net/..) 
set(PHYSX_INC ${PROJECT_SOURCE_DIR}/engine/physx/..)
set(MONGO_INC ${PROJECT_SOURCE_DIR}/engine/mongo/..)
#set(physx_DIR "${PROJECT_SOURCE_DIR}/cmake/FindPhysX.cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
# hireids
find_package(hiredis CONFIG REQUIRED)
#target_link_libraries(main PRIVATE hiredis::hiredis)

#protobuf
#find_package(protobuf CONFIG REQUIRED)
#target_link_libraries(main PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

#spdlog
find_package(spdlog CONFIG REQUIRED)
#target_link_libraries(main PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

#asio
find_package(asio CONFIG REQUIRED)
#target_link_libraries(main PRIVATE asio asio::asio)

#concurrentqueue
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
#target_link_libraries(main PRIVATE unofficial::concurrentqueue::concurrentqueue)

# find_package(pthread CONFIG REQUIRED)

#physx
find_package(PhysX)
#target_link_libraries(main PRIVATE physx physx::physx)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)