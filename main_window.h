
#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

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
#include "superpixel.h"

class Window : public QMainWindow
{
	Q_OBJECT
protected:
	void createWindow(void);
	void connectSignal(void);
	void updateZoomImage(void);
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
	QCheckBox *zoomCheckBox;
	QHBoxLayout *zoomButtonLayout;
	SuperPixel *sp;
	int zoomImageIndex;
public:
	Window();
	~Window();
private slots:
	void loadImage(void);
	void apply(void);
	void searchWhiteLine(int, int);
	void undo(void);
	void exportLabel(void);
	void zoomImage(int);
	void upZoomClicked(void);
	void downZoomClicked(void);
	void leftZoomClicked(void);
	void rightZoomClicked(void);
};

#endif //__MAIN_WINDOW_H__

