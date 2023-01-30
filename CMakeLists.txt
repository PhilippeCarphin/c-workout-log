cmake_minimum_required(VERSION 3.12)

project(Workout C)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


find_package(json-c REQUIRED)

add_executable(main main.c from_json.c modifiers.c to_json.c)
target_link_libraries(main json-c::json-c-static)

include(CTest)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_test(NAME run COMMAND $<TARGET_FILE:main>)
add_dependencies(check main)

