
#ifndef __SLIC_H__
#define __SLIC_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <QImage>

class Slic
{
public:
	Slic(void);
	~Slic();
	void loadImage(const char *);
	void process(const int, const double);
	void searchWhiteLine(const int, const int);
	QImage getVisualizeImage(void);
	QImage drawWhiteLine(void);
	void undoSelectLabel(void);
	void exportLabelData(const char *);
	void drawLabelImage(void);
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
	std::string input_filename;
	int width;
	int height;
	unsigned int getLabelVecIndex(const int, const int);
};

#endif // __SLIC_H__

