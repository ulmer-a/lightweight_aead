cmake_minimum_required(VERSION 3.14)

set(ISAP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(ISAP_SOURCES
    ${ISAP_INCLUDES}/isap/isap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/isap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/isap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/isap_mac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/isap_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/isap_rk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interleaving.h
)

if (CMAKE_TESTING_ENABLED OR ENABLE_ISAP_A_128)
    message(STATUS "Building with ISAP-A-128")
    add_library(isap_a_128 STATIC ${ISAP_SOURCES})
    target_include_directories(isap_a_128 PRIVATE
        ${ISAP_INCLUDES} ${ASCON_COMMON_INCLUDES} ${ASCON_P_INCLUDES}
    )
    target_compile_definitions(isap_a_128 PRIVATE ISAP_128)
    target_link_libraries(isap_a_128 PRIVATE ascon_p)
endif()

if (CMAKE_TESTING_ENABLED OR ENABLE_ISAP_A_128A)
    message(STATUS "Building with ISAP-A-128a")
    add_library(isap_a_128a STATIC ${ISAP_SOURCES})
    target_include_directories(isap_a_128a PRIVATE
        ${ISAP_INCLUDES} ${ASCON_COMMON_INCLUDES} ${ASCON_P_INCLUDES}
    )
    target_compile_definitions(isap_a_128a PRIVATE ISAP_128A)
    target_link_libraries(isap_a_128a PRIVATE ascon_p)
endif()

set(ISAP_INCLUDES ${ISAP_INCLUDES} PARENT_SCOPE)