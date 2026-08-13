cmake_minimum_required(VERSION 3.14)
project(mi_api_crow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(JWT_BUILD_WITH_PICOJSON OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_WITH_NLOHMANN_JSON ON CACHE BOOL "" FORCE)

set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.3.3
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
    GIT_REPOSITORY https://github.com/Nightmare115117/libbcrypt.git
    GIT_TAG master
)

FetchContent_MakeAvailable(bcrypt)

FetchContent_Declare(
    hiredis
    GIT_REPOSITORY https://github.com/Nightmare115117/hiredis.git
    GIT_TAG master
)
FetchContent_MakeAvailable(hiredis)

# redis-plus-plus espera este layout:
file(MAKE_DIRECTORY
    "${hiredis_SOURCE_DIR}/hiredis"
)

if(NOT EXISTS "${hiredis_SOURCE_DIR}/hiredis/hiredis.h")
    file(CREATE_LINK
        "${hiredis_SOURCE_DIR}/hiredis.h"
        "${hiredis_SOURCE_DIR}/hiredis/hiredis.h"
        SYMBOLIC
    )
endif()

# --- ESTO ES LO NUEVO ---
# Le decimos a CMake dónde buscar el header y la lib de hiredis,
# ya que no están en una ruta "estándar" del sistema sino en _deps.
list(APPEND CMAKE_PREFIX_PATH "${hiredis_SOURCE_DIR}")
set(HIREDIS_HEADER "${hiredis_SOURCE_DIR}" CACHE PATH "" FORCE)

set(REDIS_PLUS_PLUS_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(REDIS_PLUS_PLUS_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(REDIS_PLUS_PLUS_BUILD_SHARED OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    redis-plus-plus
    GIT_REPOSITORY https://github.com/Nightmare115117/redis-plus-plus.git
    GIT_TAG master
)

FetchContent_MakeAvailable(redis-plus-plus)

if(TARGET redis++_static)
    target_compile_options(redis++_static PRIVATE -Wno-error=maybe-uninitialized)
endif()
if(TARGET hiredis)
    target_compile_options(hiredis PRIVATE -Wno-error=maybe-uninitialized)
endif()

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/DBConfig/*.cpp")
add_executable(mi_api ${SOURCES})

target_compile_definitions(mi_api PRIVATE JWT_DISABLE_PICOJSON)

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
nlohmann_json::nlohmann_json
bcrypt
redis++::redis++_static)