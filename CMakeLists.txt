cmake_minimum_required(VERSION 3.2)

set(NAME outbox)
PROJECT (${NAME})
set(PROJECT ${NAME} )

set (CMAKE_CXX_STANDARD 11)

set( HEADERS ${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR} "*.cpp")

include_directories( ${PROJECT_SOURCE_DIR} )

set( REQUIRED_LIBRARIES )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLite3_INCLUDE_DIRS})
  set( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${SQLite3_LIBRARIES})
endif (SQLITE3_FOUND)

if(UNIX)
    set( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} pthread)
endif()

add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} ${REQUIRED_LIBRARIES} )

