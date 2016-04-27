#Cmake for Pre Processing Project
PROJECT(clustering)

#Minimum version of Cmake required
cmake_minimum_required(VERSION 3.1)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)


#Find and add opencv libraries
find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)

IF(${OpenCV_VERSION} VERSION_LESS 2.4.12)
	MESSAGE(FATAL_ERROR "OpenCV version is not compatible : ${OpenCV_VERSION}")
ENDIF()

#Check for C++ Compiler version. I am using C++14.
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX14)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSE()
    MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
ENDIF()


#include the header files located in the include folder
INCLUDE_DIRECTORIES(include)

#Set the variables for Headers and Sources. Manually add the filenames. This 
#is useful if the files are changed, removed or name modified.
#Better than GLOB version
SET( 	HEADERS 
	include/clustering.h
)

set( 	SOURCES
	src/main.cpp
)


add_executable(clustering ${SOURCES} ${HEADERS})

target_link_libraries(clustering
    ${OpenCV_LIBS}
)
