##################
# Target
get_filename_component(DIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" TARGET_MAIN ${DIRECTORY_NAME})
set(TARGET_MAIN_TESTS ${TARGET_MAIN}_tests)

####################
# Sources & headers
aux_source_directory(. SRC_LIST)
file(GLOB HEADERS_LIST "*.h" "*.hpp")

add_executable(${TARGET_MAIN_TESTS} ${SRC_LIST} ${HEADERS_LIST})

if (MSVC)
    target_compile_options(${TARGET_MAIN_TESTS} PRIVATE  /std:c++latest)
endif()

find_package(Catch2 REQUIRED)

Include(FetchContent)

FetchContent_Declare(
  stdexec
  GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
  GIT_TAG        a3374f5d1d84dbecfa339b32de2395dbf9460cb1
  # GIT_TAG        daf12bcc46438f88b3c78212363f7b6531338780
  
)

FetchContent_MakeAvailable(stdexec)

target_link_libraries(${TARGET_MAIN_TESTS} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain STDEXEC::stdexec)

include(Catch)
catch_discover_tests(${TARGET_MAIN_TESTS})