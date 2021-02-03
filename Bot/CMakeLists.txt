cmake_minimum_required(VERSION 3.10)
project(cppbot LANGUAGES CXX)

# output
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/release)

# look in for the RLUtilities directory next door
add_subdirectory(../RLUtilities ${CMAKE_BINARY_DIR}/rlu)
#add_subdirectory(../RLBotCPP ${CMAKE_BINARY_DIR}/rlbotcpp)

# let CMake know which files to compile for your bot library
set(source_files
        ${PROJECT_SOURCE_DIR}/src/agent.cc
        ${PROJECT_SOURCE_DIR}/src/maneuvers/MoveTo.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/MoveTo.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Maneuver.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Maneuver.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Recovery.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Recovery.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Kickoff/SpeedFlipKickoff.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Kickoff/SpeedFlipKickoff.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/GroundShot.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/GroundShot.h
        #${PROJECT_SOURCE_DIR}/src/utils/misc.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Kickoff/FrontFlipKickoff.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Kickoff/FrontFlipKickoff.h
        ${PROJECT_SOURCE_DIR}/src/strategy/Strategy.cpp
        #${PROJECT_SOURCE_DIR}/src/strategy/Strategy.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/Shot.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/Shot.h
        ${PROJECT_SOURCE_DIR}/src/utils/Prediction.cpp
        #${PROJECT_SOURCE_DIR}/src/utils/Prediction.h
        ${PROJECT_SOURCE_DIR}/src/utils/misc.cpp
        ${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/DodgeShot.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/Shots/DodgeShot.h
        ${PROJECT_SOURCE_DIR}/src/maneuvers/GetBoost.cpp
        #${PROJECT_SOURCE_DIR}/src/maneuvers/GetBoost.h
        src/maneuvers/Shots/AerialShot.cpp src/maneuvers/Shots/AerialShot.h src/strategy/Targeting.cpp src/strategy/Targeting.h src/utils/game_info.h)

# build the library
add_library(botlib STATIC ${source_files})
target_include_directories(botlib PUBLIC inc)
target_compile_features(botlib PUBLIC cxx_std_17)        
target_link_libraries(botlib PUBLIC rlutilities_cpp)
#target_link_libraries(botlib PUBLIC RLBotCPP)

# build the executable
add_executable(my_bot main.cpp)
target_link_libraries(my_bot  PUBLIC rlutilities_cpp botlib)