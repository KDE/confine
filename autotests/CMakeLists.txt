enable_testing()
include(ECMAddTests)
include(ECMMarkAsTest)


find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)

if(NOT Qt5Test_FOUND)
    message(STATUS "Qt5Test not found, autotests will not be built.")
    return()
endif()

include_directories("${CMAKE_SOURCE_DIR}/src")

add_library(autotests STATIC ${confineBase_SRCS} )

target_link_libraries(autotests    
    Qt5::Core
    KF5::CoreAddons
    KF5::I18n
    KF5::XmlGui
    KF5::ConfigCore
)

ecm_mark_as_test(autotests)

ecm_add_tests(
   userprofilemanagertest.cpp
   usertest.cpp
   immutabletest.cpp
   configxttest.cpp
   LINK_LIBRARIES Qt5::Test autotests
)
