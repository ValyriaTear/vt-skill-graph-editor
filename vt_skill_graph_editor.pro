#///////////////////////////////////////////////////////////////////////////////
#//            Copyright (C) 2017 by Bertram (Valyria Tear)
#//                         All Rights Reserved
#//
#// This code is licensed under the GNU GPL version 3. It is free software
#// and you may modify it and/or redistribute it under the terms of this license.
#// See http://www.gnu.org/copyleft/gpl.html for details.
#///////////////////////////////////////////////////////////////////////////////

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vt_skill_graph_editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
    src/main_window.cpp \
    src/graph_scene.cpp \
    src/node_model.cpp \
    src/skill_nodes_table.cpp \
    src/skill_data_dialog.cpp \
    src/lua_writer.cpp

HEADERS += \
    src/main_window.h \
    src/graph_scene.h \
    src/node_model.h \
    src/skill_nodes_table.h \
    src/skill_data_dialog.h \
    src/node_data.h \
    src/lua_writer.h

FORMS +=

DISTFILES += \
    COPYRIGHT \
    README.md
