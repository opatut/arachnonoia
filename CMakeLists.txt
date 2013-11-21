cmake_minimum_required(VERSION 2.8.0)

project(arachnonoia)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

find_package(SFML 2 COMPONENTS audio graphics system window REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLM REQUIRED)

add_definitions(-Wall -Wextra -g -pedantic -fPIC -std=c++0x -Wno-unused-parameter)
set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    "${CMAKE_SOURCE_DIR}/engine/src/"
    ${SFML_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(${CMAKE_PROJECT_NAME}
    ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
)
