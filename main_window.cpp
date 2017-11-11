
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
	loadButton = new QPushButton("Load");
	applyButton = new QPushButton("Super pixel");
	exportButton = new QPushButton("Export");
	undoButton = new QPushButton("Undo");
	fileNameEdit = new QLineEdit("image.png");
	clusterNumEdit = new QLineEdit("256");
	rawImageButton = new QRadioButton("Raw");
	superpixelImageButton = new QRadioButton("Super pixel");
	zoomButtonGroup = new QGroupBox("Zoom");
	leftButton = new QPushButton("<");
	rightButton = new QPushButton(">");
	upButton = new QPushButton("^");
	downButton = new QPushButton("v");
	zoomButtonLayout = new QHBoxLayout;

	zoomButtonLayout->addWidget(leftButton);
	zoomButtonLayout->addWidget(rightButton);
	zoomButtonLayout->addWidget(upButton);
	zoomButtonLayout->addWidget(downButton);
	zoomButtonGroup->setLayout(zoomButtonLayout);

	paintarea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	formLayout->addWidget(fileNameEdit);
	formLayout->addWidget(clusterNumEdit);
	formLayout->addWidget(loadButton);
	formLayout->addWidget(applyButton);
	formLayout->addWidget(undoButton);
	formLayout->addWidget(zoomButtonGroup);
	formLayout->addWidget(exportButton);
	formLayout->addWidget(rawImageButton);
	formLayout->addWidget(superpixelImageButton);
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(paintarea);

	window->setLayout(mainLayout);
	setCentralWidget(window);
}

void Window::connectSignal(void)
{
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadImage()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	connect(paintarea, SIGNAL(mousePressSignal(int, int)), this, SLOT(searchWhiteLine(int, int)));
	connect(paintarea, SIGNAL(mouseMoveSignal(int, int)), this, SLOT(searchWhiteLine(int, int)));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undo()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(exportLabel()));
}

void Window::loadImage(void)
{
	QString filename = fileNameEdit->text();
	slic->loadImage(filename.toStdString().c_str());
	const int cluster_num = clusterNumEdit->text().toInt();
	slic->process(cluster_num, 2.0);
	QImage result = slic->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::apply(void)
{
	const int cluster_num = clusterNumEdit->text().toInt();
	slic->process(cluster_num, 2.0);
	QImage result = slic->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::undo(void)
{
	slic->undoSelectLabel();
	QImage result = slic->drawWhiteLine();
	paintarea->setImage(result);
	this->update();
}

void Window::searchWhiteLine(int x, int y)
{
	slic->searchWhiteLine(x, y);
	QImage result = slic->drawWhiteLine();
	paintarea->setImage(result);
	this->update();
}

void Window::exportLabel(void)
{
	QString filename = fileNameEdit->text();
	filename.replace(QString("png"), QString("txt"));
	slic->exportLabelData(filename.toStdString().c_str());
}

