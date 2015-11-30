project(camPTC08)

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

set(SOURCE_DIR Source)
set(project_SOURCE ${SOURCE_DIR}/camPTC08.cpp)

# install headers
file(COPY ${LOCAL_HEADERS}/camPTC08.h DESTINATION ${DEST_INCLUDE_DIR})
file(COPY ${LOCAL_HEADERS}/command.h DESTINATION ${DEST_INCLUDE_DIR})
file(COPY ${LOCAL_HEADERS}/iserialport.h DESTINATION ${DEST_INCLUDE_DIR})

include_directories(${DEST_INCLUDE_DIR})

# Add interfaces

set (USE_QtSerialPort OFF CACHE BOOL "Using Qt?")
set (USE_fcntl ON CACHE BOOL "Using FCNTL?")

if (USE_QtSerialPort)
    set(sub_project_name QtSerialPort_${PROJECT_NAME})
    find_package(Qt5Core REQUIRED)
    find_package(Qt5SerialPort REQUIRED)

    set(CMAKE_AUTOMOC ON)

    add_library(${sub_project_name} STATIC ${SOURCE_DIR}/serialport_qt.cpp)
    # qt5_use_modules(${sub_project_name} Core SerialPort)
     target_link_libraries(${sub_project_name} ${Qt5Core_LIBRARIES}) # old stye
     target_link_libraries(${sub_project_name} ${Qt5SerialPort_LIBRARIES}) # old stye

    set(project_LIBS ${project_LIBS} ${sub_project_name})
    file(COPY ${LOCAL_HEADERS}/serialport_qt.h DESTINATION ${DEST_INCLUDE_DIR})
endif(USE_QtSerialPort)

if (USE_fcntl)
    set(sub_project_name fcntl_${PROJECT_NAME})

    add_library(${sub_project_name} STATIC ${SOURCE_DIR}/serialport_fcntl.cpp)

    set(project_LIBS ${project_LIBS} ${sub_project_name})
    file(COPY ${LOCAL_HEADERS}/serialport_fcntl.h DESTINATION ${DEST_INCLUDE_DIR})
endif(USE_fcntl)

add_library(${PROJECT_NAME} STATIC ${project_SOURCE})

# set_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING})
# set_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION ${VERSION_MAJOR})
# set_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "")
# set_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "")

