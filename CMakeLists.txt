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
 
set(confine_SRCS src/main.cpp src/mainwindow.cpp src/profilemanager.cpp src/profile.cpp src/usermanager.cpp src/user.cpp)

qt5_wrap_ui(confine_UI src/mainwindow.ui)

# just plain add_executable
add_executable(confine ${confine_SRCS} ${confine_UI})
 
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
