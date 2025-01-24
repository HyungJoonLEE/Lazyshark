cmake_minimum_required(VERSION 3.5)

project(LAZYSHARK
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

#======================== INCLUSION OF Qt ========================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)


list(APPEND CMAKE_PREFIX_PATH "/home/hj/Qt/6.5.3/gcc_64")

#if(APPLE)
#else()
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
#    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -arch x86_64 -O3")
#    string(APPEND CMAKE_CC_FLAGS_RELEASE " -arch x86_64 -O3")
#endif()


#======================== INCLUSION OF PROJECT FILES ========================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})
include_directories(${PCAP_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES
        "${FORMS_DIR}/*.ui"
        "${FORMS_DIR}/*.qrc"
        "${INCLUDE_DIR}/*.h"
        "${SOURCE_DIR}/*.cpp"
)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
#find_package(QT${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Pcap REQUIRED)

link_directories(${PCAP_LIBRARY_DIRS})



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${SOURCES}
        Resource/img.qrc
        src/common.cpp
    )
else()
    include (GNUInstallDirs)
    include(FindPCAP.cmake)
    add_executable(${PROJECT_NAME}
            ${SOURCES}
    )
endif()

# Add the target includes for ${PROJECT_NAME}target_include_directories(${PROJECT_NAME} PRIVATE ${FORMS_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})


include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(${PROJECT_NAME})
#endif()

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PCAP_LIBRARIES})

