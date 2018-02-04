cmake_minimum_required(VERSION 2.8)
 
PROJECT(miPrimerEjemploEsUnCono)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


INCLUDE_DIRECTORIES(
/usr/local/include/bullet/
)

LINK_LIBRARIES(
 BulletDynamics BulletCollision LinearMath
)



IF (WIN32)
	ADD_EXECUTABLE(miPrimerEjemploEsUnCono 
		miPrimerEjemploEsUnCono.cxx
		${BULLET_PHYSICS_SOURCE_DIR}/usr/include/bullet
		target_link_libraries(miPrimerEjemploEsUnCono ${VTK_LIBRARIES})

	)
ELSE()
	ADD_EXECUTABLE(miPrimerEjemploEsUnCono 
		miPrimerEjemploEsUnCono.cxx
	)
	target_link_libraries(miPrimerEjemploEsUnCono ${VTK_LIBRARIES})
	

ENDIF()



IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
