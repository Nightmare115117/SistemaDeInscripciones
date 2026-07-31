cmake_minimum_required(VERSION 3.14)
project(mi_api_crow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(JWT_BUILD_WITH_PICOJSON OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_WITH_NLOHMANN_JSON ON CACHE BOOL "" FORCE)

set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)

FetchContent_MakeAvailable(Crow)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
    jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG v0.7.2
)

FetchContent_MakeAvailable(jwt-cpp)

FetchContent_Declare(
    bcrypt
    GIT_REPOSITORY https://github.com/trusch/libbcrypt.git
    GIT_TAG master
)

FetchContent_MakeAvailable(bcrypt)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/DBConfig/*.cpp")
add_executable(mi_api ${SOURCES})
target_include_directories(mi_api PRIVATE 
src 
${PQXX_INCLUDE_DIRS} 
${bcrypt_SOURCE_DIR}/include)
target_link_libraries(mi_api PRIVATE 
Crow::Crow 
Threads::Threads 
${PQXX_LIBRARIES} pq 
jwt-cpp::jwt-cpp
OpenSSL::Crypto
nlohmann_json::nlohmann_json)