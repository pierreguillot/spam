cmake_minimum_required(VERSION 2.8)

set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(CMAKE_MACOSX_RPATH OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.6)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

include(pd.build/pd.cmake)

project(spam)
set_pd_sources("${PROJECT_SOURCE_DIR}/pd/src/")
set_pd_external_path("${PROJECT_SOURCE_DIR}/spam/")

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-padded -Wno-reserved-id-macro -Wno-disabled-macro-expansion -Wno-missing-variable-declarations")
endif()

include_directories(${PROJECT_SOURCE_DIR}/pd/src/)
if(${WIN32})
  link_directories(${PROJECT_SOURCE_DIR}/pdwinlib)
endif()


set(SPAM_SRC_DIR "${PROJECT_SOURCE_DIR}/src/")

file(GLOB SPAMTILDESRC
  ${SPAM_SRC_DIR}/spam.tie.h ${SPAM_SRC_DIR}/spam.tie.c
  ${SPAM_SRC_DIR}/spam.utils.h ${SPAM_SRC_DIR}/spam.utils.c ${SPAM_SRC_DIR}/spam_tilde.c)
source_group(src FILES ${SPAMTILDESRC})
add_pd_external(spam_tilde spam~ "${SPAMTILDESRC}")

file(GLOB SPAMCURRENTSRC
  ${SPAM_SRC_DIR}/spam.tie.h ${SPAM_SRC_DIR}/spam.tie.c
  ${SPAM_SRC_DIR}/spam.io.h ${SPAM_SRC_DIR}/spam.io.c ${SPAM_SRC_DIR}/spam.in.c)
source_group(src FILES ${SPAMCURRENTSRC})
add_pd_external(spam_in spam.in "${SPAMCURRENTSRC}")

file(GLOB SPAMCURRENTSRC
  ${SPAM_SRC_DIR}/spam.tie.h ${SPAM_SRC_DIR}/spam.tie.c
  ${SPAM_SRC_DIR}/spam.io.h ${SPAM_SRC_DIR}/spam.io.c ${SPAM_SRC_DIR}/spam.out.c)
source_group(src FILES ${SPAMCURRENTSRC})
add_pd_external(spam_out spam.out "${SPAMCURRENTSRC}")

file(GLOB SPAMCURRENTSRC
    ${SPAM_SRC_DIR}/spam.tie.h ${SPAM_SRC_DIR}/spam.tie.c
    ${SPAM_SRC_DIR}/spam.io.h ${SPAM_SRC_DIR}/spam.io.c ${SPAM_SRC_DIR}/spam.in_tilde.c)
source_group(src FILES ${SPAMCURRENTSRC})
add_pd_external(spam_in_tilde spam.in~ "${SPAMCURRENTSRC}")

file(GLOB SPAMCURRENTSRC
    ${SPAM_SRC_DIR}/spam.tie.h ${SPAM_SRC_DIR}/spam.tie.c
    ${SPAM_SRC_DIR}/spam.io.h ${SPAM_SRC_DIR}/spam.io.c ${SPAM_SRC_DIR}/spam.out_tilde.c)
source_group(src FILES ${SPAMCURRENTSRC})
add_pd_external(spam_out_tilde spam.out~ "${SPAMCURRENTSRC}")

file(GLOB SPAMCURRENTSR ${SPAM_SRC_DIR}/spam.connect.c)
source_group(src FILES ${SPAMCURRENTSR})
add_pd_external(spam_connect spam.connect "${SPAMCURRENTSR}")
