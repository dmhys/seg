cmake_minimum_required(VERSION 3.10.0)
project(
    seg
    VERSION 0.0.1
    DESCRIPTION "Slam Easy GUI"
    LANGUAGES CXX
)

# build options ==========================================
set(BUILD_EXAMPLES on)
set(USE_LOG on)
set(USE_SPDLOG on)
set(USE_OPENCV on)

# options ================================================
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(EIGEN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/eigen)
set(IMGUI_ROOT_DIR ${CMAKE_SOURCE_DIR}/3rdparty/imgui)
set(IMGUI_INCLUDE_DIRS ${IMGUI_ROOT_DIR} ${IMGUI_ROOT_DIR}/backends)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_BUILD_TYPE Release)

# dependencies ===========================================
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rdparty
    ${EIGEN_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIRS}
)

set(GL_LIBS
  ${OPENGL_LIBRARIES}
  glfw
  GLEW
)

# optional Features ======================================
set(OPTIONAL_LIBS)
if(${USE_LOG})
if(${USE_SPDLOG})
    add_definitions(-DSPDLOG)
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/spdlog)
    set(LOGGER_LIB spdlog::spdlog)
endif()
endif()

if(${USE_OPENCV})
find_package(OpenCV REQUIRED)

if(${OPENCV_FOUND})
    add_definitions(-DOPENCV)
    list(APPEND OPTIONAL_LIBS opencv_core opencv_highgui opencv_imgproc)
endif()

endif()

# source files ===========================================
file(GLOB IMGUI_SRCS
    ${IMGUI_ROOT_DIR}/*.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SEG_SRCS
    seg/*.cpp
)
# foreach(sourcefile ${SEG_SRCS})
# message(STATUS ${sourcefile})
# endforeach()

# build ==================================================
add_library(imgui_glfw_opengl3 SHARED
    ${IMGUI_SRCS}
)

target_link_libraries(imgui_glfw_opengl3
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
    glfw
)

add_library(seg SHARED
    ${SEG_SRCS}
)

target_link_libraries(seg PRIVATE
    ${LOGGER_LIB}
    ${OPTIONAL_LIBS}
    ${GL_LIBS}
    imgui_glfw_opengl3
)

if(${BUILD_EXAMPLES})
add_subdirectory(examples)
endif()