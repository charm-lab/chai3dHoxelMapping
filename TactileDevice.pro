                                                                                                                                                              #-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T14:42:00
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport

TARGET = TactileDevice
TEMPLATE = app
CONFIG += qwt

SOURCES += ./src/main.cpp
SOURCES += ./src/endNotification.cpp
SOURCES += ./src/mainwindow.cpp
SOURCES += ./src/cMotorController.cpp
SOURCES += ./src/haptics_thread.cpp
SOURCES += ./src/c3DOFdevice.cpp
SOURCES += ./src/Widget_OpenGLDisplay.cpp
SOURCES += ./src/c3dofChaiDevice.cpp
SOURCES += ./src/trakSTAR.cpp
SOURCES += ./src/magtracker.cpp
SOURCES += ./src/experiment_thread.cpp
SOURCES += ./src/magTracker_thread.cpp
SOURCES += #Heathers Vibration
SOURCES += ./src/TappingParameters.cpp
SOURCES += ./src/AccSynthHashMatrix.cpp
SOURCES += ./src/pugixml.cpp
SOURCES += #./src/digitalFilter.cpp
SOURCES += ./External/chai3d-3.1.1/modules/OCULUS/src/OVRDevice.cpp
SOURCES += ./External/chai3d-3.1.1/modules/OCULUS/src/OVRRenderContext.cpp
SOURCES += ./src/Vibrotactile_thread.cpp
SOURCES += ./src/mychai3dwindow.cpp
SOURCES += ./src/breaktimedialog.cpp
SOURCES += ./src/cceExpTypeDialog.cpp
SOURCES += ./src/trialNotification.cpp

HEADERS  += ./include/mainwindow.h
HEADERS  += ./include/endNotification.h
HEADERS  += ./include/haptics_thread.h
HEADERS  += ./include/cMotorController.h
HEADERS  += ./include/shared_data.h
HEADERS  += ./include/c3DOFdevice.h
HEADERS  += ./include/Widget_OpenGLDisplay.h
HEADERS  += ./include/c3dofChaiDevice.h
HEADERS  += ./include/TrakSTAR.h
HEADERS  += ./include/ATC3DG.h
HEADERS  += ./include/magtracker.h
HEADERS  += ./include/SimpleIni.h
HEADERS  += ./include/experiment_thread.h
HEADERS  += ./include/magTracker_thread.h
HEADERS  += ./include/mychai3dwindow.h
HEADERS  += ./include/breaktimedialog.h
HEADERS  += ./include/cceExpTypeDialog.h
HEADERS  += ./include/trialNotification.h

# Heathers Vibration
HEADERS  += ./include/TappingParameters.h
#HEADERS  += ./include/digitalFilter.h
HEADERS  += ./include/AccSynthHashMatrix.h
HEADERS  += ./include/pugixml.hpp
HEADERS  += ./include/shared.h
HEADERS  += ./include/autoGenHashMatrix.h
HEADERS  += ./include/pugiconfig.hpp
HEADERS  += ./include/Vibrotactile_thread.h


FORMS    += ./mainwindow.ui
FORMS    += ./mychai3dwindow.ui

#include path for Eigen
INCLUDEPATH += "./External"

# Include path for qwt
INCLUDEPATH += "./include"
INCLUDEPATH += "./External/Qwt-6.1.3/include"

# Include path for Sensoray
INCLUDEPATH += "./External/s826_3.3.9/api/"

# Include path for Chai3d, Openhaptics
INCLUDEPATH += "./External/chai3d-3.1.1/src"
INCLUDEPATH += "./External/chai3d-3.1.1/external/glew/include"
INCLUDEPATH += "./External/chai3d-3.1.1/extras/freeglut/include"
INCLUDEPATH += "./External/chai3d-3.1.1/external/Eigen"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/ODE"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/ODE/src"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/ODE/external/ODE/include"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/ODE/obj/Debug/Win32"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/OCULUS/src/"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/OCULUS/external/SDL/include"
INCLUDEPATH += "./External/chai3d-3.1.1/modules/OCULUS/external/oculusSDK/LibOVR/Include/"

# Include path for Boost
INCLUDEPATH += "./External/boost_1_64_0"
INCLUDEPATH += "./External/boost_1_64_0/boost"
INCLUDEPATH += "./External/boost_1_64_0/boost/random"

# Libraries for Chai3d
LIBS += "./External/chai3d-3.1.1/external/glfiles/lib/OPENGL32.lib"
LIBS += "./External/chai3d-3.1.1/external/glfiles/lib/GLU32.lib"
LIBS += "./External/chai3d-3.1.1/extras/freeglut/lib/Debug/Win32/freeglut.lib"
LIBS += "./External/chai3d-3.1.1/lib/Debug/Win32/chai3d.lib"
LIBS += "./External/chai3d-3.1.1/modules/ODE/lib/Debug/Win32/chai3d-ODE.lib"

# Libraries for Oculus
LIBS += "./External/chai3d-3.1.1/modules/OCULUS/external/SDL/lib/VS2013/Win32/SDL2.lib"
LIBS += "./External/chai3d-3.1.1/modules/OCULUS/external/SDL/lib/VS2013/Win32/SDL2main.lib"
LIBS += "./External/chai3d-3.1.1/modules/OCULUS/external/oculusSDK/LibOVR/Lib/Windows/Win32/Debug/VS2013/LibOVR.lib"
LIBS += "./External/chai3d-3.1.1/modules/OCULUS/external/oculusSDK/LibOVRKernel/Lib/Windows/Win32/Debug/VS2013/LibOVRKernel.lib"

# Libraries for 826
LIBS += "./External/s826_3.3.9/api/x32/s826.lib"

# Library for Ascension tracker
LIBS += "./lib/ATC3DG.lib"

LIBS += "atls.lib"
LIBS += "winmm.lib"
LIBS += "odbc32.lib"
LIBS += "odbccp32.lib"
LIBS += "user32.lib"
LIBS += "kernel32.lib"
LIBS += "gdi32.lib"
LIBS += "winspool.lib"
LIBS += "shell32.lib"
LIBS += "ole32.lib"
LIBS += "oleaut32.lib"
LIBS += "uuid.lib"
LIBS += "comdlg32.lib"
LIBS += "advapi32.lib"
LIBS += "./External/Qwt-6.1.3/lib/qwtd.lib"

