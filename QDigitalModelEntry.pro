QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#多字节编译
DEFINES  -= UNICODE
DEFINES  += UMBCS

#指定生成类型
TEMPLATE = app

#指定生成文件名
TARGET = QDigitalModelEntry

#输出文件夹定义
DESTDIR = $$PWD/build/bin

# 公共配置
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qadmintypedlg.cpp \
    qmytableview.cpp \
    qusertypedlg.cpp

HEADERS += \
    mainwindow.h \
    qadmintypedlg.h \
    qglobaldefine.h \
    qmytableview.h \
    qusertypedlg.h

FORMS += \
    mainwindow.ui \
    qadmintypedlg.ui \
    qusertypedlg.ui

# Windows 平台配置
win32 {
	#定义宏传递给编译器,代码中可使用
        DEFINES += WIN32
	DEFINES += MY_WINDOWS_FLAG

	#添加头文件目录
        INCLUDEPATH +=$$PWD/cplus_class
        INCLUDEPATH +=$$PWD/cplus_class/DB
        INCLUDEPATH +=$$PWD/cplus_class/AWS
	INCLUDEPATH +=$$PWD/inc
        INCLUDEPATH +=$$PWD/inc/pthread
        INCLUDEPATH +=$$PWD/inc/mysql
        INCLUDEPATH +=$$PWD/inc/aws

	#添加库目录、库文件
	LIBS += -L$$PWD/lib
        LIBS += -L$$PWD/lib/pthread
	LIBS += -L$$PWD/lib/mysql/windows/Release 
        LIBS += -L$$PWD/lib/aws/windows/Release

        #添加额外文件
        SOURCES += $$PWD/cplus_class/public.h
        SOURCES += $$PWD/cplus_class/public.cpp

        SOURCES += $$PWD/cplus_class/loghelp.h
        SOURCES += $$PWD/cplus_class/logger.h
        SOURCES += $$PWD/cplus_class/logger.cpp

        SOURCES += $$PWD/cplus_class/DB/digitalMysql.h
        SOURCES += $$PWD/cplus_class/DB/digitalMysql.cpp

        SOURCES += $$PWD/cplus_class/AWS/dirent.h
        SOURCES += $$PWD/cplus_class/AWS/awsUpload.h
        SOURCES += $$PWD/cplus_class/AWS/awsUpload.cpp
}

# macOS 平台配置
macx {
    #定义宏传递给编译器,代码中可使用
    DEFINES += MY_MACOS_FLAG
}

# Linux 平台配置
unix {
    #定义宏传递给编译器,代码中可使用
    DEFINES += MY_LINUX_FLAG
}

#主程序图标
win32:RC_ICONS += $$PWD/res/Main.ico

#皮肤资源
RESOURCES += \
    res/MyRec.qrc

TRANSLATIONS = DigitalModelEntry.ts
