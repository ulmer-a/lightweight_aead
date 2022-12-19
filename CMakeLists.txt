cmake_minimum_required(VERSION 3.14)

option(ENABLE_ASCON_128 "Build with Ascon-128" ON)
option(ENABLE_ASCON_128A "Build with Ascon-128a" ON)
option(ENABLE_ISAP_A_128 "Build with ISAP-A-128" ON)
option(ENABLE_ISAP_A_128A "Build with ISAP-A-128a" ON)

option(SYS_VERILATOR_IBEX "Build for Verilator/Ibex simulation target" OFF)

option(ENABLE_BENCH "Build Benchmarks" OFF)
option(ENABLE_TESTING "Build Unit and Integration Tests" OFF)

option(OPT_SPEED "Optimize specifically for performance" ON)
option(OPT_SIZE "Optimize specifically for binary size" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED TOOLCHAIN_PATH)
    set(TOOLCHAIN_PATH "/opt")
endif()

if(NOT DEFINED SPIKE_PATH)
    set(SPIKE_PATH "/home/alex/opt/spike/bin/spike")
endif()

if(ENABLE_TESTING OR ENABLE_BENCH)
    enable_testing()
endif()

if(DEFINED OPT_SPEED)
    add_compile_definitions(OPT_SPEED)
endif()

if(CMAKE_TESTING_ENABLED)
    add_compile_definitions(TESTING TESTING_ENABLED=1)
else()
    add_compile_definitions(TESTING_ENABLED=0)
endif()

if(TARGET_ARCH STREQUAL "rv32")
    add_compile_definitions(__RV__ __RV32__ __BI32__ TEST_FREESTANDING)
    set(TARGET_TRIPLE "riscv64-unknown-elf")
    set(RV_BASE_ISA "rv32im")
    set(RV_ISA "rv32im")
    set(RV_ABI "ilp32")
    set(TARGET_TYPE elf32-littleriscv)
    set(PK_PATH "/home/alex/opt/pk32/riscv32-unknown-elf/bin/pk")

elseif(TARGET_ARCH STREQUAL "rv32_zbkb")
    add_compile_definitions(__RV__ __RV32__ __BI32__ TEST_FREESTANDING
        RV_HAVE_ROR HAVE_ZIP HAVE_REV8 RV_HAVE_LOGIC_WITH_NEGATE HAVE_ZBKB)
    set(TARGET_TRIPLE "riscv64-unknown-elf")
    set(RV_BASE_ISA "rv32im")
    set(RV_ISA "rv32im_zbkb")
    set(RV_ABI "ilp32")
    set(TARGET_TYPE elf32-littleriscv)
    set(PK_PATH "/home/alex/opt/pk32/riscv32-unknown-elf/bin/pk")

elseif(TARGET_ARCH STREQUAL "rv64")
    add_compile_definitions(__RV__ __RV64__ TEST_FREESTANDING)
    set(TARGET_TRIPLE "riscv64-unknown-elf")
    set(RV_BASE_ISA "rv64imac")
    set(RV_ISA "rv64imac")
    set(RV_ABI "lp64")
    set(TARGET_TYPE elf64-littleriscv)
    set(PK_PATH "/home/alex/opt/pk/riscv64-unknown-elf/bin/pk")

elseif(TARGET_ARCH STREQUAL "rv64_zbkb")
    add_compile_definitions(__RV__ __RV64__ TEST_FREESTANDING
        RV_HAVE_ROR HAVE_ZIP HAVE_REV8 RV_HAVE_LOGIC_WITH_NEGATE HAVE_ZBKB)
    set(TARGET_TRIPLE "riscv64-unknown-elf")
    set(RV_BASE_ISA "rv64imac")
    set(RV_ISA "rv64imac_zbkb")
    set(RV_ABI "lp64")
    set(TARGET_TYPE elf64-littleriscv)
    set(PK_PATH "/home/alex/opt/pk/riscv64-unknown-elf/bin/pk")

else()
    message(FATAL_ERROR "Please set -DTARGET_ARCH=<arch> parameter")
endif()

if(DEFINED PROXY_KERNEL_PATH)
    set(PK_PATH "/home/alex/opt/pk/riscv64-unknown-elf/bin/pk")
endif()

if(SYS_VERILATOR_IBEX)
    add_compile_definitions(SYS_VERILATOR)
endif()

# which compilers to use for C and C++
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLE}-g++)

message(STATUS "Building for ${RV_ISA}-${RV_ABI} [${CMAKE_BUILD_TYPE}]")

project(lightweight_aead
    VERSION 0.1
    LANGUAGES C ASM
)

add_compile_options(
    -march=${RV_ISA}
    -mabi=${RV_ABI}
)

add_link_options(
    -march=${RV_ISA}
    -mabi=${RV_ABI}
    -nostdlib
)

if(SYS_VERILATOR_IBEX)
    add_link_options(
        -T "${CMAKE_CURRENT_SOURCE_DIR}/libtest/verilator_ibex.ld"
    )
endif()

link_libraries(
    "${TOOLCHAIN_PATH}/lib/gcc/riscv64-unknown-elf/11.1.0/${RV_BASE_ISA}/${RV_ABI}/libgcc.a"
)

# Testing Framework
add_subdirectory(common)        # Common Headers and Data
add_subdirectory(printf)        # Freestanding 'printf()' implementation
add_subdirectory(libtest)       # Unit Testing Framework

# Ascon Implementation, Tests and Benchmarks
add_subdirectory(ascon-p)       # Ascon Permutation implementation
add_subdirectory(ascon)         # Ascon AEAD implementation
add_subdirectory(ascon_tests)   # Ascon integration tests
add_subdirectory(ascon_bench)   # Ascon benchmarks

# ISAP Implementation, Tests and Benchmarks
add_subdirectory(isap)          # ISAP AEAD implementation
add_subdirectory(isap_tests)    # ISAP integration tests
add_subdirectory(isap_bench)    # ISAP benchmarks
