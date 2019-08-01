cmake_minimum_required(VERSION 3.10.2)
project(core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_FLG "--warn-unused-variable -unknown-pragmas -Wall -Werror")
set(CXX_DBG "-g")
set(CXX_THD "-pthread")
list(APPEND CMAKE_CXX_FLAGS "${CXX_FLAG} ${CXX_DBG} ${CXX_THD}")

# if (MSVC)
# 	find_package(Poco 1.8.0 COMPONENTS FOUNDATION DATA JSON REQUIRED)
# else()
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra")
#     find_package(Poco 1.8.0 COMPONENTS FOUNDATION DATA JSON REQUIRED)
# endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories(${Boost_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")
add_executable(Test ${SOURCES})

link_directories("/usr/local/lib")
set(FOUNDATION_LIB "PocoFoundation")
set(NET_LIB "PocoNet")
set(UTIL_LIB "PocoUtil")
set(DATA_LIB "PocoData")
set(MYSQL_LIB "PocoDataMySQL")
target_link_libraries(Test ${FOUNDATION_LIB} ${NET_LIB} ${UTIL_LIB} ${DATA_LIB} ${MYSQL_LIB})

# PHONY TARGETS
add_custom_target(run "./Test")
add_custom_target(memcheck memcheck "./Test")