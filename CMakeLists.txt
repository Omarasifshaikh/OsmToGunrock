cmake_minimum_required(VERSION 3.5)
project(OsmToGunrock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Osmium REQUIRED)

set(SOURCE_FILES main.cpp utils.cpp)
add_executable(OsmToGunrock ${SOURCE_FILES})

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( OsmToGunrock ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

find_package( BZIP2 REQUIRED )
if ( BZIP2_FOUND )
    include_directories( ${BZIP2_INCLUDE_DIRS} )
    target_link_libraries( OsmToGunrock ${BZIP2_LIBRARIES} )
endif( BZIP2_FOUND )

find_package( EXPAT REQUIRED )
if ( EXPAT_FOUND )
    include_directories( ${EXPAT_INCLUDE_DIRS} )
    target_link_libraries( OsmToGunrock ${EXPAT_LIBRARIES} )
endif( EXPAT_FOUND )



