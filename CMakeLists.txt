cmake_minimum_required(VERSION 3.28.3)
project(MySQLConnectionPool)

link_libraries(mysqlclient)

file(GLOB TESTSRC ${CMAKE_SOURCE_DIR}/test/*.cc)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cc)

include_directories (
  ${CMAKE_SOURCE_DIR}/include
)  # 包含自定义头文件


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# add_executable(${PROJECT_NAME} ${TESTSRC})
add_executable(${PROJECT_NAME} ${TESTSRC} ${SRC})