
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
	sp = new SuperPixel;
	setAcceptDrops(true);
	createWindow();
	connectSignal();
	zoomImageIndex = 0;
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
	chooseFileComboBox = new QComboBox;
	loadButton = new QPushButton("Load");
	openButton = new QPushButton("Open");
	applyButton = new QPushButton("Super pixel");
	exportButton = new QPushButton("Export");
	undoButton = new QPushButton("Undo");
	fileNameEdit = new QLineEdit("image.png");
	clusterNumSpin = new QSpinBox;
	rawImageButton = new QRadioButton("Raw");
	superpixelImageButton = new QRadioButton("Super pixel");
	zoomButtonGroup = new QGroupBox("Zoom");
	zoomCheckBox = new QCheckBox("Zoom");
	leftButton = new QPushButton("<");
	rightButton = new QPushButton(">");
	upButton = new QPushButton("^");
	downButton = new QPushButton("v");
	zoomButtonLayout = new QHBoxLayout;

	clusterNumSpin->setMinimum(1);
	clusterNumSpin->setMaximum(10000);
	clusterNumSpin->setValue(256);

	zoomButtonLayout->addWidget(leftButton);
	zoomButtonLayout->addWidget(rightButton);
	zoomButtonLayout->addWidget(upButton);
	zoomButtonLayout->addWidget(downButton);
	zoomButtonGroup->setLayout(zoomButtonLayout);

	paintarea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	formLayout->addWidget(fileNameEdit);
	formLayout->addWidget(chooseFileComboBox);
	formLayout->addWidget(clusterNumSpin);
	formLayout->addWidget(loadButton);
	formLayout->addWidget(openButton);
	formLayout->addWidget(applyButton);
	formLayout->addWidget(undoButton);
	formLayout->addWidget(zoomCheckBox);
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
	connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(zoomImage(int)));
	connect(upButton, SIGNAL(clicked()), this, SLOT(upZoomClicked()));
	connect(downButton, SIGNAL(clicked()), this, SLOT(downZoomClicked()));
	connect(leftButton, SIGNAL(clicked()), this, SLOT(leftZoomClicked()));
	connect(rightButton, SIGNAL(clicked()), this, SLOT(rightZoomClicked()));
	connect(clusterNumSpin, SIGNAL(valueChanged(int)), this, SLOT(clusterNumChanged(int)));
	connect(openButton, SIGNAL(clicked()), this, SLOT(loadListFileSlot()));
	connect(chooseFileComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fileIndexChanged(int)));
}

void Window::loadImage(void)
{
	QString filename = fileNameEdit->text();
	sp->loadImage(filename.toStdString().c_str());
	sp->process(2.0);
	QImage result = sp->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::apply(void)
{
	sp->process(2.0);
	QImage result = sp->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::undo(void)
{
	sp->undoSelectLabel();
	QImage result = sp->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::searchWhiteLine(int x, int y)
{
	sp->selectLabel(x, y);
	QImage result = sp->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::exportLabel(void)
{
	QString filename = fileNameEdit->text();
	filename.replace(QString("png"), QString("txt"));
	sp->exportLabelData(filename.toStdString().c_str());
}

void Window::zoomImage(int state)
{
	if(state == Qt::Checked) {
		if(zoomImageIndex == 0)
			zoomImageIndex = 1;
		sp->setIndex(zoomImageIndex);
		updateZoomImage();
	} else {
		zoomImageIndex = 0;
		sp->setIndex(zoomImageIndex);
		updateZoomImage();
	}
}

void Window::upZoomClicked(void)
{
	if(zoomImageIndex - 3 >= 1)
		zoomImageIndex -= 3;
	updateZoomImage();
}

void Window::downZoomClicked(void)
{
	if(zoomImageIndex + 3 <= 9)
		zoomImageIndex += 3;
	updateZoomImage();
}

void Window::leftZoomClicked(void)
{
	if(zoomImageIndex - 1 >= 1)
		zoomImageIndex--;
	updateZoomImage();
}

void Window::rightZoomClicked(void)
{
	if(zoomImageIndex + 1 <= 9)
		zoomImageIndex++;
	updateZoomImage();
}

void Window::updateZoomImage(void)
{
	sp->setIndex(zoomImageIndex);
	QImage result = sp->getVisualizeImage();
	paintarea->setImage(result);
	this->update();
}

void Window::clusterNumChanged(int cluster_num)
{
	sp->setClusterNum(cluster_num);
}

void Window::loadListFileSlot(void)
{
	listFileName = QFileDialog::getOpenFileName(this, "Open Image list", "./");
	std::ifstream listFileStream(listFileName.toStdString().c_str());
	std::string line;
	listFile.clear();
	while(std::getline(listFileStream, line)) {
		listFile.push_back(line);
		chooseFileComboBox->addItem(QString::fromStdString(line));
	}
	if(listFile.size() != 0) {
		fileNameEdit->setText(QString(listFile[0].c_str()));
		loadImage();
	}
	listFileIndex = 0;
}

void Window::nextFileSlot(void)
{
	listFileIndex++;
	if(listFileIndex >= listFile.size())
		listFileIndex = std::max<int>(listFile.size() - 1, 0);
	fileNameEdit->setText(QString(listFile[listFileIndex].c_str()));
	loadImage();
	chooseFileComboBox->setCurrentIndex(listFileIndex);
}

void Window::fileIndexChanged(int index)
{
	if(listFileIndex != index) {
		listFileIndex = index;
		if(listFileIndex >= listFile.size())
			listFileIndex = std::max<int>(listFile.size() - 1, 0);
		chooseFileComboBox->setCurrentIndex(listFileIndex);
		fileNameEdit->setText(QString(listFile[listFileIndex].c_str()));
		loadImage();
	}
}

