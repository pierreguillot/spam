cmake_minimum_required(VERSION 2.8)

project(spam)

set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/)
set(CMAKE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/)
if(${APPLE})
	set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
	set(CMAKE_SHARED_LIBRARY_SUFFIX ".pd_darwin")
elseif(${UNIX})
	set(CMAKE_SHARED_LIBRARY_SUFFIX ".pd_linux")
else()
	set(CMAKE_SHARED_LIBRARY_SUFFIX  ".dll")
	link_directories(${PROJECT_SOURCE_DIR}/pdwinlib)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS /wd4091 /wd4996")
endif()

include_directories(${PROJECT_SOURCE_DIR}/pd/src/)



set(SPAMTILDESRC
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.utils.h
	${PROJECT_SOURCE_DIR}/src/spam.utils.c
	${PROJECT_SOURCE_DIR}/src/spam_tilde.c)
source_group(src FILES ${SPAMTILDESRC})
add_library(spam_tilde SHARED ${SPAMTILDESRC})
set_target_properties(spam_tilde PROPERTIES LIBRARY_OUTPUT_NAME spam.process~)
if(${WIN32})
		target_link_libraries(spam_tilde pd)
endif()

set(SPAMINSRC
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.io.h
	${PROJECT_SOURCE_DIR}/src/spam.io.c
	${PROJECT_SOURCE_DIR}/src/spam.in.c)
source_group(src FILES ${SPAMINSRC})
add_library(spam_in SHARED ${SPAMINSRC})
set_target_properties(spam_in PROPERTIES LIBRARY_OUTPUT_NAME spam.in)
if(${WIN32})
		target_link_libraries(spam_in pd)
endif()

set(SPAMOUTSRC
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.io.h
	${PROJECT_SOURCE_DIR}/src/spam.io.c
	${PROJECT_SOURCE_DIR}/src/spam.out.c)
source_group(src FILES ${SPAMOUTSRC})
add_library(spam_out SHARED ${SPAMOUTSRC})
set_target_properties(spam_out PROPERTIES LIBRARY_OUTPUT_NAME spam.out)
if(${WIN32})
		target_link_libraries(spam_out pd)
endif()

set(SPAMINTILDESRC
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.io.h
	${PROJECT_SOURCE_DIR}/src/spam.io.c
	${PROJECT_SOURCE_DIR}/src/spam.in_tilde.c)
source_group(src FILES ${SPAMINTILDESRC})
add_library(spam_in_tilde SHARED ${SPAMINTILDESRC})
set_target_properties(spam_in_tilde PROPERTIES LIBRARY_OUTPUT_NAME spam.in~)
if(${WIN32})
		target_link_libraries(spam_in_tilde pd)
endif()

set(SPAMOUTTILDESRC
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.io.h
	${PROJECT_SOURCE_DIR}/src/spam.io.c
	${PROJECT_SOURCE_DIR}/src/spam.out_tilde.c)
source_group(src FILES ${SPAMOUTTILDESRC})
add_library(spam_out_tilde SHARED ${SPAMOUTTILDESRC})
set_target_properties(spam_out_tilde PROPERTIES LIBRARY_OUTPUT_NAME spam.out~)
if(${WIN32})
		target_link_libraries(spam_out_tilde pd)
endif()

set(SPAMSRC
	${PROJECT_SOURCE_DIR}/src/spam.def.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.h
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.io.h
	${PROJECT_SOURCE_DIR}/src/spam.io.c
	${PROJECT_SOURCE_DIR}/src/spam.utils.h
	${PROJECT_SOURCE_DIR}/src/spam.utils.c
	${PROJECT_SOURCE_DIR}/src/spam_tilde.c
	${PROJECT_SOURCE_DIR}/src/spam.in.c
	${PROJECT_SOURCE_DIR}/src/spam.out.c
	${PROJECT_SOURCE_DIR}/src/spam.in_tilde.c
	${PROJECT_SOURCE_DIR}/src/spam.out_tilde.c
	${PROJECT_SOURCE_DIR}/src/spam.c)
source_group(src FILES ${SPAMSRC})
add_library(spam SHARED ${SPAMSRC})
set_target_properties(spam PROPERTIES LIBRARY_OUTPUT_NAME spam)
if(${WIN32})
		target_link_libraries(spam pd)
endif()
