cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


project(Graduation-work)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")
set(INCLUDE_DIR include)
set(EVO_HEADERS include/genetic)

function(build_src SRC_NAME)
    set(TARGET_DIR "src/${SRC_NAME}.cpp")
    add_executable(${SRC_NAME} ${TARGET_DIR})
    target_include_directories(${SRC_NAME} PUBLIC ${INCLUDE_DIR} ${EVO_HEADERS})
endfunction(build_src)

#build_src(evo.cpp)

add_executable(evo src/evo.cpp)
target_include_directories(evo PUBLIC ${INCLUDE_DIR} ${EVO_HEADERS})