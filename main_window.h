
#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <iostream>
#include <vector>
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QRadioButton>
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
#include <QSpinBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

#include "image_label.h"
#include "superpixel.h"

class Window : public QMainWindow
{
	Q_OBJECT
protected:
	void createWindow(void);
	void connectSignal(void);
	void updateImage(void);
	QWidget *window;
	PaintArea *paintarea;
	QPushButton *loadButton;
	QPushButton *openButton;
	QPushButton *applyButton;
	QPushButton *exportButton;
	QPushButton *undoButton;
	QPushButton *leftButton;
	QPushButton *rightButton;
	QPushButton *upButton;
	QPushButton *downButton;
	QPushButton *nextImageButton;
	QRadioButton *whiteLineButton;
	QRadioButton *ballButton;
	QRadioButton *fieldButton;
	QRadioButton *robotButton;
	QRadioButton *goalPostButton;
	QRadioButton *personButton;
	QCheckBox *visibleBorderLineCheckBox;
	QLineEdit *fileNameEdit;
	QSpinBox *clusterNumSpin;
	QHBoxLayout *mainLayout;
	QVBoxLayout *formLayout;
	QGroupBox *zoomButtonGroup;
	QGroupBox *labelButtonGroup;
	QGridLayout *zoomButtonLayout;
	QHBoxLayout *clusterNumLayout;
	QHBoxLayout *fileNameLayout;
	QVBoxLayout *labelButtonLayout;
	QComboBox *chooseFileComboBox;
	SuperPixel *sp;
	unsigned int zoomImageIndex;
	unsigned int listFileIndex;
	QString listFileName;
	std::vector<std::string> listFile;
public:
	Window();
	~Window();
private slots:
	void loadImage(void);
	void apply(void);
	void searchWhiteLine(int, int);
	void undo(void);
	void exportLabel(void);
	void zoomImage(bool);
	void upZoomClicked(void);
	void downZoomClicked(void);
	void leftZoomClicked(void);
	void rightZoomClicked(void);
	void clusterNumChanged(int);
	void loadListFileSlot(void);
	void nextFileSlot(void);
	void fileIndexChanged(int);
	void setVisibleBorderLine(bool);
	void selectWhiteLine(bool);
	void selectBall(bool);
	void selectField(bool);
	void selectRobot(bool);
	void selectGoalPost(bool);
	void selectPerson(bool);
};

#endif //__MAIN_WINDOW_H__

