cmake_minimum_required(VERSION 3.10)
project(NIZK)

set(CMAKE_CXX_STANDARD 11)

set(INC_DIR1 /usr/local/include)
set(INC_DIR2 /usr/local/include/pbc)
set(INC_DIR3 /home/swj/Tools/nacl-20110221/build/swjT460p/include/amd64)
set(LINK_DIR1 /usr/local/lib)
set(LINK_DIR2 /home/swj/Tools/nacl-20110221/build/swjT460p/lib/amd64)

include_directories(${INC_DIR1} ${INC_DIR2} ${INC_DIR3})
link_directories(${LINK_DIR1} ${LINK_DIR2})
link_libraries(pbc nacl gmp)

add_executable(NIZK main.cpp DSC.cpp DSC.h biligroup.cpp biligroup.h account.cpp account.h verifier.cpp verifier.h)

target_link_libraries(NIZK pbc nacl gmp)