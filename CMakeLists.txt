cmake_minimum_required(VERSION 3.2.1)
CMAKE_POLICY(SET CMP0003 NEW)

project(MTEXT)

message(STATUS "CPU: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "CXX flags: " ${CMAKE_CXX_FLAGS})

####################################################################
#
#
# set MTEXT_QT_VERSION
#
#
####################################################################

set(MTEXT_QT_VERSION "QT_VERSION_NOT_SELECTED" CACHE STRING "Qt version")
set_property(CACHE MTEXT_QT_VERSION PROPERTY STRINGS "QT_VERSION_NOT_SELECTED" "4" "5" "6")

if(APPLE)
  set(MTEXT_OSX_BUNDLE_VERSION "1.0.0")
endif()

if(MTEXT_QT_VERSION STREQUAL "4")
  set(USE_QT_V_4 TRUE)
  message(STATUS "Using Qt4")
elseif(MTEXT_QT_VERSION STREQUAL "5")
  set(USE_QT_V_5 TRUE)
  message(STATUS "Using Qt5")
elseif(MTEXT_QT_VERSION STREQUAL "6")
  set(USE_QT_V_6 TRUE)
  message(STATUS "Using Qt6")
else()
  message(FATAL_ERROR "Can not determine Qt version to use")
endif()

if(USE_QT_V_5 OR USE_QT_V_6)
  if(WIN32)
# https://cmake.org/cmake/help/v3.0/policy/CMP0020.html
# Automatically link Qt executables to qtmain target on Windows
    CMAKE_POLICY(SET CMP0020 NEW)
  endif()
endif()

if(USE_QT_V_6)
  find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    Svg
    Core5Compat REQUIRED)
  include_directories(${Qt6Core5Compat_INCLUDE_DIRS})
elseif(USE_QT_V_5)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Svg REQUIRED)
elseif(USE_QT_V_4)
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTMAIN TRUE)
  set(QT_USE_QTGUI TRUE)
  set(QT_USE_QTSVG TRUE)
  include(${QT_USE_FILE})
  #add_definitions(${QT_DEFINITIONS})
  #message(STATUS "QT_DEFINITIONS: ${QT_DEFINITIONS}")
else()
  message(FATAL_ERROR "Can not determine Qt version to use")
endif()

set(MTEXT_SRCS
  matrixbutton.cpp
  editorgraphicsview.cpp
  textedit.cpp
  tabledialog.cpp
  charsmodel.cpp
  charswidget.cpp
  main.cpp)

if(WIN32)
  set(MTEXT_SRCS ${MTEXT_SRCS} mtext.rc)
endif()

set(MTEXT_QRCS mtext.qrc)

set(MTEXT_MOC_HRDS
  matrixbutton.h
  editorgraphicsview.h
  tabledialog.h
  charsmodel.h
  charswidget.h)

if(USE_QT_V_6)
  set(MTEXT_MOC_HRDS ${MTEXT_MOC_HRDS} textedit-qt6.h)
elseif(USE_QT_V_5)
  set(MTEXT_MOC_HRDS ${MTEXT_MOC_HRDS} textedit-qt5.h)
elseif(USE_QT_V_4)
  set(MTEXT_MOC_HRDS ${MTEXT_MOC_HRDS} textedit-qt4.h)
else()
  message(FATAL_ERROR "Can not determine Qt version to use")
endif()

set(MTEXT_UIS tabledialog.ui charswidget.ui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if(USE_QT_V_6)
  QT6_WRAP_CPP(MTEXT_MOC_SRCS ${MTEXT_MOC_HRDS})
  QT6_WRAP_UI(MTEXT_UI ${MTEXT_UIS})
  QT6_ADD_RESOURCES(MTEXT_QRCS_RCC ${MTEXT_QRCS})
  set(MTEXT_LINK_LIBRARIES
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Svg
    Qt6::Core5Compat)
elseif(USE_QT_V_5)
  QT5_WRAP_CPP(MTEXT_MOC_SRCS ${MTEXT_MOC_HRDS})
  QT5_WRAP_UI(MTEXT_UI ${MTEXT_UIS})
  QT5_ADD_RESOURCES(MTEXT_QRCS_RCC ${MTEXT_QRCS})
  set(MTEXT_LINK_LIBRARIES
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Svg)
elseif(USE_QT_V_4)
  QT4_WRAP_CPP(MTEXT_MOC_SRCS ${MTEXT_MOC_HRDS})
  QT4_WRAP_UI(MTEXT_UI ${MTEXT_UIS})
  QT4_ADD_RESOURCES(MTEXT_QRCS_RCC ${MTEXT_QRCS})
  set(MTEXT_LINK_LIBRARIES ${QT_LIBRARIES})
else()
  message(FATAL_ERROR "Can not determine Qt version to use")
endif()

if(WIN32)
  add_executable(mtext WIN32
    ${MTEXT_SRCS}
    ${MTEXT_MOC_SRCS}
    ${MTEXT_UIS}
    ${MTEXT_QRCS_RCC}
    Win32AppUtf8.manifest)
  target_link_libraries(mtext ${MTEXT_LINK_LIBRARIES})
elseif(APPLE)
  set(MTEXT_OSX_ICON_NAME "Mtext.icns")
  set(MTEXT_OSX_ICON ${CMAKE_CURRENT_SOURCE_DIR}/${MTEXT_OSX_ICON_NAME})
  set_source_files_properties(${MTEXT_OSX_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  add_executable(Mtext MACOSX_BUNDLE
    ${MTEXT_SRCS}
    ${MTEXT_MOC_SRCS}
    ${MTEXT_UIS}
    ${MTEXT_QRCS_RCC}
    ${MTEXT_OSX_ICON})
  set_target_properties(Mtext PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Mtext"
    MACOSX_BUNDLE_BUNDLE_VERSION ${MTEXT_OSX_BUNDLE_VERSION}
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.aliza.mtext"
    MACOSX_BUNDLE_ICON_FILE ${MTEXT_OSX_ICON_NAME}
    MACOSX_BUNDLE_COPYRIGHT "© 2015-2021 Aliza Medical Imaging")
  target_link_libraries(Mtext ${MTEXT_LINK_LIBRARIES})
else()
  add_executable(mtext
    ${MTEXT_SRCS}
    ${MTEXT_MOC_SRCS}
    ${MTEXT_UIS}
    ${MTEXT_QRCS_RCC})
  target_link_libraries(mtext ${MTEXT_LINK_LIBRARIES})
endif()

