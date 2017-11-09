
#include <QtGui>

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

#include "main_window.h"
#include "image_label.h"

Window::Window() : QMainWindow()
{
	setAcceptDrops(true);
	createWindow();
	connectSignal();
}

Window::~Window()
{
}

void Window::createWindow(void)
{
	int width = 640, height = 480;
	window = new QWidget;
	paintarea = new PaintArea(width, height);
	mainLayout = new QHBoxLayout;
	formLayout = new QVBoxLayout;
	applyButton = new QPushButton("Apply");
	fileNameEdit = new QLineEdit;

	paintarea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	formLayout->addWidget(fileNameEdit);
	formLayout->addWidget(applyButton);
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(paintarea);

	window->setLayout(mainLayout);
	setCentralWidget(window);
}

void Window::connectSignal(void)
{
	connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
}

void Window::apply(void)
{
	paintarea->loadPixmapImage("image.png");
	this->update();
}

