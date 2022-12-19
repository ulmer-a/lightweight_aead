cmake_minimum_required(VERSION 3.14)

set(ASCON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(ASCON_SOURCES
    ${ASCON_INCLUDES}/ascon/ascon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/word.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interleaving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ascon_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ascon_encrypt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ascon_decrypt.c
)

if (CMAKE_TESTING_ENABLED OR ENABLE_ASCON_128)
    message(STATUS "Building with Ascon-128")
    add_library(ascon_128 STATIC ${ASCON_SOURCES})
    target_include_directories(ascon_128 PRIVATE
        ${ASCON_INCLUDES} ${ASCON_COMMON_INCLUDES} ${ASCON_P_INCLUDES}
    )
    target_compile_definitions(ascon_128 PRIVATE ASCON_128)
    target_link_libraries(ascon_128 PRIVATE ascon_p)
endif()

if (CMAKE_TESTING_ENABLED OR ENABLE_ASCON_128A)
    message(STATUS "Building with Ascon-128a")
    add_library(ascon_128a STATIC ${ASCON_SOURCES})
    target_include_directories(ascon_128a PRIVATE
        ${ASCON_INCLUDES} ${ASCON_COMMON_INCLUDES} ${ASCON_P_INCLUDES}
    )
    target_compile_definitions(ascon_128a PRIVATE ASCON_128A)
    target_link_libraries(ascon_128a PRIVATE ascon_p)
endif()

# if(CMAKE_TESTING_ENABLED)

#     file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c")
#     add_executable(ascon_tests
#         ${TEST_SOURCES}
#     )

#     target_link_libraries(ascon_tests PRIVATE ascon testing)

#     add_test(
#         NAME AsconAeadTests
#         COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:ascon_tests>
#     )

# endif()

set(ASCON_INCLUDES ${ASCON_INCLUDES} PARENT_SCOPE)
