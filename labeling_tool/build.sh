#!/bin/sh

PROJECT_FILE="Labeling_tool.pro"

if [ $(which qmake-qt4) ]; then
	QMAKE="qmake-qt4"
else
	QMAKE="qmake"
fi

build_dir=build
if [ ! -e $build_dir ]; then
	mkdir $build_dir
fi

${QMAKE} -project -o ${PROJECT_FILE}

cat << FIN >> ${PROJECT_FILE}
QMAKE_CFLAGS += -std=c++11 \`pkg-config --cflags opencv\`
QMAKE_CXXFLAGS += -std=c++11 \`pkg-config --cflags opencv\`
LIBS += \`pkg-config --libs opencv\`
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DESTDIR=../
OBJECTS_DIR=build
MOC_DIR=build
FIN

${QMAKE}
