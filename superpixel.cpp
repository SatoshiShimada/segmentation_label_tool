
#include <cmath>
#include "superpixel.h"

SuperPixel::SuperPixel(void)
{
	image_vec.resize(10);
}

SuperPixel::~SuperPixel()
{
}

void SuperPixel::loadImage(const char *filename)
{
	for(auto l : image_vec) {
		l.valid = false;
	}
	input_filename = std::string(filename);
	input_img = cv::imread(filename);
	width = input_img.cols;
	height = input_img.rows;
	label_mat = cv::Mat::zeros(height, width, CV_8UC1);
	label_img = input_img.clone();
	selected_labels.clear();
	zoom_image_index_x_start = new int[9]{
		0, width / 4, width / 2, 0, width / 4, width / 2, 0, width / 4, width / 2
	};
	zoom_image_index_x_end = new int[9]{
		width / 2, width - width / 4, width, width / 2, width - width / 4, width, width / 2, width - width / 4, width
	};
	zoom_image_index_y_start = new int[9]{
		0, 0, 0, height / 4, height / 4, height / 4, height / 2, height / 2, height / 2
	};
	zoom_image_index_y_end = new int[9]{
		height / 2, height / 2, height / 2, height - height /4, height - height / 4, height - height / 4, height, height, height
	};
	current_index = 0;
	cluster_num = 256;
	image_label.resize(width * height);
}

void SuperPixel::process(const double threshold)
{
	image_vec[0].label_vec.clear();
	cv::Mat gaussian_img = input_img.clone();
	cv::GaussianBlur(input_img, gaussian_img, cv::Size(3, 3), 0);
	cv::Mat lab_img = input_img.clone();
	cv::cvtColor(gaussian_img, lab_img, CV_BGR2Lab);
	image_vec[0].img = lab_img.clone();
	Slic slic;
	slic(lab_img, cluster_num, threshold, image_vec[0].label_vec, image_vec[0].centers);
	image_vec[0].cluster_num = cluster_num;
	image_vec[0].valid = true;
}

void SuperPixel::searchWhiteLine(const int x, const int y)
{
	const int label = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
	selected_labels.push_back(label);
}

QImage SuperPixel::drawWhiteLine(void)
{
	cv::Mat img = image_vec[current_index].img.clone();
	cv::Vec3b red(0, 0, 255);
	int xstart = 0;
	int ystart = 0;
	int ratio = 1;
	if(current_index != 0) {
		xstart = zoom_image_index_x_start[current_index - 1];
		ystart = zoom_image_index_y_start[current_index - 1];
		ratio = 2;
	}
	for(auto label : selected_labels) {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				const int l = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
				const int xx = xstart + x / ratio;
				const int yy = ystart + y / ratio;
				if(l == label) {
					img.at<cv::Vec3b>(y, x) = red;
				} else if(image_label[getLabelVecIndex(xx, yy)]) {
					img.at<cv::Vec3b>(y, x) = red;
				}
			}
		}
	}
	cvtColor(img, img, CV_Lab2RGB);
	QImage tmp_img((unsigned char *)img.data, width, height, img.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
}

QImage SuperPixel::getVisualizeImage(void)
{
	visualize_img = input_img.clone();
	visualizeImage(visualize_img, image_vec[current_index].label_vec, image_vec[current_index].centers);
	cv::cvtColor(visualize_img, visualize_img, CV_Lab2RGB);
	QImage tmp_img((unsigned char *)visualize_img.data, width, height, visualize_img.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
}

QImage SuperPixel::getZoomImage(void)
{
	const int index = current_index;
	if(!image_vec[index].valid) {
		zoomImage();
	}
	struct imageData &d = image_vec[index];
	d.vimg = d.img.clone();
	visualizeImage(d.vimg, d.label_vec, d.centers);
	cv::cvtColor(d.vimg, d.vimg, CV_Lab2RGB);
	QImage tmp_img((unsigned char *)d.vimg.data, width, height, d.vimg.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
}

void SuperPixel::visualizeImage(cv::Mat &out_img, const std::vector<int> in_label_vec, const std::vector<struct cluster_center> in_centers)
{
	for(int y = 1; y < height - 1; y++) {
		for(int x = 1; x < width - 1; x++) {
			const int label = in_label_vec[getLabelVecIndex(x, y)];
			cv::Vec3b v(in_centers[label].l, in_centers[label].a, in_centers[label].b);
			out_img.at<cv::Vec3b>(y, x) = v;
		}
	}
}

inline unsigned int SuperPixel::getLabelVecIndex(const int x, const int y)
{
	return (y * width + x);
}

void SuperPixel::undoSelectLabel(void)
{
	if(!selected_labels.empty())
		selected_labels.pop_back();
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
	for(auto label : selected_labels) {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int l = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
				if(l == label) {
					label_mat.at<unsigned char>(y, x) = 1;
					label_img.at<cv::Vec3b>(y, x) = red;
					//label_img[getLabelVecIndex(x, y)] = 1;
				}
			}
		}
	}
}

void SuperPixel::zoomImage()
{
	const int index = current_index;
	if(image_vec[index].valid) {
		if(image_vec[index].cluster_num != cluster_num) {
			Slic slic;
			slic(image_vec[index].img, cluster_num, 10, image_vec[index].label_vec, image_vec[index].centers);
		}
	} else {
		if(index < 1) {
			return;
		}
		cv::Mat &z = image_vec[index].img;
		z = input_img.clone();
		cv::Mat i = cv::Mat(height / 2, width / 2, CV_8UC3);
		const int xstart = zoom_image_index_x_start[index - 1];
		const int xend = zoom_image_index_x_end[index - 1];
		const int ystart = zoom_image_index_y_start[index - 1];
		const int yend = zoom_image_index_y_end[index - 1];
		for(int y = ystart; y < yend; y++) {
			for(int x = xstart; x < xend; x++) {
				i.at<cv::Vec3b>(y - ystart, x - xstart) = image_vec[0].img.at<cv::Vec3b>(y, x);
			}
		}
		cv::resize(i, z, z.size(), 0, 0);
		Slic slic;
		slic(z, cluster_num, 10, image_vec[index].label_vec, image_vec[index].centers);
		image_vec[index].valid = true;
	}
}

void SuperPixel::setClusterNum(const int in_cluster_num)
{
	cluster_num = in_cluster_num;
}

void SuperPixel::setIndex(const int index)
{
	if(index != current_index) {
		int xstart = 0;
		int ystart = 0;
		int ratio = 1;
		if(current_index != 0) {
			xstart = zoom_image_index_x_start[current_index - 1];
			ystart = zoom_image_index_y_start[current_index - 1];
			ratio = 2;
		}
		for(auto label : selected_labels) {
			for(int y = 0; y < height; y++) {
				for(int x = 0; x < width; x++) {
					int l = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
					if(l == label) {
						const int xx = xstart + x / ratio;
						const int yy = ystart + y / ratio;
						image_label[getLabelVecIndex(xx, yy)] = 1;
					}
				}
			}
		}
		selected_labels.clear();
		current_index = index;
	}
}

