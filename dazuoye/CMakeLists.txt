cmake_minimum_required(VERSION 3.28.3)
project(TeachingDiscussionGroup)

file(GLOB SERVERSRC ${CMAKE_SOURCE_DIR}/server/src/*.cc)
file(GLOB CLIENTSRC ${CMAKE_SOURCE_DIR}/client/src/*.cc)
message(STATUS "Client Source: ${CLIENTSRC}")
message(STATUS "Server Source: ${SERVERSRC}")

include_directories (
  ${CMAKE_SOURCE_DIR}/server/include
  ${CMAKE_SOURCE_DIR}/client/include
)  # 包含自定义头文件


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 创建服务器端可执行文件
add_executable(server ${SERVERSRC})
# 创建客户端可执行文件
add_executable(client ${CLIENTSRC})