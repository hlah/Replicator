cmake_minimum_required(VERSION 3.15)
project(Replicator)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.3)

# Set warning and errors flags
set(CMAKE_CXX_STANDARD 17)
set(warnings "-Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-but-set-variable")
set(CMAKE_CXX_FLAGS "${warnings}")
set(CMAKE_C_FLAGS   "${warnings}")


# Add include folder
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIR})
# Add source folder
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable and link libraries
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})



