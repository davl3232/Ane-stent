cmake_minimum_required(VERSION 2.8)
 
PROJECT(CilindroCayendo)
set(CMAKE_CXX_STANDARD 11)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include(FindBullet)

include_directories(${BULLET_INCLUDE_DIRS})
link_directories(${BULLET_LIBRARY_DIRS})

ADD_EXECUTABLE(main 
	src/main.cpp src/loaders/ModelLoader.cpp src/scene/SceneObject.cpp src/scene/Scene.cpp src/scene/SceneMotionState.cpp src/util/ToString.cpp
)
message(${BULLET_LIBRARIES})
target_link_libraries(main ${VTK_LIBRARIES} ${BULLET_LIBRARIES})

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(main PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(main PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(main PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
