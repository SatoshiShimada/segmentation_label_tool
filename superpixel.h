
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

struct imageData {
	cv::Mat img;
	cv::Mat vimg;
	int cluster_num;
	std::vector<struct cluster_center> centers;
	std::vector<int> label_vec;
	bool valid;
	imageData() : valid(false)
	{
	}
};

class SuperPixel
{
public:
	SuperPixel(void);
	~SuperPixel();
	void loadImage(const char *);
	void process(const double);
	void searchWhiteLine(const int, const int);
	QImage getVisualizeImage(void);
	QImage getZoomImage();
	QImage drawWhiteLine(void);
	void undoSelectLabel(void);
	void exportLabelData(const char *);
	void drawLabelImage(void);
	void zoomImage(void);
	void setClusterNum(const int);
	void setIndex(const int);
protected:
	cv::Mat input_img;
	cv::Mat visualize_img;
	cv::Mat label_mat;
	cv::Mat label_img;
	std::vector<int> selected_labels;
	std::vector<int> image_label;
	std::vector<struct imageData> image_vec;
	std::string input_filename;
	int width;
	int height;
	int current_index;
	int cluster_num;
	int *zoom_image_index_x_start;
	int *zoom_image_index_x_end;
	int *zoom_image_index_y_start;
	int *zoom_image_index_y_end;
	unsigned int getLabelVecIndex(const int, const int);
	void visualizeImage(cv::Mat &, const std::vector<int>, const std::vector<struct cluster_center>);
};

#endif // __SUPERPIXEL_H__

