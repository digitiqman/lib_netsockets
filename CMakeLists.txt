#CMake build, Pedro Vicente 
#command line options for library dependencies
#-DOPENSSL_INCLUDE=PATH (e.g C:\ssl\include)
#-DOPENSSL_LIBRARY=FILE (e.g C:\ssl\lib\libssl.lib)
#-DCRYPTO_LIBRARY=FILE (e.g C:\ssl\lib\libcrypto.lib)

#///////////////////////////////////////////////////////
#logic for finding header files and library dependencies:
#find_path
#find_library
#are used (with optional location hints); the first argument contains the found file if found.
#and the message "-- Found file at location" is made
#if not found the first argument variable is the same as a user option that contains the file location full path.
#the same message "-- Found file at location" is made
#but there is not an attempt at this time to validate the user input file (.h or .a)
#///////////////////////////////////////////////////////

cmake_minimum_required(VERSION 2.8)
project (lib_netsockets)
include(CheckCXXCompilerFlag)
set(BUILD_SHARED_LIBRARIES OFF)

#///////////////////////////////////////////////////////
#MSVC
#///////////////////////////////////////////////////////

if (MSVC)
  add_definitions( "/D_CRT_SECURE_NO_WARNINGS /D_WINSOCK_DEPRECATED_NO_WARNINGS" )
  if (STATIC_CRT)
    message("-- Building with static runtime library")
    set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "/MT /O1 /Ob1 /D NDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "/MT /O2 /Ob2 /D NDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
  endif()  
endif(MSVC)

if (CMAKE_VERSION VERSION_LESS "3.1")
 if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
   CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
   if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
   endif()
 endif()
else ()
  set (CMAKE_CXX_STANDARD 11)
endif()

set(LIBS ${LIBS})
if (MSVC)
  set(LIBS ${LIBS} ws2_32.lib)
endif(MSVC)

include_directories(src)

set(SOURCES ${SOURCES} src/socket.hh)
set(SOURCES ${SOURCES} src/socket.cc)
set(SOURCES ${SOURCES} src/http.hh)
set(SOURCES ${SOURCES} src/http.cc)
set(SOURCES ${SOURCES} src/ftp.hh)
set(SOURCES ${SOURCES} src/ftp.cc)
add_executable(tcp_client examples/tcp_client.cc ${SOURCES})
add_executable(tcp_server examples/tcp_server.cc ${SOURCES})
add_executable(http_client examples/http_client.cc ${SOURCES})
add_executable(http_server examples/http_server.cc ${SOURCES})
add_executable(ftp_client examples/ftp_client.cc ${SOURCES})
add_executable(json_client examples/json_client.cc ${SOURCES})
add_executable(json_server examples/json_server.cc ${SOURCES})
target_link_libraries (tcp_client ${LIBS})
target_link_libraries (tcp_server ${LIBS})
target_link_libraries (http_client ${LIBS})
target_link_libraries (http_server ${LIBS})
target_link_libraries (ftp_client ${LIBS})
target_link_libraries (json_client ${LIBS})
target_link_libraries (json_server ${LIBS})

#///////////////////////////////////////////////////////
#SQLite server example (uses JSON, SQLite)
#///////////////////////////////////////////////////////

set(SOURCES ${SOURCES} external/sqlite-3.18.0/sqlite3.h)
set(SOURCES ${SOURCES} external/sqlite-3.18.0/sqlite3.c)
set(SOURCES ${SOURCES} external/gasoncpp/gason.hpp)
set(SOURCES ${SOURCES} external/gasoncpp/gason.cpp)
add_executable(sqlite_server examples/sqlite_server.cc ${SOURCES})
target_include_directories(sqlite_server PRIVATE external/sqlite-3.18.0)
target_include_directories(sqlite_server PRIVATE external/gasoncpp)
target_link_libraries (sqlite_server ${LIBS})


#///////////////////////////////////////////////////////
#OpenSSL
#///////////////////////////////////////////////////////

find_path(OPENSSL_INCLUDE_FOUND openssl/ssl.h HINTS ${OPENSSL_INCLUDE})
if(NOT OPENSSL_INCLUDE_FOUND)
  message("-- OpenSSL header files not found")
else()
  message("-- Found OpenSSL header files at: " ${OPENSSL_INCLUDE_FOUND})
endif()

find_library(OPENSSL_LIBRARY NAMES ssl)
if(NOT OPENSSL_LIBRARY)
  message("-- OpenSSL library not found")
else()
  message("-- Found OpenSSL library at: " ${OPENSSL_LIBRARY})
endif()

find_library(CRYPTO_LIBRARY NAMES crypto)
if(NOT CRYPTO_LIBRARY)
  message("-- CRYPTO library not found")
else()
  message("-- Found CRYPTO library at: " ${CRYPTO_LIBRARY})
endif()


if(OPENSSL_INCLUDE_FOUND AND OPENSSL_LIBRARY AND CRYPTO_LIBRARY)
  message("-- Building SSL TCP example")
  add_executable(ssl_client src/ssl_socket.cc examples/ssl_client.cc )
  target_include_directories(ssl_client PRIVATE ${OPENSSL_INCLUDE_FOUND})
  if (MSVC)
    set(LIBS ${LIBS} crypt32.lib)
  endif(MSVC)
  target_link_libraries (ssl_client ${LIBS} ${OPENSSL_LIBRARY} ${CRYPTO_LIBRARY}) 
endif()
