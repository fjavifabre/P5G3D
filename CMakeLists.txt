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
SOURCE_GROUP( "src" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/.*" )
SOURCE_GROUP( "src\\camera" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/camera/.*" )
SOURCE_GROUP( "src\\light" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/light/.*" )
SOURCE_GROUP( "src\\mesh" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/mesh/.*" )
SOURCE_GROUP( "src\\renderer" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/renderer/.*" )
SOURCE_GROUP( "src\\shading" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/shading/.*" )
SOURCE_GROUP( "src\\utils" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/src/utils/.*" )

SOURCE_GROUP( "include" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/.*" )
SOURCE_GROUP( "include\\camera" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/camera/.*" )
SOURCE_GROUP( "include\\light" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/light/.*" )
SOURCE_GROUP( "include\\mesh" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/mesh/.*" )
SOURCE_GROUP( "include\\renderer" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/renderer/.*" )
SOURCE_GROUP( "include\\shading" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/shading/.*" )
SOURCE_GROUP( "include\\utils" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/include/utils/.*" )



# +------------------------------------------------------------------+
# ¦  Build executable                                                ¦
# +------------------------------------------------------------------+
file(GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/* ${PROJECT_SOURCE_DIR}/include/*)
add_executable(main ${source_files})
target_link_libraries(main glfw ${GLFW_LIBRARIES} libglew_static)

