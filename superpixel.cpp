
#include <cmath>
#include "superpixel.h"

SuperPixel::SuperPixel(void)
{
}

SuperPixel::~SuperPixel()
{
}

void SuperPixel::loadImage(const char *filename)
{
	input_filename = std::string(filename);
	input_img = cv::imread(filename);
	width = input_img.cols;
	height = input_img.rows;
	gaussian_img = input_img.clone();
	cv::GaussianBlur(input_img, gaussian_img, cv::Size(3, 3), 0);
	lab_img = input_img.clone();
	cv::cvtColor(gaussian_img, lab_img, CV_BGR2Lab);
	label_mat = cv::Mat::zeros(height, width, CV_8UC1);
	label_img = input_img.clone();
	centers.clear();
	label_vec.clear();
	whitelines_label.clear();
}

void SuperPixel::process(const int cluster_num, const double threshold)
{
	Slic slic;
	slic(lab_img, cluster_num, threshold, label_vec, centers);
}

void SuperPixel::searchWhiteLine(const int x, const int y)
{
	const int label = label_vec[getLabelVecIndex(x, y)];
	whitelines_label.push_back(label);
}

QImage SuperPixel::drawWhiteLine(void)
{
	cv::Mat img = label_img.clone();
	cv::Vec3b red(0, 0, 255);
	for(auto label : whitelines_label) {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				const int l = label_vec[getLabelVecIndex(x, y)];
				if(l == label) {
					img.at<cv::Vec3b>(y, x) = red;
				}
			}
		}
	}
	cvtColor(img, img, CV_BGR2RGB);
	QImage tmp_img((unsigned char *)img.data, width, height, img.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
}

QImage SuperPixel::getVisualizeImage(void)
{
	visualize_img = input_img.clone();
	for(int y = 1; y < height - 1; y++) {
		for(int x = 1; x < width - 1; x++) {
			const int label = label_vec[getLabelVecIndex(x, y)];
			cv::Vec3b v(centers[label].l, centers[label].a, centers[label].b);
			visualize_img.at<cv::Vec3b>(y, x) = v;
		}
	}
	cv::cvtColor(visualize_img, visualize_img, CV_Lab2RGB);
	QImage tmp_img((unsigned char *)visualize_img.data, width, height, visualize_img.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
}

inline unsigned int SuperPixel::getLabelVecIndex(const int x, const int y)
{
	return (y * width + x);
}

void SuperPixel::undoSelectLabel(void)
{
	if(!whitelines_label.empty())
		whitelines_label.pop_back();
}

void SuperPixel::exportLabelData(const char *filename)
{
	drawLabelImage();
	std::ofstream ofs;
	ofs.open(filename, std::ios::out);
	ofs << input_filename << std::endl;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			const unsigned char data = label_mat.at<unsigned char>(y, x);
			if(data != 0)
				ofs << x << " " << y << std::endl;
		}
	}
}

void SuperPixel::drawLabelImage(void)
{
	cv::Vec3b red(0, 0, 255);
	for(auto label : whitelines_label) {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int l = label_vec[getLabelVecIndex(x, y)];
				if(l == label) {
					label_mat.at<unsigned char>(y, x) = 1;
					label_img.at<cv::Vec3b>(y, x) = red;
				}
			}
		}
	}
}

