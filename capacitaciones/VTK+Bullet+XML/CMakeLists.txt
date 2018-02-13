cmake_minimum_required(VERSION 2.8)
 
PROJECT(miPrimerEjemploEsUnCono)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include(FindBullet)

add_definitions     ( ${BULLET_DEFINITIONS} )
include_directories ( ${BULLET_INCLUDE_DIRS} )
link_directories    ( ${BULLET_LIBRARY_DIRS} )
message(STATUS "${BULLET_FOUND}")
message(STATUS "${BULLET_INCLUDE_DIRS}")
message(STATUS "${BULLET_LIBRARIES}")

IF (WIN32)
	ADD_EXECUTABLE(miPrimerEjemploEsUnCono 
		miPrimerEjemploEsUnCono.cxx
		${BULLET_PHYSICS_SOURCE_DIR}
		target_link_libraries(miPrimerEjemploEsUnCono ${VTK_LIBRARIES} ${BULLET_LIBRARIES})

	)
ELSE()
	ADD_EXECUTABLE(miPrimerEjemploEsUnCono 
		miPrimerEjemploEsUnCono.cxx
	)
	target_link_libraries(miPrimerEjemploEsUnCono ${VTK_LIBRARIES} ${BULLET_LIBRARIES})
	

ENDIF()



IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(miPrimerEjemploEsUnCono PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
