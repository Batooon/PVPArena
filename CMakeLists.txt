cmake_minimum_required(VERSION 3.20)
project(PVPArena)

set(CMAKE_CXX_STANDARD 17)

add_executable(PVPArena main.cpp ResourceHolder.h ResourceHolder.inl SceneNode.h SceneNode.cpp Command.h Command.cpp
        Entity.h Entity.cpp World.h World.cpp Soldier.h Soldier.cpp Game.h Game.cpp Category.h
        Resources.h CommandQueue.cpp CommandQueue.h Player.cpp Player.h Data.h Data.cpp TextNode.cpp TextNode.h LineNode.cpp LineNode.h Projectile.cpp Projectile.h SpriteNode.cpp SpriteNode.h Utilities.cpp Utilities.h)

set(SFML_ROOT "C:/SFML-2.5.1")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()
