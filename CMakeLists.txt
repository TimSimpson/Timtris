# *********************************************************************
# Timtris
#
# *********************************************************************
project(lp3_timtris CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Timtris
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameOverBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameOverBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Timtris.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Timtris.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisGrid.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisGrid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisPlayer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisPlayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisScore.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisScore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisTetrad.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisTetrad.cpp
)

target_include_directories(Lp3_Timtris
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Timtris Lp3_Core Lp3_Sims Lp3_Gfx Lp3_Input)

install(TARGETS Lp3_Timtris EXPORT Lp3_Timtris_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Timtris_Config DESTINATION share/Lp3_Timtris/cmake)

export(TARGETS Lp3_Timtris FILE Lp3_Timtris_Config.cmake)

add_executable(Timtris ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(Timtris Lp3_Timtris)

if (LP3_SETUP_FOLDERS)
    set_target_properties (Lp3_Timtris PROPERTIES FOLDER Timtris)
    set_target_properties (Timtris PROPERTIES FOLDER Timtris)
endif()
