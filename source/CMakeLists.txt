################################################################
cmake_minimum_required(VERSION 2.6)
################################################################
project(libict)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_SOURCE_FILES utf8.cpp options.cpp base64.cpp time.cpp logger.cpp)

add_library(ict-static STATIC ${CMAKE_SOURCE_FILES})
set_target_properties(ict-static  PROPERTIES OUTPUT_NAME ict)

add_library(ict-shared SHARED ${CMAKE_SOURCE_FILES})
set_target_properties(ict-shared  PROPERTIES OUTPUT_NAME ict)

add_executable(libict-test test.cpp ${CMAKE_SOURCE_FILES})
target_compile_definitions(libict-test PUBLIC -DENABLE_TESTING)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "GIT_VERSION"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message( STATUS "Git version: ${GIT_VERSION}" )
  else(GIT_FOUND)
    set(GIT_VERSION unknown)
  endif(GIT_FOUND)
else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)
 set(GIT_VERSION unknown)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/git_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/git_version.h @ONLY)

install(TARGETS ict-static ict-shared DESTINATION lib COMPONENT libraries)
install(FILES utf8.hpp options.hpp base64.hpp time.hpp logger.hpp all.hpp DESTINATION include/libict COMPONENT headers)
################################################################
enable_testing()
set(CMAKE_CURRENT_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tests)
set(CMAKE_CURRENT_TEST_TOOL ${CMAKE_CURRENT_BINARY_DIR}/libict-test)

add_test(NAME ict-utf8-tc1 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict utf8 tc1)
add_test(NAME ict-utf8-tc2 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict utf8 tc2)

add_test(NAME ict-base64-tc1 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc1)
add_test(NAME ict-base64-tc2 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc2)
add_test(NAME ict-base64-tc3 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc3)
add_test(NAME ict-base64-tc4 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc4)
add_test(NAME ict-base64-tc5 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc5)
add_test(NAME ict-base64-tc6 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc6)
add_test(NAME ict-base64-tc7 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc7)
add_test(NAME ict-base64-tc8 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc8)
add_test(NAME ict-base64-tc9 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc9)
add_test(NAME ict-base64-tc10 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc10)
add_test(NAME ict-base64-tc11 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc11)
add_test(NAME ict-base64-tc12 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict base64 tc12)

add_test(NAME ict-options-tc1 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc1)
add_test(NAME ict-options-tc2 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc2)
add_test(NAME ict-options-tc3 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc3)
add_test(NAME ict-options-tc4 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc4)
add_test(NAME ict-options-tc5 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc5)
add_test(NAME ict-options-tc6 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict options tc6)
################################################################
set(CPACK_GENERATOR "TZ;TGZ;STGZ;DEB;RPM")

set(CPACK_PACKAGE_NAME "libict")
set(CPACK_PACKAGE_VENDOR "ict-project.pl")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mariusz Ornowski <mariusz.ornowski@ict-project.pl>")
set(CPACK_RPM_PACKAGE_LICENSE "BSD 3-clause")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libict - C++11 Library with common application modules.")
set(CPACK_PACKAGE_VERSION ${GIT_VERSION})

set(CPACK_COMPONENTS_ALL libraries headers)

set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Libraries for libict - C++11 Library with common application modules.")

set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION  "C++ Headers for libict - C++11 Library with common application modules.")
set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)

include(CPack)
################################################################