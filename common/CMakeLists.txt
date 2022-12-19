cmake_minimum_required(VERSION 3.15)

set(ASCON_COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(ascon_common STATIC
    ${ASCON_COMMON_INCLUDES}/_internal/common.h
    ${ASCON_COMMON_INCLUDES}/_internal/state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/round_constants.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interleaving.c
)

set(ASCON_COMMON_INCLUDES ${ASCON_COMMON_INCLUDES} PARENT_SCOPE)
