cmake_minimum_required(VERSION 3.0)
project(uju C)
set(CMAKE_C_STANDARD 99)

include_directories(include)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)


file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
# set(ASSETS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")


# file(COPY ${ASSETS_DIR} DESTINATION ${ASSETS_OUTPUT_DIR})

# add_custom_target(copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${ASSETS_DIR} ${ASSETS_OUTPUT_DIR}
# )

# add_dependencies(${PROJECT_NAME} copy_assets)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") 