cmake_minimum_required(VERSION 3.14)
project(
    seg
    VERSION 0.0.1
    DESCRIPTION "Slam Easy GUI"
    LANGUAGES CXX
)

# build options ==========================================
option(BUILD_EXAMPLES "Build example executables" ON)
option(USE_LOG        "Enable logging"            ON)
option(USE_SPDLOG     "Use spdlog for logging"    ON)
option(USE_OPENCV     "Enable OpenCV support"      ON)

# compiler settings ======================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# 3rdparty paths =========================================
set(SEG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(EIGEN_INCLUDE_DIR ${SEG_ROOT}/3rdparty/eigen)
set(IMGUI_ROOT_DIR ${SEG_ROOT}/3rdparty/imgui)

# dependencies ===========================================
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)

if(USE_LOG AND USE_SPDLOG)
  add_subdirectory(${SEG_ROOT}/3rdparty/spdlog EXCLUDE_FROM_ALL)
endif()

if(USE_OPENCV)
  find_package(OpenCV REQUIRED)
endif()

# source files ===========================================
file(GLOB IMGUI_SRCS
    ${IMGUI_ROOT_DIR}/*.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SEG_SRCS
    ${SEG_ROOT}/seg/*.cpp
)

# imgui (internal) =======================================
add_library(seg_imgui STATIC ${IMGUI_SRCS})

target_include_directories(seg_imgui PUBLIC
    ${IMGUI_ROOT_DIR}
    ${IMGUI_ROOT_DIR}/backends
)

target_link_libraries(seg_imgui PUBLIC
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
    glfw
)

# seg target =============================================
add_library(seg SHARED ${SEG_SRCS})
add_library(seg::seg ALIAS seg)

target_include_directories(seg
  PUBLIC
    $<BUILD_INTERFACE:${SEG_ROOT}>
    $<BUILD_INTERFACE:${EIGEN_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${SEG_ROOT}/3rdparty
)

target_link_libraries(seg
  PUBLIC
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
    Threads::Threads
  PRIVATE
    seg_imgui
    $<$<AND:$<BOOL:${USE_LOG}>,$<BOOL:${USE_SPDLOG}>>:spdlog::spdlog>
)

if(USE_LOG AND USE_SPDLOG)
  target_compile_definitions(seg PRIVATE SPDLOG)
endif()

if(USE_OPENCV)
  target_include_directories(seg PUBLIC ${OpenCV_INCLUDE_DIRS})
  target_compile_definitions(seg PUBLIC OPENCV)
  target_link_libraries(seg PUBLIC ${OpenCV_LIBS})
endif()

# examples ===============================================
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
