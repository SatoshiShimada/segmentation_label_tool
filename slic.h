
#ifndef __SLIC_H__
#define __SLIC_H__

#include <vector>

#include <cv.h>
#include <highgui.h>

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

class Slic
{
public:
	Slic();
	~Slic();
	void operator()(const cv::Mat, const int, const double, std::vector<int> &, std::vector<struct cluster_center> &);
protected:
	void process(const cv::Mat, const int, const double, std::vector<int> &, std::vector<struct cluster_center> &);
	int getLabelVecIndex(const int, const int);
	std::vector<struct cluster_center> centers;
	std::vector<int> label_vec;
	int width;
	int height;
};

#endif // __SLIC_H__

