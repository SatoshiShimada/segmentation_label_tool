
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
	slic = new Slic;
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
	applyButton = new QPushButton("Super pixel");
	fileNameEdit = new QLineEdit("image.png");

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
	connect(paintarea, SIGNAL(mousePressSignal(int, int)), this, SLOT(searchWhiteLine(int, int)));
}

void Window::apply(void)
{
	QString filename = fileNameEdit->text();
	slic->loadImage(filename.toStdString().c_str());
	slic->process(256, 2.0);
	QImage result = slic->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::searchWhiteLine(int x, int y)
{
	slic->searchWhiteLine(x, y);
}

