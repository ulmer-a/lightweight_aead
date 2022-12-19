cmake_minimum_required(VERSION 3.14)

set(PRINTF_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(printf STATIC
    "${PRINTF_INCLUDES}/_internal/printf.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/printf.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/printf.c"
)
target_include_directories(printf PRIVATE ${PRINTF_INCLUDES})

set(PRINTF_INCLUDES ${PRINTF_INCLUDES} PARENT_SCOPE)