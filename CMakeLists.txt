# +------------------------------------------------------------------+
# ¦  Projects Settings                                               ¦
# +------------------------------------------------------------------+
cmake_minimum_required (VERSION 2.8)
project (Taranis)

# +------------------------------------------------------------------+
# ¦  Compilation flags                                               ¦
# +------------------------------------------------------------------+
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-Wall)

# +------------------------------------------------------------------+
# ¦  Load libraries                                                  ¦
# +------------------------------------------------------------------+
# glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE STRING "" FORCE)
set(GLFW_INSTALL        OFF CACHE STRING "" FORCE)
add_subdirectory(ext/glfw)
include_directories(ext/glfw/include)

# glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(ext/glew)
include_directories(ext/glew/include ext/glew/src)

# glm
include_directories(ext/glm)

# STB
include_directories(ext/stb)

# imgui
include_directories(ext/imgui)

include_directories(include)

# GROUP OF FILES
SOURCE_GROUP( src REGULAR_EXPRESSION “.*/src/.*” )
SOURCE_GROUP( includes REGULAR_EXPRESSION “.*/includes/.*” )
SOURCE_GROUP( shaders REGULAR_EXPRESSION “.*/shaders/.*” )
SOURCE_GROUP( files REGULAR_EXPRESSION “.*/files/.*” )
#SOURCE_GROUP( externals REGULAR_EXPRESSION “.*/ext/.*” )




# +------------------------------------------------------------------+
# ¦  Build executable                                                ¦
# +------------------------------------------------------------------+
file(GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*)
add_executable(main ${source_files})
target_link_libraries(main glfw ${GLFW_LIBRARIES} libglew_static)

