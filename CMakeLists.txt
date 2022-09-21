CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(Sand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp sandbox_core.cpp util.cpp material_sand.cpp material_water.cpp material_iron.cpp)
