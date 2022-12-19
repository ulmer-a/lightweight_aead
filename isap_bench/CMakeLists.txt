cmake_minimum_required(VERSION 3.14)

if(CMAKE_TESTING_ENABLED)

    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/bench_*.c")
    add_executable(isap_bench
        ${TEST_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/bench.c"
    )

    target_include_directories(isap_bench PRIVATE ${ISAP_INCLUDES} ${ASCON_COMMON_INCLUDES})
    target_link_libraries(isap_bench PRIVATE isap_a_128 isap_a_128a testing)
    target_include_directories(isap_bench PRIVATE ${TESTING_INCLUDES})

    add_test(
        NAME IsapBenchmarks
        COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:isap_bench>
    )

endif()
