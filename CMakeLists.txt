# cmake requirements
cmake_minimum_required(VERSION 2.8)

# Build options have to be before PROJECT(...)
SET(CMAKE_CONFIGURATION_TYPES "Release" CACHE PATH "Configuration types")
SET(CMAKE_BUILD_TYPE "Release" CACHE PATH "Current build configuration")

# Example Project configuration
PROJECT(OPTIMIZER)

SET(CXX_STANDARD "-std=c++14")
SET(CMAKE_CXX_FLAGS "${CXX_STANDARD} -Wall -Wextra -Wshadow -Wconversion -pthread")

# Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Settings
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/resources")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/Defs")


# core libraries
option(BUILD_OPTIMIZER "Build Optimizer libraries" YES)
option(BUILD_DEFS "Build Optimizer Defs library" YES)
mark_as_advanced(BUILD_DEFS)
option(BUILD_OPTIMIZER "Build black box optimizer library" ON)
mark_as_advanced(BUILD_OPTIMIZER)

# demos
option(BUILD_DEMO_OPTIMIZER "Build Optimizer demo" ON)
mark_as_advanced(BUILD_DEMO_OPTIMIZER)

#additional dependencies
# Boost
FIND_PACKAGE(Boost COMPONENTS system filesystem thread asio)

#Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-Wno-deprecated -DEIGEN_DONT_VECTORIZE -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)

# Executable output directory
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Executable output directory")
mark_as_advanced(RUNTIME_OUTPUT_DIRECTORY)

# Dynamic library output directory
SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Dynamic library output directory")
mark_as_advanced(LIBRARY_OUTPUT_DIRECTORY)

# Static library output directory
SET(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Static library output directory")
mark_as_advanced(ARCHIVE_OUTPUT_DIRECTORY)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY})
foreach(CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${CONFIGURATION_TYPE} CONFIGURATION_TYPE)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${RUNTIME_OUTPUT_DIRECTORY})
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${LIBRARY_OUTPUT_DIRECTORY})
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${ARCHIVE_OUTPUT_DIRECTORY})
endforeach(CONFIGURATION_TYPE CMAKE_CONFIGURATION_TYPES)

if(MSVC)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR CMake)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()


###############################################################################
#
# Optimizer tinyXML2 library
#
###############################################################################

#add_library(tinyxml2 SHARED ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)
add_library(tinyxml2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)

###############################################################################
#
# OPTIMIZER defs library
#
###############################################################################

if(BUILD_DEFS)
        file(GLOB DEFS_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Defs/*.cpp"
        )
        file(GLOB DEFS_HEADERS
            "${CMAKE_SOURCE_DIR}/include/Defs/*.h"
        )
        ADD_LIBRARY(Defs STATIC ${DEFS_SOURCES} ${DEFS_HEADERS})
#        set_target_properties(Defs PROPERTIES LINKER_LANGUAGE CXX)
# 	INSTALL(TARGETS walkersDefs RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
        INSTALL(FILES ${DEFS_HEADERS} DESTINATION include/Defs/)
endif(BUILD_DEFS)

###############################################################################
#
# Optimizer library
#
###############################################################################

if(BUILD_OPTIMIZER)
        file(GLOB OPTIMIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Optimizer/*.cpp"
        )
        file(GLOB OPTIMIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/Optimizer/*.h"
        )
        ADD_LIBRARY(Optimizer STATIC ${OPTIMIZER_SOURCES} ${OPTIMIZER_HEADERS})
        TARGET_LINK_LIBRARIES(Optimizer tinyxml2)
#        INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
#        INSTALL(FILES ${VISUALIZER_HEADERS} DESTINATION include/walkers/Visualizer/)
endif(BUILD_OPTIMIZER)

###############################################################################
#
# SIMULATOR demo optimizer
#
###############################################################################

if(BUILD_OPTIMIZER AND BUILD_DEMO_OPTIMIZER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoOptimizer.cpp)
        ADD_EXECUTABLE(demoOptimizer ${DEMO_SOURCES})
        TARGET_LINK_LIBRARIES(demoOptimizer Optimizer)
        INSTALL(TARGETS demoOptimizer RUNTIME DESTINATION bin)

endif(BUILD_OPTIMIZER AND BUILD_DEMO_OPTIMIZER)
