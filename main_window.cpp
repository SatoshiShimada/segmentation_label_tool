
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
	exportButton = new QPushButton("Export");
	fileNameEdit = new QLineEdit("image.png");
	clusterNumEdit = new QLineEdit("256");

	paintarea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	formLayout->addWidget(fileNameEdit);
	formLayout->addWidget(clusterNumEdit);
	formLayout->addWidget(applyButton);
	formLayout->addWidget(exportButton);
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
	const int cluster_num = clusterNumEdit->text().toInt();
	slic->process(cluster_num, 2.0);
	QImage result = slic->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::searchWhiteLine(int x, int y)
{
	QImage result = slic->searchWhiteLine(x, y);
	paintarea->setImage(result);
	this->update();
}

