cmake_minimum_required(VERSION 3.14)

if(CMAKE_TESTING_ENABLED)

    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/bench_*.c")
    add_executable(ascon_bench
        ${TEST_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/bench.c"
    )

    target_include_directories(ascon_bench PRIVATE
        ${ASCON_INCLUDES} ${ASCON_COMMON_INCLUDES}
    )
    target_link_libraries(ascon_bench PRIVATE ascon_128 ascon_128a testing)
    target_include_directories(ascon_bench PRIVATE ${TESTING_INCLUDES})

    add_test(
        NAME AsconBenchmarks
        COMMAND ${SPIKE_PATH} --isa=${RV_ISA} ${PK_PATH} $<TARGET_FILE:ascon_bench>
    )

endif()
