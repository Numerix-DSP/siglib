cmake_minimum_required(VERSION 3.25)

project(siglib LANGUAGES CXX C)

set(SIGLIB_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${SIGLIB_BASE_DIR}/src)

file(GLOB_RECURSE SIGLIB_SRCS
	${SRC_DIR}/*.c
	${SRC_DIR}/*.h)

ADD_DEFINITIONS(-DSIGLIB_STATIC_LIB=1)

add_library( siglib STATIC ${SIGLIB_SRCS} )
target_include_directories(siglib PUBLIC ${SIGLIB_BASE_DIR}/include)
