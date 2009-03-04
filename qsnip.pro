# -------------------------------------------------
# Project created by QtCreator 2009-02-26T02:13:30
# -------------------------------------------------
QT += xml
TARGET = qsnip
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    standarditemmodel.cpp \
    workmodedialog.cpp
HEADERS += mainwindow.h \
    ui_mainwindow.h \
    Snippet.h \
    standarditemmodel.h \
    workmodedialog.h \
    ui_workmodedialog.h
FORMS += mainwindow.ui \
    workmodedialog.ui
OTHER_FILES += snippets.xml
