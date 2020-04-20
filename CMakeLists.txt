project(SoEiXRS)
cmake_minimum_required(VERSION 2.8)

# put binary output in bin/ subdirectory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(LIBRARY_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# select release build type as default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "'${CMAKE_BUILD_TYPE}' built type was selected.")

# add include directories
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}")

# add compiler flags to define C version
# and to compile position independent code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89 -fPIC")

# add compiler flags to enable strict compilation in debug mode
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 --coverage")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O3 ")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wmissing-prototypes")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wstrict-prototypes")

# add compiler flags to define C++ version
# and to compile position independent code
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# add C++ compiler flag to enable code coverage testing
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")

#add geant4
find_package(Geant4 REQUIRED vis_all )
include_directories(${Geant4_INCLUDE_DIRS})
add_definitions(${Geant4_DEFINITIONS})

#add root
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO MathCore GenVector)
include(${ROOT_USE_FILE})

set(HEADERS
    ${PROJECT_SOURCE_DIR}/src/include.hh
    ${PROJECT_SOURCE_DIR}/src/define.hh
    ${PROJECT_SOURCE_DIR}/src/classes/ActionInitialization.hpp
    ${PROJECT_SOURCE_DIR}/src/classes/PrimaryGeneratorAction.hpp
    ${PROJECT_SOURCE_DIR}/src/classes/SteppingAction.hpp
    ${PROJECT_SOURCE_DIR}/src/classes/DetectorConstruction.hpp
    ${PROJECT_SOURCE_DIR}/src/classes/UIThread.hpp
    ${PROJECT_SOURCE_DIR}/src/classes/DetectorPosition.hh
)

# add source files of this module
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/classes/DetectorConstruction.cpp
    ${PROJECT_SOURCE_DIR}/src/classes/ActionInitialization.cpp
    ${PROJECT_SOURCE_DIR}/src/classes/PrimaryGeneratorAction.cpp
    ${PROJECT_SOURCE_DIR}/src/classes/SteppingAction.cpp
    ${PROJECT_SOURCE_DIR}/src/classes/UIThread.cpp
)

# add object files to be compiled
add_executable(SoEiXRS ${SOURCES})
target_link_libraries(SoEiXRS ${Geant4_LIBRARIES})
target_link_libraries(SoEiXRS "stdc++fs")# "c++experimental")

link_directories(/opt/lib/)
target_link_libraries(SoEiXRS
    #"AllpixCore"
    "Core"
	"G4zlib"
	"RIO"
	"MathCore"
	"GenVector"
	#"AllpixObjects"
	"dl"
	"pthread"
	"m"
	"pcre"
	"z"
	"Thread"
	"Imt"
	"tbb")

