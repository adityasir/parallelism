cmake_minimum_required(VERSION 3.0.0)
project(parallelism VERSION 0.1.0)


include(CTest)
enable_testing()

add_executable(parallelism main.cpp)
include_directories(hight_precision_timer)
include_directories(socket)
include_directories(lock_based_queue)
include_directories(boost_lock_free_queue)

set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES /usr/local/include/ ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

message(STATUS "Include directory locatoin" ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

target_link_libraries(parallelism libgtest.a libgtest_main.a gtest boost_system boost_thread pthread)

#target_link_libraries(parallelism PUBLIC -lboost_system -lboost_thread -lpthread)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

