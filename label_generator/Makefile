
CFLAGS=-std=c++11 `pkg-config --cflags opencv`
LIBS=`pkg-config --libs opencv`

TARGET=Label_generator

all: label_generator

label_generator: label_generator.cpp
	g++ $(CFLAGS) -o $@ $^ $(LIBS)
	mv $@ ../$(TARGET)

