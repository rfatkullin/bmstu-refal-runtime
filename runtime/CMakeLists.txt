project(runtime)
cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/headers)

file(GLOB_RECURSE RuntimeHeaders ${PROJECT_SOURCE_DIR}/headers/*.h)
file(GLOB_RECURSE RuntimeSources ${PROJECT_SOURCE_DIR}/sources/*.c)
file(GLOB_RECURSE RuntimeTestSources ${PROJECT_SOURCE_DIR}/test/*.c)

set(LIB_SRC_LIST ${RuntimeSources} ${RuntimeHeaders})
set(TEST_SRC_LIST ${RuntimeSources} ${RuntimeHeaders} ${RuntimeTestSources})

add_executable(${PROJECT_NAME}_test ${TEST_SRC_LIST})
add_library(${PROJECT_NAME} ${LIB_SRC_LIST})

target_link_libraries(${PROJECT_NAME} -lgmp)
target_link_libraries(${PROJECT_NAME}_test -lgmp)
