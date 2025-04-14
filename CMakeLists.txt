cmake_minimum_required(VERSION 3.10)

project(AlignedVSMisaligned)

set(CMAKE_CXX_STANDARD 23)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "/arch:AVX")
else()
    set(CMAKE_CXX_FLAGS "-mavx -march=native")
endif()


add_executable(main main.cpp)

# Add the main executable as a test
enable_testing()
add_test(NAME MainTest3 COMMAND main 104)
add_test(NAME MainTest4 COMMAND main 1000)
add_test(NAME MainTest5 COMMAND main 10000)
add_test(NAME MainTest6 COMMAND main 100000)
add_test(NAME MainTest7 COMMAND main 1000000)
add_test(NAME MainTest8 COMMAND main 10000000)
add_test(NAME MainTest9 COMMAND main 75000000)
add_test(NAME MainTest10 COMMAND main 300000000)