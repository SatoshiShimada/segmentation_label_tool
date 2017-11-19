
#ifndef __SUPERPIXEL_H__
#define __SUPERPIXEL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
	void selectLabel(const int, const int);
	QImage getVisualizeImage(void);
	void undoSelectLabel(void);
	void exportLabelData(const char *);
	void zoomImage(void);
	void setClusterNum(const int);
	void setIndex(const int);
	void setVisibleBorderLine(bool);
	void drawClusterBorder(cv::Mat &);
protected:
	cv::Mat input_img;
	std::vector<int> selected_labels;
	std::vector<int> image_label;
	std::vector<struct imageData> image_vec;
	std::string input_filename;
	bool visible_border_line;
	int width;
	int height;
	int current_index;
	int cluster_num;
	int *zoom_image_index_x_start;
	int *zoom_image_index_x_end;
	int *zoom_image_index_y_start;
	int *zoom_image_index_y_end;
	inline unsigned int getLabelVecIndex(const int, const int);
	void saveSelectLabel(void);
};

#endif // __SUPERPIXEL_H__

