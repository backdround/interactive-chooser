cmake_minimum_required(VERSION 3.12.2)
project(interactive-chooser CXX)

# Configure project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Setting up qt5
find_package(Qt5 COMPONENTS Core REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resources/resources.qrc)

# Add dependency cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


add_subdirectory(src)
add_subdirectory(fuzzywuzzy)
add_subdirectory(cxxopts)
add_subdirectory(libxdgbasedir)
