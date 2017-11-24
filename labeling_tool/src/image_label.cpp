
#include "image_label.h"

PaintArea::PaintArea(int width, int height) : QLabel(), width(width), height(height)
{
	mainPixmap = new QPixmap(width, height);
	mainPixmap->fill(qRgb(0, 0, 0));
	this->resize(width, height);
	this->setPixmap(*mainPixmap);
	this->update();
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton) {
		int x = event->x();
		int y = event->y();
		if(x < 0 || x > width) return;
		if(y < 0 || y > height) return;
		emit mousePressSignal(x, y);
	}
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
	if((event->button() & Qt::LeftButton)) {
		int x = event->x();
		int y = event->y();
		if(x < 0 || x > width) return;
		if(y < 0 || y > height) return;
		emit mouseReleaseSignal(x, y);
	}
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
	if(event->button() == Qt::NoButton) {
		int x = event->x();
		int y = event->y();
		if(x < 0 || x > width) return;
		if(y < 0 || y > height) return;
		emit mouseMoveSignal(x, y);
	}
}

void PaintArea::resetPixmapArea(void)
{
	mainPixmap->fill(Qt::black);
	this->setPixmap(*mainPixmap);
	this->update();
}

void PaintArea::savePixmapImage(const char *filename)
{
	QString qfilename = QString(filename);
	mainPixmap->save(qfilename);
}

void PaintArea::loadPixmapImage(const char *filename)
{
	QString qfilename = QString(filename);
	mainPixmap->load(qfilename);
	*mainPixmap = mainPixmap->scaled(width, height);
	this->setPixmap(*mainPixmap);
	this->update();
}

void PaintArea::setPixImage(const unsigned char *data)
{
	QImage image(width, height, QImage::Format_RGB32);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			image.setPixel(x, y, qRgb(
				(int)data[(y * width + x) * 3 + 0],
				(int)data[(y * width + x) * 3 + 1],
				(int)data[(y * width + x) * 3 + 2]));
		}
	}
	*mainPixmap = QPixmap::fromImage(image);
	this->setPixmap(*mainPixmap);
	this->update();
}

void PaintArea::setImage(QImage image)
{
	*mainPixmap = QPixmap::fromImage(image);
	this->setPixmap(*mainPixmap);
	this->update();
}

