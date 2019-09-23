cmake_minimum_required(VERSION 3.15)
project(Replicator)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# Boost
add_subdirectory(boost-cmake)

# Set warning and errors flags
set(CMAKE_CXX_STANDARD 17)
set(warnings "-Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-but-set-variable")
set(CMAKE_CXX_FLAGS "${warnings}")
set(CMAKE_C_FLAGS   "${warnings}")

# Add include folder
include_directories(include)
# Add source folder
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable and link libraries
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::log)



