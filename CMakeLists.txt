project (confine)
 
#1
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")
 
#2
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
 
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
 
# Find Qt modules
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication 
)
 
# Find KDE modules
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    XmlGui          # KXmlGuiWindow
    Config
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

if (POLICY CMP0063)
  cmake_policy (SET CMP0063 NEW)
endif (POLICY CMP0063)
 
set(confineBase_SRCS 
  ${CMAKE_SOURCE_DIR}/src/profile.cpp 
  ${CMAKE_SOURCE_DIR}/src/userprofilemanager.cpp 
  ${CMAKE_SOURCE_DIR}/src/user.cpp 
  ${CMAKE_SOURCE_DIR}/src/configxtreader.cpp 
  ${CMAKE_SOURCE_DIR}/src/kgroupinfo.cpp 
  ${CMAKE_SOURCE_DIR}/src/kentryinfo.cpp 
  ${CMAKE_SOURCE_DIR}/src/kconfigfileinfo.cpp 
  ${CMAKE_SOURCE_DIR}/src/confineconfiguration.cpp
  ${CMAKE_SOURCE_DIR}/src/configurationinfomanager.cpp
  ${CMAKE_SOURCE_DIR}/src/kconfigimmutable.cpp
)

set(confineUI_SRCS 
  ${CMAKE_SOURCE_DIR}/src/main.cpp 
  ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp 
  ${CMAKE_SOURCE_DIR}/src/configdialog.cpp
  ${CMAKE_SOURCE_DIR}/src/copyconfigfile.cpp
  ${CMAKE_SOURCE_DIR}/src/restrictionsdialog.cpp
  ${CMAKE_SOURCE_DIR}/src/settingsdialog.cpp
  ${CMAKE_SOURCE_DIR}/src/restrictionelement.cpp
)

ki18n_wrap_ui(confine_UI ${CMAKE_SOURCE_DIR}/src/mainwindow.ui ${CMAKE_SOURCE_DIR}/src/configdialog.ui ${CMAKE_SOURCE_DIR}/src/settingsdialog.ui ${CMAKE_SOURCE_DIR}/src/copyconfigfile.ui ${CMAKE_SOURCE_DIR}/src/kderestrictions.ui)

add_subdirectory(autotests)

# just plain add_executable
add_executable(confine ${confineBase_SRCS} ${confineUI_SRCS} ${confine_UI})
 
# module-based linking
target_link_libraries(confine
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    KF5::XmlGui
    KF5::ConfigCore 
    KF5::ConfigGui
)
 
install(TARGETS confine  ${INSTALL_TARGETS_DEFAULT_ARGS})
