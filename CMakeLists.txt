cmake_minimum_required(VERSION 3.20)
project(PVPArena)

set(CMAKE_CXX_STANDARD 17)

add_executable(PVPArena main.cpp)

set(SFML_ROOT "C:/SFML-2.5.1")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()
