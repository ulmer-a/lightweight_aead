# Optimized RISC-V Implementations for Ascon and ISAP-A

## Build Configuration

To build the project, you will need **CMake**. A few configuration options
are available:

* `-DENABLE_TESTING=[ON|OFF]`: Build unit and integration tests as well
as Benchmarks tests. Default is OFF.
* `-DCMAKE_BUILD_TYPE=[Debug|Release]`: Default is **Release**.
* `-DTARGET_ARCH=[rv32|rv32_zbkb|rv64|rv64_zbkb]`: Choose the target
architecture to build for.

It is possible to pull in features as they are needed:

* `-DENABLE_ASCON_128=[ON|OFF]` Build with support for Ascon128. Default is ON.
* `-DENABLE_ASCON_128A=[ON|OFF]` Build with support for Ascon128a. Default is ON.
* `-DENABLE_ISAP_A_128=[ON|OFF]` Build with support for ISAP-A-128. Default is ON.
* `-DENABLE_ISAP_A_128A=[ON|OFF]` Build with support for ISAP-A-128a. Default is ON.

RISC-V specific configuration options. These are only required if the target
architecture is RISC-V:

* `-DTOOLCHAIN_PATH=<path>`: Path where the RISC-V GCC toolchain is located. 
* `-DSPIKE_PATH=<path>`: Path to the Spike ISA Simulator binary. This will be
used by `ctest` to execute the test suite.
* `-DPROXY_KERNEL_PATH`: Path to the `pk` (proxy kernel) binary. This will be
used together with Spike to execute the test suite. Make sure that this points
to a pk binary that corresponds to the target architecture.

Example building everything and running tests (of course please adjust
your toolchain paths!):

```
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTING=ON \
    -DTARGET_ARCH=rv32_zbkb \
    -DTOOLCHAIN_PATH=/opt \
    -DSPIKE_PATH=/opt/bin/spike \
    -DPROXY_KERNEL_PATH=/opt/pk/riscv64-unknown-elf/bin/pk
$ ctest . -V
```


## Usage

### Usage in another project

Add this directory as a subdirectory in CMake. In your CMakeLists, just link
against all Ascon/ISAP targets you need (choices are `ascon_128`, `ascon_128a`,
`isap_a_128`, `isap_a_128a`. Do not forget to add the include directories that
can be found in the `ASCON_INCLUDES` and `ISAP_INCLUDES` CMake variables.

Example:

```
add_executable(MyProject ...)

target_link_libraries(MyProject PRIVATE ascon_128 isap_a_128a)
target_include_directories(MyProject PRIVATE ${ASCON_INCLUDES} ${ISAP_INCLUDES})
```

### Run Tests

You can run the tests after a successful build in the build directory by typing

```
ctest . -V
```
