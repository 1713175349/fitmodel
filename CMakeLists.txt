cmake_minimum_required(VERSION 3.10.0)
project(forcefit VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)


find_package(MPI REQUIRED)
# set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS mpi)

message(STATUS "Boost version: ${Boost_VERSION}")
#boost library
message(STATUS "Boost library: ${Boost_LIBRARIES}")
#boost include
message(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
set(LAMMPS_DIR /home/zln/nowproject/lammps-patch_14Dec2021)


include_directories(${LAMMPS_DIR}/src ${MPI_INCLUDE_PATH} ${BOOST_INCLUDE_DIRS} . src)
link_directories(${LAMMPS_DIR}/build)
aux_source_directory(./src src_list)
add_executable(forcefit main.cpp ${src_list})

target_link_libraries(forcefit lammps ${MPI_CXX_LIBRARIES} yaml-cpp nlopt m)

# add_executable(mpitest main_mpi.cpp)
# target_link_libraries(mpitest ${MPI_CXX_LIBRARIES} yaml-cpp nlopt m ${Boost_LIBRARIES})

find_package(Pagmo REQUIRED)


add_executable(psofit main_pso.cpp ${src_list})

target_link_libraries(psofit Pagmo::pagmo  lammps ${MPI_CXX_LIBRARIES} yaml-cpp nlopt m)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
