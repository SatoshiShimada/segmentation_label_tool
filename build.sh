#!/bin/sh

PROJECT_FILE="segmentation_label_tool.pro"

if [ $(which qmake-qt4) ]; then
	QMAKE="qmake-qt4"
else
	QMAKE="qmake"
fi

${QMAKE} -project -o ${PROJECT_FILE}

cat << FIN >> ${PROJECT_FILE}
QMAKE_CFLAGS += -std=c++11 \`pkg-config --cflags opencv\`
QMAKE_CXXFLAGS += -std=c++11 \`pkg-config --cflags opencv\`
LIBS += \`pkg-config --libs opencv\`
FIN

${QMAKE}
