cmake_minimum_required(VERSION 3.0)
project(ecg)

###########################################################
## Find Section                                          ##
###########################################################
find_package(glfw3 REQUIRED)
SET(GLFW_LIBRARIES glfw)

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_definitions(-DOPENCV_VERSION=${OpenCV_VERSION_MAJOR})

find_package(glm)
if (glm_FOUND)
  if (${GLM_VERSION} VERSION_GREATER 0.9.7)
    message ("Modern glm detected: ${GLM_VERSION}")
    add_definitions("-DMODERN_GLM")
  else()
    message ("Older glm detected: ${GLM_VERSION}")
  endif()
else()
  message ("")
  message (" ==> Continuing without glm found as a package (this should work if glm is installed)")
  message ("")
endif()

###########################################################
## Settings Section                                      ##
###########################################################

add_definitions("-std=c++11 -g -O0 -pthread")
add_definitions("-DSHADERPATH=\"${CMAKE_SOURCE_DIR}/shader/\"")
add_definitions("-DMODELPATH=\"${CMAKE_SOURCE_DIR}/obj/\"")
add_definitions("-DTEXTUREPATH=\"${CMAKE_SOURCE_DIR}/textures/\"")
add_definitions("-DSCENEPATH=\"${CMAKE_SOURCE_DIR}/scenes//\"")

#Specifically set the project root folder in the includes (that allows us to use relative include pathes to the project root instead of pathes relative to the build unit)
include_directories(${CMAKE_SOURCE_DIR})

set(COMMON_LIBS glfw pthread ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} 
    ${OpenGL_LIBRARIES} ${OpenCV_LIBS} stdc++)

###########################################################
## SubDir Section                                        ##
###########################################################
add_subdirectory(gl)
add_subdirectory(utility)

###########################################################
## Binary Section                                        ##
###########################################################

add_executable(ecg_ex01_a ecg_ex01_a.cpp )
target_link_libraries(ecg_ex01_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex01_b ecg_ex01_b.cpp)
target_link_libraries(ecg_ex01_b ${COMMON_LIBS})

add_executable(ecg_ex01_c ecg_ex01_c.cpp)
target_link_libraries(ecg_ex01_c cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex02_a ecg_ex02_a.cpp )
target_link_libraries(ecg_ex02_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex02_b ecg_ex02_b.cpp )
target_link_libraries(ecg_ex02_b cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex03_a ecg_ex03_a.cpp )
target_link_libraries(ecg_ex03_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex03_b ecg_ex03_b.cpp )
target_link_libraries(ecg_ex03_b cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex03_c ecg_ex03_c.cpp )
target_link_libraries(ecg_ex03_c cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex03_d ecg_ex03_d.cpp )
target_link_libraries(ecg_ex03_d cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex04_a ecg_ex04_a.cpp )
target_link_libraries(ecg_ex04_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex04_b ecg_ex04_b.cpp )
target_link_libraries(ecg_ex04_b cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex04_c ecg_ex04_c.cpp )
target_link_libraries(ecg_ex04_c cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex05_a ecg_ex05_a.cpp )
target_link_libraries(ecg_ex05_a cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex06_a ecg_ex06_a.cpp)
target_link_libraries(ecg_ex06_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex06_b ecg_ex06_b.cpp)
target_link_libraries(ecg_ex06_b cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex06_c ecg_ex06_c.cpp)
target_link_libraries(ecg_ex06_c cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex07_a ecg_ex07_a.cpp)
target_link_libraries(ecg_ex07_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex07_b ecg_ex07_b.cpp)
target_link_libraries(ecg_ex07_b cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex08_a ecg_ex08_a.cpp)
target_link_libraries(ecg_ex08_a cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex08_b ecg_ex08_b.cpp)
target_link_libraries(ecg_ex08_b cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex08_c ecg_ex08_c.cpp)
target_link_libraries(ecg_ex08_c cg2gl cg2util ${COMMON_LIBS})

add_executable(ecg_ex08_c_nodef ecg_ex08_c_nodef.cpp)
target_link_libraries(ecg_ex08_c_nodef cg2gl cg2util ${COMMON_LIBS})
######

add_executable(ecg_ex09_a ecg_ex09_a.cpp)
target_link_libraries(ecg_ex09_a cg2gl cg2util ${COMMON_LIBS})
