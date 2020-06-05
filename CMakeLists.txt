# *********************************************************************
# Timtris
#
# *********************************************************************
project(lp3_timtris CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(Lp3_Core)
find_package(Lp3_Input)
find_package(Lp3_Gfx)
find_package(Lp3_Mix)
find_package(Lp3_Sims)

add_library(Lp3_Timtris
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Timtris.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Timtris.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisGrid.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisGrid.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisPlayer.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisPlayer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisScore.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisScore.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisTetrad.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/TimtrisTetrad.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/input/Controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input/Controller.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/Options.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/Screen.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/Screens.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/Title.cpp
)

target_include_directories(Lp3_Timtris
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Timtris lp3::core lp3::sims lp3::gfx lp3::input)

add_executable(Timtris ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(Timtris Lp3_Timtris)

if (LP3_SETUP_FOLDERS)
    set_target_properties (Lp3_Timtris PROPERTIES FOLDER Timtris)
    set_target_properties (Timtris PROPERTIES FOLDER Timtris)
endif()
