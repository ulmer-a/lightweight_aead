cmake_minimum_required(VERSION 3.14)

if(CMAKE_TESTING_ENABLED)

    set(TESTING_INCLUDES ${PRINTF_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(SYS_VERILATOR_IBEX)
        set(VERILATOR_CRT0
            "${CMAKE_CURRENT_SOURCE_DIR}/verilator_ibex/crt0.S"
            "${CMAKE_CURRENT_SOURCE_DIR}/verilator_ibex/common.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/verilator_ibex/common.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/verilator_ibex/regs.h"
        )
    endif()

    add_library(testing OBJECT
        "${CMAKE_CURRENT_SOURCE_DIR}/include/_internal/testing.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/crt0.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/state.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/getcycles.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/random.c"
        "${VERILATOR_CRT0}"
    )

    target_include_directories(testing PRIVATE
        ${TESTING_INCLUDES}
        ${ASCON_COMMON_INCLUDES}
    )

    target_link_libraries(testing PRIVATE printf ascon_common)

    set(TESTING_INCLUDES ${TESTING_INCLUDES} PARENT_SCOPE)

endif()
