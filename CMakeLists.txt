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
elseif(APPLE)
elseif(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra")
else()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("/usr/local/include")

file(GLOB SOURCES "src/*.cpp")
add_executable(Main ${SOURCES})

link_directories("/usr/local/lib")
set(POCO_FOUNDATION "PocoFoundation")
set(POCO_NET "PocoNet")
set(POCO_JSON "PocoJSON")
set(POCO_UTIL "PocoUtil")
set(POCO_DATA "PocoData")
set(POCO_MYSQL "PocoDataMySQL")
target_link_libraries(Main ${POCO_FOUNDATION} ${POCO_NET} ${POCO_JSON} ${POCO_UTIL} ${POCO_DATA} ${POCO_MYSQL})

# PHONY TARGETS
add_custom_target(sync cp ../application.properties .)
add_custom_target(run Main)
add_custom_target(memcheck memcheck ./Main)
