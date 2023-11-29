cmake_minimum_required(VERSION 3.18 FATAL_ERROR)


include(${CMAKE_SOURCE_DIR}/utils.cmake)

GetVersion()

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(nodeml_audio VERSION ${PACKAGE_VERSION})
message(STATUS "VERSION ${PACKAGE_VERSION}")

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DNAPI_VERSION=4)

include_directories(${CMAKE_JS_INC})

set(PORT_AUDIO_VERSION "190700_20210406" CACHE STRING "PortAudio Version")

DownloadPortAudio(${PORT_AUDIO_VERSION} ${CMAKE_SOURCE_DIR}/deps PORT_AUDIO_DIR)

DownloadLibSndFile(1.0.28 ${CMAKE_SOURCE_DIR}/deps LIB_SND_FILE_DIR)

message(STATUS "DIRS ${PORT_AUDIO_DIR} ${WAVE_DIR}")

find_package(SndFile)

add_subdirectory(${PORT_AUDIO_DIR} portaudio)

# Build a shared library named after the project from the files in `src/`
file(GLOB_RECURSE SOURCE_FILES "src/*.cc" "src/*.h")

# message(STATUS "DR WAVE FILES ${DR_SOURCE_FILES}")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

target_link_libraries(${PROJECT_NAME} SndFile::sndfile)

target_link_libraries(${PROJECT_NAME} portaudio)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_SND_FILE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE ${PORT_AUDIO_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE ${DR_LIBS_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

GenerateNodeLIB()