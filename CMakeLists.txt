cmake_minimum_required(VERSION 3.21)
project(aeron_php C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_INSTALL_PREFIX /usr)

add_compile_definitions(ZEND_DEBUG=1)

# Aeron Client
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/aeron EXCLUDE_FROM_ALL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/aeron/aeron-client/src/main/c)

# PHP
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src/TSRM)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src/Zend)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src/ext)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/php-src/ext/date/lib)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/aeron.c)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/php_aeron.h)

set(CONFIGS
    ${CMAKE_CURRENT_SOURCE_DIR}/aeron.stub.php
    ${CMAKE_CURRENT_SOURCE_DIR}/config.m4
    ${CMAKE_CURRENT_SOURCE_DIR}/config.w32)

install(TARGETS aeron LIBRARY DESTINATION lib)

add_custom_target(build
    COMMAND phpize
    COMMAND ./configure
    COMMAND make
    COMMAND sudo make install
    DEPENDS ${SOURCES} ${HEADERS} ${CONFIGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(_ EXCLUDE_FROM_ALL ${SOURCES} ${HEADERS})
