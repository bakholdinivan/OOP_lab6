cmake_minimum_required(VERSION 3.20)
project(Lab04Templates CXX)

# Требуем стандарт C++20 для concepts, умных указателей и range-based for
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папка с заголовками
include_directories(include)

# Основная программа
add_executable(lab04 main.cpp)

enable_testing()
include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG release-1.12.1
 )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(tests tests/test_templates.cpp)
target_link_libraries(tests gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)