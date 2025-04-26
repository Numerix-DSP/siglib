cmake_minimum_required(VERSION 3.25)

project(siglib LANGUAGES CXX C)

ADD_DEFINITIONS(-DSIGLIB_STATIC_LIB=1)

file(GLOB_RECURSE SIGLIB_SRCS src/*.c)

add_library( siglib STATIC ${SIGLIB_SRCS} )

target_include_directories(siglib PUBLIC include)
