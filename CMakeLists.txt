cmake_minimum_required(VERSION 2.8)

project(spam)
include(pd.cmake)

include_directories(${PROJECT_SOURCE_DIR}/pd/src/)
if(${MSYS})
  link_directories(${PROJECT_SOURCE_DIR}/pdwinlib)
endif()

set_external_path("${PROJECT_SOURCE_DIR}/spam/")
set(SPAM_TILDE_SRC ${PROJECT_SOURCE_DIR}/src/spam_tilde.c
	${PROJECT_SOURCE_DIR}/src/spam.tie.c
	${PROJECT_SOURCE_DIR}/src/spam.utils.h
  ${PROJECT_SOURCE_DIR}/src/spam.tie.h
  ${PROJECT_SOURCE_DIR}/src/spam.utils.c)
add_external(spam_tilde spam~ "${SPAM_TILDE_SRC}")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/")
add_external(spam_loadbang spam.loadbang ${SRC_DIR}/spam.loadbang.c)
add_external(spam_connect spam.connect ${SRC_DIR}/spam.connect.c)
add_external(spam_in spam.in ${SRC_DIR}/spam.in.c)

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
#add_external(spam spam "${SPAM_SRC}")
