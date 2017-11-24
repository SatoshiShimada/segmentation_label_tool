
#include <cmath>
#include "superpixel.h"

SuperPixel::SuperPixel(void)
{
	valid_image = false;
	visible_border_line = true;
	cluster_num = 256;
	image_vec.resize(10);
	default_width = 640;
	default_height = 480;
	current_object = OBJECT_WHITE_LINE;
	color_map = new int[21] {
		255, 0, 0,
		255, 255, 0,
		0, 255, 0,
		128, 0, 0,
		128, 128, 0,
		0, 128, 0,
		255, 128, 0,
	};
}

SuperPixel::~SuperPixel()
{
}

void SuperPixel::loadImage(const char *filename)
{
	for(std::size_t i = 0; i < image_vec.size(); i++) {
		image_vec[i].valid = false;
	}
	current_index = 0;
	image_label.clear();
	selected_labels.clear();
	input_filename = std::string(filename);
	try {
		input_img = cv::imread(filename);
		CV_Assert(input_img.type() == CV_8UC3);
	} catch(cv::Exception &e) {
		std::cerr << e.what() << std::endl;
		return;
	}
	valid_image = true;
	width = input_img.cols;
	height = input_img.rows;
	for(int i = 0; i < width * height; i++) {
		image_label.push_back(OBJECT_BACK_GROUND);
	}
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
}

void SuperPixel::process(const double threshold)
{
	if(!valid_image) return;
	saveSelectLabel();
	selected_labels.clear();
	image_vec[0].label_vec.clear();
	cv::Mat gaussian_img = input_img.clone();
	cv::GaussianBlur(input_img, gaussian_img, cv::Size(3, 3), 0);
	cv::Mat lab_img = input_img.clone();
	cv::cvtColor(gaussian_img, lab_img, CV_BGR2Lab);
	image_vec[0].img = lab_img.clone();
	cv::cvtColor(input_img, image_vec[0].vimg, CV_BGR2RGB);
	Slic slic;
	slic(lab_img, cluster_num, threshold, image_vec[0].label_vec, image_vec[0].centers);
	image_vec[0].cluster_num = cluster_num;
	image_vec[0].valid = true;
}

void SuperPixel::selectLabel(const int x, const int y)
{
	const int label = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
	selected_labels.push_back(label);
}

QImage SuperPixel::getVisualizeImage(void)
{
	if(!valid_image) {
		QImage tmp_img(default_width, default_height, QImage::Format_RGB888);
		QColor black;
		black.setRgb(0, 0, 0);
		tmp_img.fill(black);
		QImage result = tmp_img.copy();
		return result;
	}
	const int index = current_index;
	if(!image_vec[index].valid) {
		zoomImage();
	} else if(image_vec[index].cluster_num != cluster_num) {
		zoomImage();
	}
	cv::Vec3b color(
		color_map[current_object * 3 + 0],
		color_map[current_object * 3 + 1],
		color_map[current_object * 3 + 2]);
	struct imageData &d = image_vec[index];
	cv::Mat vimg = d.vimg.clone();
	for(auto l : selected_labels) {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				const int label = d.label_vec[getLabelVecIndex(x, y)];
				if(label == l) {
					vimg.at<cv::Vec3b>(y, x) = color;
				}
			}
		}
	}
	int xstart = 0;
	int ystart = 0;
	int ratio = 1;
	if(current_index != 0) {
		xstart = zoom_image_index_x_start[current_index - 1];
		ystart = zoom_image_index_y_start[current_index - 1];
		ratio = 2;
	}
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			const int xx = xstart + x / ratio;
			const int yy = ystart + y / ratio;
			const int object_type = image_label[getLabelVecIndex(xx, yy)];
			if(object_type != OBJECT_BACK_GROUND) {
				cv::Vec3b col(
					color_map[object_type * 3 + 0],
					color_map[object_type * 3 + 1],
					color_map[object_type * 3 + 2]);
				vimg.at<cv::Vec3b>(y, x) = col;
			}
		}
	}
	d.vimg = vimg;
	if(visible_border_line)
		drawClusterBorder(vimg);
	QImage tmp_img((unsigned char *)vimg.data, width, height, d.vimg.step, QImage::Format_RGB888);
	QImage result = tmp_img.copy();
	return result;
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
	if(!valid_image) return;
	saveSelectLabel();
	std::ofstream ofs;
	ofs.open(filename, std::ios::out);
	ofs << input_filename << std::endl;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(image_label[getLabelVecIndex(x, y)] != 0)
				ofs << x << " " << y << std::endl;
		}
	}
}

void SuperPixel::zoomImage()
{
	if(!valid_image) return;
	const int index = current_index;
	if(image_vec[index].valid) {
		if(image_vec[index].cluster_num != cluster_num) {
			Slic slic;
			slic(image_vec[index].img, cluster_num, 10, image_vec[index].label_vec, image_vec[index].centers);
			cv::cvtColor(image_vec[index].img, image_vec[index].vimg, CV_Lab2RGB);
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
		cv::cvtColor(z, image_vec[index].vimg, CV_Lab2RGB);
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
		saveSelectLabel();
		selected_labels.clear();
		current_index = index;
	}
}

void SuperPixel::saveSelectLabel(void)
{
	if(!valid_image) return;
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
					image_label[getLabelVecIndex(xx, yy)] = current_object;
				}
			}
		}
	}
}

void SuperPixel::drawClusterBorder(cv::Mat &visualize_img)
{
	cv::Vec3b blue(0, 0, 255);
	for(int y = 1; y < height - 1; y++) {
		for(int x = 1; x < width - 1; x++) {
			const int label = image_vec[current_index].label_vec[getLabelVecIndex(x, y)];
			if(
				image_vec[current_index].label_vec[getLabelVecIndex(x - 1, y    )] != label ||
				image_vec[current_index].label_vec[getLabelVecIndex(x + 1, y    )] != label ||
				image_vec[current_index].label_vec[getLabelVecIndex(x    , y - 1)] != label ||
				image_vec[current_index].label_vec[getLabelVecIndex(x    , y + 1)] != label
			) {
				visualize_img.at<cv::Vec3b>(y, x) = blue;
			}
		}
	}
}

void SuperPixel::setVisibleBorderLine(bool visible)
{
	visible_border_line = visible;
}

void SuperPixel::setObject(const int index)
{
	if(index >= OBJECT_BACK_GROUND && index <= OBJECT_PERSON)
		if(index != current_object)
			saveSelectLabel();
			selected_labels.clear();
			current_object = index;
}

