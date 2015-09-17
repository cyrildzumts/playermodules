TEMPLATE = lib
TARGET = PlayerModules
QT += qml quick sql multimedia core dbus
CONFIG += qt plugin link_pkgconfig no_keywords c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = PlayerModules


# extern Libs
#QMAKE_CXXFLAGS += -I /usr/include/id3
#QMAKE_CXXFLAGS += -std=c++0x -pthread
#QMAKE_LIBS += -lid3
DESTDIR = /home/cyrildz/Project/LP/lib/PlayerModules
PKGCONFIG += taglib
MOC_DIR = .moc
OBJECTS_DIR = .obj

# Input
SOURCES += \
    playermodules_plugin.cpp \
    albummodel.cpp \
    notify.cpp \
    player.cpp \
    playlist.cpp \
    playlistcontentmodel.cpp \
    tracklistmodel.cpp \
    mpris2.cpp \
    mpris2_player.cpp \
    dbusmanager.cpp \
    mpris2_Adaptor.cpp \
    mpris2_playerAdaptor.cpp \
    Notify_Adaptor.cpp \
    artistmodel.cpp \
    genremodel.cpp \
    playlistmanager.cpp \
    logger.cpp \
    queue.cpp \
    tagreaderrevisited.cpp \
    utils.cpp \
    playlistmodel.cpp \
    randomgenerator.cpp

HEADERS += \
    playermodules_plugin.h \
    albummodel.h \
    notify.h \
    player.h \
    playlist.h \
    playlistcontentmodel.h \
    tracklistmodel.h \
    mpris2.h \
    mpris2_player.h \
    dbusmanager.h \
    mpris2_Adaptor.h \
    mpris2_playerAdaptor.h \
    Notify_Adaptor.h \
    artistmodel.h \
    genremodel.h \
    playlistmanager.h \
    logger.h \
    queue.h \
    tagreader.h \
    utils.h \
    playlistmodel.h \
    randomgenerator.h \
    abstractplaylist.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    #installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    installPath = $$PWD/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}


