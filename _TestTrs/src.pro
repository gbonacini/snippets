#-------------------------------------------------
#
# Project created by QtCreator 2019-06-05T17:11:30
#
#-------------------------------------------------

QT       -= gui

TARGET = unittests
TEMPLATE = lib

DEFINES += LIBUNITTESTS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -I$$PWD/../../../mod_liblmtransaction/trunk/src -I$$PWD/../../../mod_unit_tests/trunk/src
QMAKE_CXXFLAGS += -DTEST_ENVIRONMENT -DNO_LM_LOGGER

SOURCES += \
        libunittests_transaction.cpp

HEADERS += \
        common_wrapper.h \
        libunittests_transaction.h \
        libunittests_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INSTALL_PREFIX="$$INSTALL_PREFIX"
isEmpty(INSTALL_PREFIX) {
   INSTALL_PREFIX = /usr
}

message("INSTALL_PREFIX = $$INSTALL_PREFIX")

target.path = $$INSTALL_PREFIX/lib
INSTALLS += target

headers.path = $$INSTALL_PREFIX/include/lmtransaction
headers.files= *.h
INSTALLS+=headers
