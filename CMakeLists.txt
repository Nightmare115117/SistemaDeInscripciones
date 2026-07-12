cmake_minimum_required(VERSION 3.14)
project(mi_api_crow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Crow)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/DBConfig/*.cpp")
add_executable(mi_api ${SOURCES})
target_include_directories(mi_api PRIVATE src ${PQXX_INCLUDE_DIRS})
target_link_libraries(mi_api PRIVATE Crow::Crow Threads::Threads ${PQXX_LIBRARIES})