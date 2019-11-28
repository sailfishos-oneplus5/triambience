TARGET = triambience

CONFIG += link_pkgconfig
PKGCONFIG += mlite5

QT += dbus
QT -= gui

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

HEADERS += \
    src/switcher.h \
    src/uinputevpoll.h

SOURCES += \
    src/triambience.cpp \
    src/switcher.cpp \
    src/uinputevpoll.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    config/$${TARGET}.service

target.path = /usr/bin/

systemduser.files = config/$${TARGET}.service
systemduser.path = /lib/systemd/user/

INSTALLS += target systemduser
