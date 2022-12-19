cmake_minimum_required(VERSION 3.14)

if(CMAKE_TESTING_ENABLED)

    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.c")
    add_executable(isap_integration
        ${TEST_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/tests.c"
    )

    target_include_directories(isap_integration PRIVATE
        ${ISAP_INCLUDES} ${TESTING_INCLUDES} ${ASCON_COMMON_INCLUDES}
    )
    target_link_libraries(isap_integration PRIVATE isap_a_128 isap_a_128a testing)

    add_test(
        NAME IsapIntegrationTests
        COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:isap_integration>
    )

endif()
