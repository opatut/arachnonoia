cmake_minimum_required(VERSION 2.8.0)

project(arachnonoia)

find_package(SFML 2 COMPONENTS audio graphics system window REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLM REQUIRED)

add_definitions(-Wall -Wextra -g -pedantic -fPIC -std=c++11 -Wshadow -Wno-unused-parameter)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(external/Thor/src thor_sources)
set_source_files_properties(${thor_sources} PROPERTIES COMPILE_FLAGS -w)

include_directories(
    "${CMAKE_SOURCE_DIR}/src/"
    SYSTEM "${CMAKE_SOURCE_DIR}/external/cereal/include/"
    SYSTEM ${SFML_INCLUDE_DIRS}
    SYSTEM ${BULLET_INCLUDE_DIRS}
    SYSTEM ${GLM_INCLUDE_DIRS}
    SYSTEM external/Thor/include
    SYSTEM external/Thor/extlibs/aurora/include
)

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(${CMAKE_PROJECT_NAME}
    ${thor_sources}
    ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${THOR_LIB}
)
