cmake_minimum_required(VERSION 3.14)

if(CMAKE_TESTING_ENABLED)

    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.c")
    add_executable(ascon_integration
        ${TEST_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/tests.c"
    )

    target_include_directories(ascon_integration PRIVATE
        ${ASCON_INCLUDES} ${TESTING_INCLUDES} ${ASCON_COMMON_INCLUDES}
    )
    target_link_libraries(ascon_integration PRIVATE ascon_128 ascon_128a testing)

    add_test(
        NAME AsconIntegrationTests
        COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:ascon_integration>
    )

endif()
