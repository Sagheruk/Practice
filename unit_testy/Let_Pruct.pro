QT -= gui

CONFIG += c++20
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Numerator_1.cpp \
        main.cpp

INCLUDEPATH += C:/boost_1_81_0/boost_1_81_0/boost_mingw/include/boost-1_81
    #LIBS += -LC:/boost_1_81_0/boost_1_81_0/boost_mingw/lib -llibboost_locale-mgw53-mt-1_61

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
