TARGET = triambience

CONFIG += link_pkgconfig
PKGCONFIG += mlite5

QT += dbus
QT -= gui

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

target.path = /usr/bin/

systemduser.files = config/triambience.service
systemduser.path = /etc/systemd/user/

INSTALLS += target systemduser

message($${DEFINES})

SOURCES += \
    src/triambience.cpp \
    src/switcher.cpp \
    src/uinputevpoll.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    config/triambience.service

HEADERS += \
    src/switcher.h \
    src/uinputevpoll.h
