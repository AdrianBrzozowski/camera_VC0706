project(example_cam_PTC08)

cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_COLOR_MAKEFILE ON)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(INCLUDE_DIR Include)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LOCAL_HEADERS ${CMAKE_CURRENT_LIST_DIR}/${INCLUDE_DIR})
set(DEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/${INCLUDE_DIR})

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

# set interfaces
set (USE_QtSerialPort OFF CACHE BOOL "Using Qt?")
set (USE_fcntl ON CACHE BOOL "Using FCNTL?")

include_directories (${DEST_INCLUDE_DIR})

set(project_libs "")
if (USE_QtSerialPort)
    find_package(Qt5Core REQUIRED)
#    qt5_use_modules(${PROJECT_NAME} Core)
    add_definitions(-DUSE_QtSerialPort)
#    include_directories(${Qt5Core_INCLUDE_DIRS})
    set(project_libs ${project_libs} QtSerialPort_camPTC08)
endif(USE_QtSerialPort)

if (USE_fcntl)
    set(project_libs ${project_libs} fcntl_camPTC08)
endif(USE_fcntl)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
add_executable(${PROJECT_NAME} main.cpp )

if (USE_QtSerialPort)
    target_link_libraries(${PROJECT_NAME} ${Qt5Core_LIBRARIES}) # old stye
endif(USE_QtSerialPort)

target_link_libraries(${PROJECT_NAME} camPTC08 ${project_libs})
