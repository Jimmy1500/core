cmake_minimum_required(VERSION 3.10.2)
project(core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_FLG "--warn-unused-variable -unknown-pragmas -Wall -Werror")
set(CXX_DBG "-g")
set(CXX_THD "-pthread")
set(CXX_OPT "-O3")
list(APPEND CMAKE_CXX_FLAGS "${CXX_FLAG} ${CXX_DBG} ${CXX_THD} ${CXX_OPT}")

if (MSVC)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("/usr/local/include")

file(GLOB SOURCES "src/*.cpp")
add_executable(Main ${SOURCES})

link_directories("/usr/local/lib")
set(FOUNDATION_LIB "PocoFoundation")
set(NET_LIB "PocoNet")
set(JSON_LIB "PocoJSON")
set(UTIL_LIB "PocoUtil")
set(DATA_LIB "PocoData")
set(MYSQL_LIB "PocoDataMySQL")
target_link_libraries(Main ${FOUNDATION_LIB} ${NET_LIB} ${JSON_LIB} ${UTIL_LIB} ${DATA_LIB} ${MYSQL_LIB})

# PHONY TARGETS
add_custom_target(sync cp ../application.properties .)
add_custom_target(run Main)
add_custom_target(memcheck memcheck "./Main")
