cmake_minimum_required(VERSION 3.8)
project(sqlite_raii
	LANGUAGES C CXX
	DESCRIPTION "sqlite api raii"
	VERSION 0.0.0.0
	)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "${PROJECT_NAME}/*.cpp")
file(GLOB_RECURSE headers RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "${PROJECT_NAME}/*.hpp")
source_group(
	TREE "${CMAKE_CURRENT_LIST_DIR}"
	FILES ${sources} ${headers}
	)

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers} "${PROJECT_NAME}/sqlite3.c" "${PROJECT_NAME}/sqlite3.h")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
set_source_files_properties("${PROJECT_NAME}/sqlite3.c" PROPERTIES
    COMPILE_DEFINITIONS "SQLITE_ENABLE_MATH_FUNCTIONS"
    )


enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
macro(add_unit_test tname path)
	add_executable("${tname}_test" "tests/${path}")
	target_link_libraries("${tname}_test" PRIVATE ${PROJECT_NAME} ${Boost_LIBRARIES})
	add_test(NAME ${tname} COMMAND "${tname}_test")
endmacro()

add_unit_test(common common_test.cpp)
