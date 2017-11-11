
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <iostream>
#include <vector>
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QDropEvent>
#include <QDrag>
#include <QUrl>
#include <QMimeData>
#include <QString>
#include <QFileDialog>

#include "image_label.h"
#include "slic.h"

class Window : public QMainWindow
{
	Q_OBJECT
protected:
	void createWindow(void);
	void connectSignal(void);
	QWidget *window;
	PaintArea *paintarea;
	QPushButton *loadButton;
	QPushButton *applyButton;
	QPushButton *exportButton;
	QPushButton *undoButton;
	QPushButton *leftButton;
	QPushButton *rightButton;
	QPushButton *upButton;
	QPushButton *downButton;
	QRadioButton *rawImageButton;
	QRadioButton *superpixelImageButton;
	QLineEdit *fileNameEdit;
	QLineEdit *clusterNumEdit;
	QHBoxLayout *mainLayout;
	QVBoxLayout *formLayout;
	QGroupBox *zoomButtonGroup;
	QHBoxLayout *zoomButtonLayout;
	Slic *slic;
public:
	Window();
	~Window();
private slots:
	void loadImage(void);
	void apply(void);
	void searchWhiteLine(int, int);
	void undo(void);
	void exportLabel(void);
};

#endif //__INTERFACE_H__

