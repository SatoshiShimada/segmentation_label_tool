
#ifndef __SUPERPIXEL_H__
#define __SUPERPIXEL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cv.h>
#include <highgui.h>

#include <QImage>

#include "slic.h"

struct zoomImageData {
	cv::Mat img;
	cv::Mat vimg;
	int cluster_num;
	std::vector<struct cluster_center> centers;
	std::vector<int> label_vec;
	bool valid;
	zoomImageData() : valid(false)
	{
	}
};

class SuperPixel
{
public:
	SuperPixel(void);
	~SuperPixel();
	void loadImage(const char *);
	void process(const int, const double);
	void searchWhiteLine(const int, const int);
	QImage getVisualizeImage(void);
	QImage getZoomImage(const int);
	QImage drawWhiteLine(void);
	void undoSelectLabel(void);
	void exportLabelData(const char *);
	void drawLabelImage(void);
	void zoomImage(const int);
protected:
	cv::Mat input_img;
	cv::Mat gaussian_img;
	cv::Mat lab_img;
	cv::Mat visualize_img;
	cv::Mat label_mat;
	cv::Mat label_img;
	std::vector<struct cluster_center> centers;
	std::vector<int> label_vec;
	std::vector<int> whitelines_label;
	std::vector<struct zoomImageData> zoom_image_vec;
	std::string input_filename;
	int width;
	int height;
	int *zoom_image_index_x_start;
	int *zoom_image_index_x_end;
	int *zoom_image_index_y_start;
	int *zoom_image_index_y_end;
	unsigned int getLabelVecIndex(const int, const int);
	void visualizeImage(cv::Mat &, const std::vector<int>, const std::vector<struct cluster_center>);
};

#endif // __SUPERPIXEL_H__

