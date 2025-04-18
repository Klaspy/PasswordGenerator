QT = core sql quick qml core5compat quickcontrols2

CONFIG += c++17
TEMPLATE = app
VERSION = 0.0.0.3

RC_ICONS = img/icon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        database/dbworker.cpp \
        database/workersmodel.cpp \
        main.cpp \
        xlsxworker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    qml.qrc \
    style.qrc

HEADERS += \
    database/dbworker.h \
    database/workersmodel.h \
    xlsxworker.h

QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(QXlsx/QXlsx.pri)
