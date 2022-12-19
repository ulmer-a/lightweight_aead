cmake_minimum_required(VERSION 3.14)

set(ASCON_P_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(ascon_p STATIC
    "${ASCON_P_INCLUDES}/ascon/permutation.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/perm_api.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/s_box.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/linear_layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/permutation.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/perm_rv32.c"
)

target_include_directories(ascon_p PRIVATE
    ${ASCON_COMMON_INCLUDES}
    ${ASCON_P_INCLUDES}
)

if(CMAKE_TESTING_ENABLED)

    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c")
    add_executable(perm_tests
        ${TEST_SOURCES}
    )

    target_include_directories(perm_tests PRIVATE
        ${ASCON_COMMON_INCLUDES}
        ${ASCON_P_INCLUDES}
        ${TESTING_INCLUDES}
    )
    target_link_libraries(perm_tests PRIVATE ascon_p testing)

    add_test(
        NAME PermutationTests
        COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:perm_tests>
    )

endif()

set(ASCON_P_INCLUDES ${ASCON_P_INCLUDES} PARENT_SCOPE)