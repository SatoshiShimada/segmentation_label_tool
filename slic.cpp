
#include <iostream>
#include <cmath>
#include "slic.h"

struct cluster_center
{
	int l;
	int a;
	int b;
	int x;
	int y;
	cluster_center() : l(0), a(0), b(0), x(0), y(0)
	{
	}
};

double calc_error(struct cluster_center c1, struct cluster_center c2)
{
	double error;
	error = sqrt(
		(c1.x - c2.x) * (c1.x - c2.x) +
		(c1.y - c2.y) * (c1.y - c2.y));
	return error;
}

double calc_distance_color(struct cluster_center c1, struct cluster_center c2)
{
	double distance = sqrt(
		(c1.x - c2.x) * (c1.x - c2.x) +
		(c1.y - c2.y) * (c1.y - c2.y));
	if(distance > 50) return std::numeric_limits<double>::max();
	distance = sqrt(
		(c1.l - c2.l) * (c1.l - c2.l) +
		(c1.a - c2.a) * (c1.a - c2.a) +
		(c1.b - c2.b) * (c1.b - c2.b));
	return distance;
}

double calc_distance(struct cluster_center c1, struct cluster_center c2)
{
	double distance;
	distance = sqrt(
		(c1.l - c2.l) * (c1.l - c2.l) +
		(c1.a - c2.a) * (c1.a - c2.a) +
		(c1.b - c2.b) * (c1.b - c2.b) +
		(c1.x - c2.x) * (c1.x - c2.x) +
		(c1.y - c2.y) * (c1.y - c2.y));
	return distance;
}

cv::Vec3b operator-(const cv::Vec3b v1, const cv::Vec3b v2)
{
	cv::Vec3b ret;
	ret[0] = v1[0] - v2[0];
	ret[1] = v1[1] - v2[1];
	ret[2] = v1[2] - v2[2];
	return ret;
}

double sqrt_vec(const cv::Vec3b v)
{
	return sqrt(
		v[0] * v[0] +
		v[1] * v[1] +
		v[2] * v[2]);
}

Slic::Slic(void)
{
}

Slic::~Slic()
{
}

void Slic::loadImage(const char *filename)
{
	input_img = cv::imread(filename);
	width = input_img.cols;
	height = input_img.rows;
	gaussian_img = input_img.clone();
	cv::GaussianBlur(input_img, gaussian_img, cv::Size(3, 3), 0);
	lab_img = input_img.clone();
	cv::cvtColor(gaussian_img, lab_img, CV_BGR2Lab);
}

void Slic::process(const int cluster_num, const double threshold)
{
	const int pixel_num = height * width;
	const int grid_steps = (int)sqrt((double)(pixel_num / cluster_num));
	/* initialization */
	for(int y = grid_steps / 2; y < height; y += grid_steps) {
		for(int x = grid_steps / 2; x < width; x += grid_steps) {
			double min_gradient = std::numeric_limits<double>::max();
			int new_x = x;
			int new_y = y;
			for(int yy = -1; yy <= 1; yy++) {
				for(int xx = -1; xx <= 1; xx++) {
					const cv::Vec3b dx = lab_img.at<cv::Vec3b>(y + yy, x + xx - 1) - lab_img.at<cv::Vec3b>(y + yy, x + xx + 1);
					const cv::Vec3b dy = lab_img.at<cv::Vec3b>(y + yy - 1, x + xx) - lab_img.at<cv::Vec3b>(y + yy + 1, x + xx);
					const double gradient = sqrt_vec(dx) + sqrt_vec(dy);
					if(gradient < min_gradient) {
						min_gradient = gradient;
						new_x = x + xx;
						new_y = y + yy;
					}
				}
			}
			struct cluster_center c;
			c.l = lab_img.at<cv::Vec3b>(new_y, new_x)[0];
			c.a = lab_img.at<cv::Vec3b>(new_y, new_x)[1];
			c.b = lab_img.at<cv::Vec3b>(new_y, new_x)[2];
			c.x = new_x;
			c.y = new_y;
			centers.push_back(c);
		}
	}
	label_vec = std::vector<int>(pixel_num, -1);
	std::vector<double> distance_vec(pixel_num, std::numeric_limits<double>::infinity());

	double error = std::numeric_limits<double>::max();
	do {
		/* assignment */
		int label = 0;
		for(auto center : centers) {
			for(int y = center.y - grid_steps; y < height && y < center.y + grid_steps; y++) {
				if(y < 0) continue;
				for(int x = center.x - grid_steps; x < width && x < center.x + grid_steps; x++) {
					if(x < 0) continue;
					struct cluster_center c;
					c.l = lab_img.at<cv::Vec3b>(y, x)[0];
					c.a = lab_img.at<cv::Vec3b>(y, x)[1];
					c.b = lab_img.at<cv::Vec3b>(y, x)[2];
					c.x = x;
					c.y = y;
					double d = calc_distance(center, c);
					if(d < distance_vec[y * width + x]) {
						distance_vec[y * width + x] = d;
						label_vec[getLabelVecIndex(x, y)] = label;
					}
				}
			}
			label++;
		}
		/* update */
		std::vector<std::vector<struct cluster_center> > labels(centers.size());
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int l = label_vec[getLabelVecIndex(x, y)];
				if(l == -1) continue;
				struct cluster_center c;
				c.l = lab_img.at<cv::Vec3b>(y, x)[0];
				c.a = lab_img.at<cv::Vec3b>(y, x)[1];
				c.b = lab_img.at<cv::Vec3b>(y, x)[2];
				c.x = x;
				c.y = y;
				labels[l].push_back(c);
			}
		}
		double e = 0.0;
		for(unsigned int i = 0; i < centers.size(); i++) {
			if(labels[i].size() == 0) continue;
			struct cluster_center c;
			for(auto lab : labels[i]) {
				c.l += lab.l;
				c.a += lab.a;
				c.b += lab.b;
				c.x += lab.x;
				c.y += lab.y;
			}
			c.l /= labels[i].size();
			c.a /= labels[i].size();
			c.b /= labels[i].size();
			c.x /= labels[i].size();
			c.y /= labels[i].size();
			e += calc_error(centers[i], c);
			centers[i] = c;
		}
		if(e != 0.0) error = e;
	} while(error > threshold);

	/* post processing */
	for(int y = 1; y < height - 1; y++) {
		for(int x = 1; x < width - 1; x++) {
			const int label = label_vec[getLabelVecIndex(x, y)];
			if(label != -1) continue;
			const int new_label = label_vec[getLabelVecIndex(x - 1, y)];
			label_vec[getLabelVecIndex(x, y)] = new_label;
		}
	}
}

void Slic::searchWhiteLine(const int x, const int y)
{
	const int label = label_vec[getLabelVecIndex(x, y)];
}

QImage Slic::getVisualizeImage(void)
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

inline unsigned int Slic::getLabelVecIndex(const int x, const int y)
{
	return (y * width + x);
}

