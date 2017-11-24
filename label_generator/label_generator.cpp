
#include <iostream>
#include <string>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[])
{
	if(argc != 2) {
		std::cout << "Usage: ./label [label image name] < [pixel data]" << std::endl;
	}
	const char *img_fname = argv[1];
	const int height = 480;
	const int width = 640;
	cv::Mat label_img(height, width, CV_8UC1, cv::Scalar(0));
	std::string line;
	while(std::getline(std::cin, line)) {
		std::istringstream iss(line);
		int x, y, label;
		if(!(iss >> x >> y >> label)) { /* error */
			continue;
		}
		if(x >= width || y >= height) {
			std::cout << x << ", " << y << ", " << label << std::endl;
			continue;
		}
		label_img.at<unsigned char>(y, x) = label;
	}
	for(int h = 0; h < height; h++) {
		for(int w = 0; w < width; w++) {
			if(label_img.at<unsigned char>(h, w) == 255) {
				continue;
			}
			const int prev_h = std::max(h - 1, 0);
			const int next_h = std::min(h + 1, height - 1);
			const int prev_w = std::max(w - 1, 0);
			const int next_w = std::min(w + 1, width - 1);
			unsigned char p1 = label_img.at<unsigned char>(prev_h, prev_w);
			unsigned char p2 = label_img.at<unsigned char>(prev_h, w     );
			unsigned char p3 = label_img.at<unsigned char>(prev_h, next_w);
			unsigned char p4 = label_img.at<unsigned char>(h     , prev_w);
			unsigned char p6 = label_img.at<unsigned char>(h     , next_w);
			unsigned char p7 = label_img.at<unsigned char>(next_h, prev_w);
			unsigned char p8 = label_img.at<unsigned char>(next_h, w     );
			unsigned char p9 = label_img.at<unsigned char>(next_h, next_w);
			unsigned char label = label_img.at<unsigned char>(h, w);
			if(p1 == 255)
				p1 = label;
			if(p2 == 255)
				p2 = label;
			if(p3 == 255)
				p3 = label;
			if(p4 == 255)
				p4 = label;
			if(p6 == 255)
				p6 = label;
			if(p7 == 255)
				p7 = label;
			if(p8 == 255)
				p8 = label;
			if(p9 == 255)
				p9 = label;
			if(p1 != label || p2 != label || p3 != label || p4 != label || p6 != label || p7 != label || p8 != label || p9 != label) {
				label_img.at<unsigned char>(h, w) = 255;
			}
		}
	}
	cv::imwrite(img_fname, label_img);
	return 0;
}

