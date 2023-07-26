QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chooselevelscene.cpp \
    dataconfig.cpp \
    main.cpp \
    mainscene.cpp \
    mycoin.cpp \
    mypushbutton.cpp \
    playscene.cpp

HEADERS += \
    chooselevelscene.h \
    dataconfig.h \
    mainscene.h \
    mycoin.h \
    mypushbutton.h \
    playscene.h

FORMS += \
    mainscene.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

#可执行文件名称
TARGET = "Super Coin Flipper"

#版本信息
VERSION = 1.0.0

#图标
RC_ICONS = ./res/myappico.ico

#公司名称
QMAKE_TARGET_COMPANY = "Kaixiang Wu"

#产品名称
QMAKE_TARGET_PRODUCT = "Super Coin Flipper"

#文件说明
QMAKE_TARGET_DESCRIPTION = "Based on Qt 5.12.12 (MinGW 7.3.0 64-bit)"

#版权信息
QMAKE_TARGET_COPYRIGHT = "Copyright 2023 Kaixiang Wu. All rights reserved."


