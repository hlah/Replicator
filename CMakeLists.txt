cmake_minimum_required(VERSION 3.15)
project(Replicator)

# Flag information for editor completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type is Debug
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# Release flags
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(build_flags "-O3 -DNDEBUG")
endif(CMAKE_BUILD_TYPE MATCHES "Release")

# Debug flags
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(build_flags "-g -DDEBUG")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

set(CMAKE_CXX_FLAGS "${build_flags}")
set(CMAKE_C_FLAGS   "${build_flags}")


# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# spdlog
add_subdirectory(spdlog)

# EnTT
add_subdirectory(entt)

# STB
include_directories(stb)

# Set warning and errors flags
set(CMAKE_CXX_STANDARD 17)
set(warnings "-Wall -Wextra -Werror -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=unused-but-set-variable -Wno-error=type-limits")
set(CMAKE_CXX_FLAGS "${warnings} ${build_flags}")
set(CMAKE_C_FLAGS   "${warnings} ${build_flags}")

# Add include folder
include_directories(include)
# Add source folder
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable and link libraries
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC EnTT)



