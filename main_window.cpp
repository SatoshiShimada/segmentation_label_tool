
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
	const int width = 640, height = 480;
	window = new QWidget;
	paintarea = new PaintArea(width, height);
	mainLayout = new QHBoxLayout;
	formLayout = new QVBoxLayout;
	chooseFileComboBox = new QComboBox;
	loadButton = new QPushButton("Load");
	openButton = new QPushButton("Load image list");
	nextImageButton = new QPushButton("Next image");
	applyButton = new QPushButton("Apply");
	exportButton = new QPushButton("Export");
	undoButton = new QPushButton("Undo");
	fileNameEdit = new QLineEdit("image file name");
	clusterNumSpin = new QSpinBox;
	visibleBorderLineCheckBox = new QCheckBox("Visible border line");
	zoomButtonGroup = new QGroupBox("Zoom");
	leftButton = new QPushButton("<");
	rightButton = new QPushButton(">");
	upButton = new QPushButton("^");
	downButton = new QPushButton("v");
	zoomButtonLayout = new QGridLayout;
	fileNameLayout = new QHBoxLayout;
	clusterNumLayout = new QHBoxLayout;

	visibleBorderLineCheckBox->setChecked(true);

	zoomButtonGroup->setCheckable(true);
	zoomButtonGroup->setChecked(false);

	clusterNumSpin->setMinimum(1);
	clusterNumSpin->setMaximum(10000);
	clusterNumSpin->setValue(256);

	fileNameLayout->addWidget(fileNameEdit);
	fileNameLayout->addWidget(loadButton);

	clusterNumLayout->addWidget(clusterNumSpin);
	clusterNumLayout->addWidget(applyButton);

	zoomButtonLayout->addWidget(leftButton, 2, 1);
	zoomButtonLayout->addWidget(rightButton, 2, 3);
	zoomButtonLayout->addWidget(upButton, 1, 2);
	zoomButtonLayout->addWidget(downButton, 3, 2);
	zoomButtonGroup->setLayout(zoomButtonLayout);

	paintarea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	formLayout->addLayout(fileNameLayout);
	formLayout->addWidget(openButton);
	formLayout->addWidget(chooseFileComboBox);
	formLayout->addWidget(nextImageButton);
	formLayout->addLayout(clusterNumLayout);
	formLayout->addWidget(undoButton);
	formLayout->addWidget(zoomButtonGroup);
	formLayout->addWidget(exportButton);
	formLayout->addWidget(visibleBorderLineCheckBox);
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
	connect(zoomButtonGroup, SIGNAL(toggled(bool)), this, SLOT(zoomImage(bool)));
	connect(upButton, SIGNAL(clicked()), this, SLOT(upZoomClicked()));
	connect(downButton, SIGNAL(clicked()), this, SLOT(downZoomClicked()));
	connect(leftButton, SIGNAL(clicked()), this, SLOT(leftZoomClicked()));
	connect(rightButton, SIGNAL(clicked()), this, SLOT(rightZoomClicked()));
	connect(clusterNumSpin, SIGNAL(valueChanged(int)), this, SLOT(clusterNumChanged(int)));
	connect(openButton, SIGNAL(clicked()), this, SLOT(loadListFileSlot()));
	connect(chooseFileComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fileIndexChanged(int)));
	connect(nextImageButton, SIGNAL(clicked()), this, SLOT(nextFileSlot()));
	connect(visibleBorderLineCheckBox, SIGNAL(toggled(bool)), this, SLOT(setVisibleBorderLine(bool)));
}

void Window::loadImage(void)
{
	zoomButtonGroup->setChecked(false);
	clusterNumSpin->setValue(256);
	QString filename = fileNameEdit->text();
	sp->loadImage(filename.toStdString().c_str());
	const double threshold = 2.0;
	sp->process(threshold);
	updateImage();
}

void Window::apply(void)
{
	const double threshold = 2.0;
	sp->process(threshold);
	updateImage();
}

void Window::undo(void)
{
	sp->undoSelectLabel();
	updateImage();
}

void Window::searchWhiteLine(int x, int y)
{
	sp->selectLabel(x, y);
	updateImage();
}

void Window::exportLabel(void)
{
	QString filename = fileNameEdit->text();
	filename.replace(QString("png"), QString("txt"));
	sp->exportLabelData(filename.toStdString().c_str());
}

void Window::zoomImage(bool on)
{
	if(on) {
		if(zoomImageIndex == 0)
			zoomImageIndex = 1;
		sp->setIndex(zoomImageIndex);
		updateImage();
	} else {
		zoomImageIndex = 0;
		sp->setIndex(zoomImageIndex);
		updateImage();
	}
}

void Window::upZoomClicked(void)
{
	if((signed)zoomImageIndex - 3 >= 1)
		zoomImageIndex -= 3;
	sp->setIndex(zoomImageIndex);
	updateImage();
}

void Window::downZoomClicked(void)
{
	if((signed)zoomImageIndex + 3 <= 9)
		zoomImageIndex += 3;
	sp->setIndex(zoomImageIndex);
	updateImage();
}

void Window::leftZoomClicked(void)
{
	if((signed)zoomImageIndex - 1 >= 1)
		zoomImageIndex--;
	sp->setIndex(zoomImageIndex);
	updateImage();
}

void Window::rightZoomClicked(void)
{
	if((signed)zoomImageIndex + 1 <= 9)
		zoomImageIndex++;
	sp->setIndex(zoomImageIndex);
	updateImage();
}

void Window::updateImage(void)
{
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
	if(listFileIndex != (unsigned int)index) {
		listFileIndex = (unsigned int)index;
		if(listFileIndex >= listFile.size())
			listFileIndex = std::max<int>(listFile.size() - 1, 0);
		chooseFileComboBox->setCurrentIndex(listFileIndex);
		fileNameEdit->setText(QString(listFile[listFileIndex].c_str()));
		loadImage();
	}
}

void Window::setVisibleBorderLine(bool checked)
{
	sp->setVisibleBorderLine(checked);
	updateImage();
}

