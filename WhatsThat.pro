QT += quick multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DeepSpeechC.cpp \
        Game.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux|win32|!android {
    INCLUDEPATH += $$PWD/lib/amd64
    LIBS += -L$$PWD/lib/amd64/ -ldeepspeech
}
android{
    INCLUDEPATH += $$PWD/lib/arm7
    LIBS += -L$$PWD/lib/arm7/ -ldeepspeech -lc++_shared

    #Install model file into APK
    dsmodel.files = $$PWD/models/ds93.tflite
    dsmodel.path = /assets/models
    INSTALLS += dsmodel

    audiof.files = $$PWD/audio/*
    audiof.path = /assets/audio
    INSTALLS += audiof

}

#DEPENDPATH += $$PWD/lib/amd64

ANDROID_EXTRA_LIBS = $$PWD/lib/arm7/libc++_shared.so $$PWD/lib/arm7/libdeepspeech.so

HEADERS += \
    DeepSpeechC.h \
    Game.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
