cmake_minimum_required(VERSION 3.10)
project(NIZK)

set(CMAKE_CXX_STANDARD 11)

set(INC_DIR1 /usr/local/include)
set(INC_DIR2 /usr/local/include/pbc)
set(LINK_DIR /usr/local/lib)

include_directories(${INC_DIR1} ${INC_DIR2})
link_directories(${LINK_DIR})
link_libraries(pbc gmp)

add_executable(NIZK main.cpp DSC.cpp DSC.h biligroup.cpp biligroup.h account.cpp account.h verifier.cpp verifier.h)

target_link_libraries(NIZK pbc)