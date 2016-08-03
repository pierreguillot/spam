cmake_minimum_required(VERSION 2.8)

project(spam)
include(pd.cmake)

option(CONF "Build for configuration" "universal")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/spam/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/spam/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/spam/)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/spam/)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/spam/)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/spam/)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

include_directories(${PROJECT_SOURCE_DIR}/pd/src/)
if((${UNIX}) AND (${CONF} EQUAL 32))
  set(CMAKE_CXX_FLAGS -m32)
elseif(${MSYS})
  link_directories(${PROJECT_SOURCE_DIR}/pdwinlib)
endif()

set(SPAM_TILDE_SRC ${PROJECT_SOURCE_DIR}/src/spam_tilde.c
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.utils.h
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h
  ${PROJECT_SOURCE_DIR}/src/spam.utils.c)
add_external(spam_tilde spam~ "${SPAM_TILDE_SRC}")

set(SPAM_IN_SRC ${PROJECT_SOURCE_DIR}/src/spam.in.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h)
add_external(spam_in spam.in "${SPAM_IN_SRC}")

set(SPAM_OUT_SRC ${PROJECT_SOURCE_DIR}/src/spam.out.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.c
  ${PROJECT_SOURCE_DIR}/src/spam.io.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h
  ${PROJECT_SOURCE_DIR}/src/spam.io.h)
add_external(spam_out spam.out "${SPAM_OUT_SRC}")

set(SPAM_IN_TILDE_SRC ${PROJECT_SOURCE_DIR}/src/spam.in_tilde.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.c
  ${PROJECT_SOURCE_DIR}/src/spam.io.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h
  ${PROJECT_SOURCE_DIR}/src/spam.io.h)
add_external(spam_in_tilde spam.in~ "${SPAM_IN_TILDE_SRC}")

set(SPAM_OUT_TILDE_SRC ${PROJECT_SOURCE_DIR}/src/spam.out_tilde.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.c
  ${PROJECT_SOURCE_DIR}/src/spam.io.c
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h
  ${PROJECT_SOURCE_DIR}/src/spam.io.h)
add_external(spam_out_tilde spam.out~ "${SPAM_OUT_TILDE_SRC}")

add_external(spam_loadbang spam.loadbang ${PROJECT_SOURCE_DIR}/src/spam.loadbang.c)
add_external(spam_connect spam.connect ${PROJECT_SOURCE_DIR}/src/spam.connect.c)

set(SPAM_SRC
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
  ${PROJECT_SOURCE_DIR}/src/spam.connect.c
  ${PROJECT_SOURCE_DIR}/src/spam.loadbang.c
	${PROJECT_SOURCE_DIR}/src/spam.c)
add_external(spam spam "${SPAM_SRC}")
