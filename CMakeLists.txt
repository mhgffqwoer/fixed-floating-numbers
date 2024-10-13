cmake_minimum_required(VERSION 3.12)

project(
    MyFloat
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(src)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} FixedPoint)
target_link_libraries(${PROJECT_NAME} ParseArgs)
target_link_libraries(${PROJECT_NAME} FloatingPoint)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})