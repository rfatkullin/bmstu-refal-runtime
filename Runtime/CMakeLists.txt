project(runtime)
cmake_minimum_required(VERSION 2.8)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
set(LIBS_DIR ${PARENT_DIR}/libs )
LINK_DIRECTORIES(${LIBS_DIR})
include_directories(${LIBS_DIR})

include_directories(${PROJECT_SOURCE_DIR}/headers)

file(GLOB RuntimeHeaders ${PROJECT_SOURCE_DIR}/headers/*.h)
file(GLOB RuntimeSources ${PROJECT_SOURCE_DIR}/sources/*.c)
file(GLOB RuntimeTestSources ${PROJECT_SOURCE_DIR}/test/*.c)

set(LIB_SRC_LIST ${RuntimeSources} ${RuntimeHeaders})
set(TEST_SRC_LIST ${RuntimeSources} ${RuntimeHeaders} ${RuntimeTestSources})

add_executable(${PROJECT_NAME}_test ${TEST_SRC_LIST})
add_library(${PROJECT_NAME} ${LIB_SRC_LIST})

target_link_libraries(${PROJECT_NAME} libgmp.a)
target_link_libraries(${PROJECT_NAME}_test libgmp.a)

#Временно
LINK_DIRECTORIES("/usr/lib/i386-linux-gnu")
target_link_libraries(${PROJECT_NAME} -licui18n -licuuc -licudata -ldl -lm -licuio )
target_link_libraries(${PROJECT_NAME}_test -licui18n -licuuc -licudata -ldl -lm -licuio )
