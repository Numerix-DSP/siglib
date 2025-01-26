cmake_minimum_required(VERSION 3.26)

project(siglib LANGUAGES CXX C)

set(SIGLIB_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${SIGLIB_BASE_DIR}/src)

file(GLOB_RECURSE SIGLIB_SRCS
	${SRC_DIR}/*.c
	${SRC_DIR}/*.h)

ADD_DEFINITIONS(-DSIGLIB_STATIC_LIB=1)

add_library( siglib STATIC ${SIGLIB_SRCS} )
target_include_directories(siglib PRIVATE ${SIGLIB_BASE_DIR}/include)
