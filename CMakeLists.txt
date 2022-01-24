cmake_minimum_required(VERSION 3.5)

project(pybindVirtualEnvTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(VENV_ROOT_PATH "/home/u1/Documents/FunCoding/learningPybind/tempVenvDir/py35venv") # my custom var for convenience
set(
    PYTHON_EXECUTABLE "${VENV_ROOT_PATH}/bin/python3"
    CACHE FILEPATH "python virtual environment executable")
message(STATUS "PYTHON_EXECUTABLE is now: ${PYTHON_EXECUTABLE}")    #DEBUG

#set($ENV{PYTHONEXECUTABLE} "${PYTHON_EXECUTABLE}")

#set(Python_VIRTUALENV FIRST)
#set(Python3_ROOT_DIR "${VENV_ROOT_PATH}")
#set(ENV{PYTHONPATH} "${VENV_ROOT_PATH}/lib/python3.5/site-packages")
#message(STATUS "ENV{PYTHONPATH} is now: $ENV{PYTHONPATH}")  #DEBUG
#set(ENV{PATH} "${VENV_ROOT_PATH}/lib/python3.9/venv/Scripts/;${VENV_ROOT_PATH}/venv/Lib")
#message(STATUS "PATH is now: $ENV{PATH}")   #DEBUG

add_subdirectory(extern/pybind11)

add_executable(pybindVirtualEnvTest main.cpp)
target_link_libraries(pybindVirtualEnvTest PRIVATE pybind11::embed)

target_compile_definitions(pybindVirtualEnvTest PRIVATE -DVENV_SITE_PACKAGES_PATH="${VENV_ROOT_PATH}/lib/python3.5/site-packages")

