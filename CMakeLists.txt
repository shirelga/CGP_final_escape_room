cmake_minimum_required(VERSION 3.3)
project(final_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
set(SOURCE_FILES main.cpp Camera.cpp ShaderIO.cpp Object.cpp Object.h Cube.cpp Cube.h Mesh.cpp Mesh.h Model.cpp Model.h)

set(OpenCV_DIR /usr/local/opt/opencv3)
set(Local_DIR /usr/local/lib)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(glm REQUIRED)
if (glm_FOUND)
    include_directories(${glm_INCLUDE_DIRS})
    link_libraries(${glm_LIBRARIES})
endif()

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIRS})
    link_libraries(${GLUT_LIBRARIES})
endif()

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(OpenCV HINTS ${OpenCV_DIR} )
if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIR})
    link_libraries(${OpenCV_LIBRARIES})
endif()

find_package(FREETYPE REQUIRED)
if (FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    link_libraries(${FREETYPE_LIBRARIES})
endif()

add_executable(final_project ${SOURCE_FILES})
target_link_libraries(final_project -L${Local_DIR} -lOpenMeshCore dl)