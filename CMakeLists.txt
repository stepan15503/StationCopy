cmake_minimum_required(VERSION 3.10)
project(SpaceStationSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск nlohmann_json (если установлена системно)
# Если нет, можно просто положить json.hpp в папку проекта и убрать эти строки
find_package(nlohmann_json QUIET)

add_executable(station_sim 
    maintest.cpp 
    station.cpp 
    modules/basemodule.cpp 
    eventmanager.cpp
)

# Если используете старый GCC, может понадобиться линковка filesystem
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(station_sim stdc++fs)
endif()

if(nlohmann_json_FOUND)
    target_link_libraries(station_sim nlohmann_json::nlohmann_json)
endif()