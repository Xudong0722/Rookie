file(GLOB_RECURSE east_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(east_shared SHARED ${east_sources})