cmake_minimum_required(VERSION 3.16)
project(hangman_cpp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default wxWidgets path for this project
if(NOT DEFINED WXWIN)
  set(WXWIN "C:/Users/lucal/Desktop/wxWidgets" CACHE PATH "wxWidgets root directory")
endif()

# For MinGW builds, help CMake find the correct paths
if(MINGW)
  if(NOT DEFINED wxWidgets_ROOT_DIR AND DEFINED WXWIN)
    set(wxWidgets_ROOT_DIR ${WXWIN} CACHE PATH "wxWidgets root directory")
  endif()
  
  if(NOT DEFINED wxWidgets_LIB_DIR)
    set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/gcc_dll CACHE PATH "wxWidgets library directory")
  endif()
  
  # Force MSVC to use MinGW DLLs
  set(wxWidgets_CONFIGURATION mswu)
endif()

find_package(wxWidgets COMPONENTS core base REQUIRED)
if(wxWidgets_FOUND)
  include(${wxWidgets_USE_FILE})
endif()

file(GLOB SOURCES "src/*.cpp")

if(WIN32)
  add_executable(${PROJECT_NAME} WIN32 ${SOURCES} src/app.manifest)
else()
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()

if(wxWidgets_FOUND)
  target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
else()
  message(FATAL_ERROR "wxWidgets not found. Please set the environment variable 'wxwidgets' to your wxWidgets root or install wxWidgets so CMake can find it.")
endif()
