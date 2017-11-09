
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

class Window : public QMainWindow
{
	Q_OBJECT
protected:
	void createWindow(void);
	void connectSignal(void);
	QWidget *window;
	PaintArea *paintarea;
	QPushButton *applyButton;
	QLineEdit *fileNameEdit;
	QHBoxLayout *mainLayout;
	QVBoxLayout *formLayout;
public:
	Window();
	~Window();
private slots:
	void apply(void);
};

#endif //__INTERFACE_H__

