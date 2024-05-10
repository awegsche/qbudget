cmake_minimum_required(VERSION 3.12)

project(qbudget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test PrintSupport)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        # core
        transaction.h transaction.cpp
        bill.h bill.cpp
        newbilldialog.h newbilldialog.cpp newbilldialog.ui
        savefile.h savefile.cpp
        qcustomplot-source/qcustomplot.h qcustomplot-source/qcustomplot.cpp
        settings.h settings.cpp
        category.h category.cpp
        billviewmodel.h billviewmodel.cpp
        contentmanager.h contentmanager.cpp
        # utils
        utils.h utils.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qbudget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qbudget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qbudget SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qbudget
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qbudget PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qbudget)
endif()
set_target_properties(qbudget PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qbudget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --------------------------------------------------------------------------------------------------
# ---- Testing -------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------

qt_add_executable( tests
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    tests/tests.cpp tests/tests.h
    )

target_link_libraries(tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::PrintSupport
    )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qbudget)
endif()
