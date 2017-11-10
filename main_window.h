
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
	QPushButton *applyButton;
	QPushButton *exportButton;
	QLineEdit *fileNameEdit;
	QLineEdit *clusterNumEdit;
	QHBoxLayout *mainLayout;
	QVBoxLayout *formLayout;
	Slic *slic;
public:
	Window();
	~Window();
private slots:
	void apply(void);
	void searchWhiteLine(int, int);
};

#endif //__INTERFACE_H__

