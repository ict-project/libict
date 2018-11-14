################################################################
cmake_minimum_required(VERSION 2.6)
################################################################
project(libict)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

add_custom_command(OUTPUT all.hpp PRE_BUILD COMMAND ./all.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SOURCE_FILES 
  utf8.cpp options.cpp base64.cpp 
  time.cpp os.cpp logger.cpp random.cpp 
  register.cpp daemon.cpp pool.cpp json-object.cpp
  websocket.cpp mutex.cpp buffer.cpp 
  data-types.cpp data-interface.cpp
  data-simple.cpp data-vector.cpp
  data-object.cpp data-array.cpp
  data-deque.cpp data-forward_list.cpp 
  data-tuple.cpp data-map.cpp
  data-info.cpp 
  utf8-robot.cpp
)

add_library(ict-static STATIC ${CMAKE_SOURCE_FILES})
target_link_libraries(ict-static ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(ict-static  PROPERTIES OUTPUT_NAME ict)

add_library(ict-shared SHARED ${CMAKE_SOURCE_FILES})
target_link_libraries(ict-shared ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(ict-shared  PROPERTIES OUTPUT_NAME ict)

add_executable(libict-test all.hpp test.cpp ${CMAKE_SOURCE_FILES})
target_link_libraries(libict-test ${CMAKE_THREAD_LIBS_INIT})
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

file(GLOB HEADER_LIST . *.hpp)

install(TARGETS ict-static ict-shared DESTINATION lib COMPONENT libraries)
install(
  FILES ${HEADER_LIST}
  DESTINATION include/libict COMPONENT headers
)
################################################################
enable_testing()

add_test(NAME ict-all COMMAND libict-test ict)

################################################################
#set(CPACK_GENERATOR "TZ;TGZ;STGZ;DEB;RPM") # Linux
#set(CPACK_GENERATOR "TZ;TGZ;STGZ")         # MacOS

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