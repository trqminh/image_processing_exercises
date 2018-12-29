#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;
using namespace cv;

//Các hàm chính
Mat convertToGrayImage(Mat image);
Mat changeBrightness(Mat image, int alpha);
Mat changeContrast(Mat image, float alpha);
Mat convertToNegativeImage(Mat image);
Mat logTransform(Mat image, float alpha);
Mat gammaTransform(Mat image, float x, float y);
Mat countHistogram(Mat image);

// Hàm phụ trợ
bool isGrayImage(Mat);
void writeToPNGFormat(Mat);  // Viết từ ma trận màu ra file png
void showImage(Mat, string); // Hiển thị ảnh ra màn hình

int main(int argc, char** argv) {

	if (argc < 3)
	{
		cout << "Chuong trinh mo va hien thi anh, tham so ko hop le" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_COLOR);
	if (!image.data)
	{
		cout << "Khong the mo anh duoc" << std::endl;
		return -1;
	}

	if (strcmp(argv[2], "--g") == 0) {

		if (isGrayImage(image)) {
			cout << "Anh nhap vao la anh xam, chi thuc hien thao tac voi anh mau";
		}
		else {
			Mat grayImage = convertToGrayImage(image);

			//writeToPNGFormat(grayImage); // viết ảnh file png

			//hiển thị ảnh ra màn hình
			showImage(grayImage, "Gray Image");
		}
	}
	else if (strcmp(argv[2], "--b") == 0) {
		int alpha = atoi(argv[3]);
		Mat newImage = changeBrightness(image, alpha);

		//writeToPNGFormat(newImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(newImage, "Brightness Image");
	}
	else if (strcmp(argv[2], "--c") == 0) {
		float alpha = atof(argv[3]);
		Mat newImage = changeContrast(image, alpha);

		//writeToPNGFormat(newImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(newImage, "Contrast Image");
	}
	else if (strcmp(argv[2], "--n") == 0) {
		Mat negativeImage = convertToNegativeImage(image);
		//writeToPNGFormat(negativeImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(negativeImage, "Negative Image");
	}
	else if (strcmp(argv[2], "--lt") == 0) {
		float alpha = atof(argv[3]);

		Mat logTransImage = logTransform(image, alpha);

		//writeToPNGFormat(logTransImage); // viết ảnh file png

		showImage(logTransImage, "LogTransForm Image");
	}
	else if (strcmp(argv[2], "--gt") == 0) {
		float x = atof(argv[3]);
		float y = atof(argv[4]);
		Mat gammaTransImage = gammaTransform(image, x, y);

		//writeToPNGFormat(logTransImage); // viết ảnh file png

		showImage(gammaTransImage, "Gamma TransForm Image");
	}
	else if (strcmp(argv[2], "--hi") == 0) {
		Mat hist = countHistogram(image);

		showImage(hist, "Histogram");
	}
	

	// Hiển thị ảnh gốc
	showImage(image, "Original Image");
	waitKey(0);
	return 0;
}


void writeToPNGFormat(Mat image) {
	cout << "Nhap duong dan file output: ";
	string outpath;
	cin >> outpath; // nhập đường dẫn đến thư mục chưa file ouput
	outpath += ".png";

	try {
		vector <int> param_content; // tập các tham số cho các thông số của ảnh
		param_content.push_back(CV_IMWRITE_PNG_COMPRESSION); // Đưa độ nén của ảnh vào tập tham số
		imwrite(outpath, image, param_content);
	}
	catch (runtime_error& ex) {
		// không thể viết ra file png
		cout << "Khong the viet ra file png!" << endl;
		return;
	}
}

void showImage(Mat image, string windowName) {
	namedWindow(windowName, WINDOW_AUTOSIZE); //Tạo cửa sổ chứa ảnh
	imshow(windowName, image); // Hiển thị ảnh
}

bool isGrayImage(Mat image) {
	if (image.channels() == 1)
		return true;

	bool flag = true;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<Vec3b>(i, j)[0] != image.at<Vec3b>(i, j)[1] || image.at<Vec3b>(i, j)[1] != image.at<Vec3b>(i, j)[2]) {
				flag = false;
			}
		}
	}

	return flag;
}

//---
Mat convertToGrayImage(Mat image) {
	Mat grayImage = image.clone();

	for (int i = 0; i < grayImage.rows; i++) {
		for (int j = 0; j < grayImage.cols; j++) {
			int b = grayImage.at<Vec3b>(i, j)[0];
			int g = grayImage.at<Vec3b>(i, j)[1];
			int r = grayImage.at<Vec3b>(i, j)[2];
			grayImage.at<Vec3b>(i, j)[0] = grayImage.at<Vec3b>(i, j)[1] = grayImage.at<Vec3b>(i, j)[2] = (r + g + b) / 3;
		}
	}

	return grayImage;
}

Mat changeBrightness(Mat image, int alpha) {
	Mat newImage = image.clone();

	for (int i = 0; i < newImage.rows; i++) {
		for (int j = 0; j < newImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				//Cộng cho mỗi giá trị R,G,B của điểm ảnh một lượng alpha
				// saturate_cast<uchar> để tránh tràn số
				newImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>((newImage.at<Vec3b>(i, j)[c]) + alpha);
			}
		}
	}

	return newImage;
}

Mat changeContrast(Mat image, float alpha) {
	Mat newImage = image.clone();

	for (int i = 0; i < newImage.rows; i++) {
		for (int j = 0; j < newImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				// Nhân cho mỗi giá trị R,G,B của điểm ảnh một lượng alpha
				// saturate_cast<uchar> để tránh tràn số
				newImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(alpha*(newImage.at<Vec3b>(i, j)[c]));
			}
		}
	}

	return newImage;
}

Mat convertToNegativeImage(Mat image) {
	Mat negativeImage = image.clone();

	for (int i = 0; i < negativeImage.rows; i++) {
		for (int j = 0; j < negativeImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				// Nhân cho mỗi giá trị R,G,B của điểm ảnh một lượng alpha
				// saturate_cast<uchar> để tránh tràn số
				negativeImage.at<Vec3b>(i, j)[c] = 255 - negativeImage.at<Vec3b>(i, j)[c];
			}
		}
	}

	return negativeImage;
}

Mat logTransform(Mat image, float alpha) {
	Mat logTransImage = image.clone();

	for (int i = 0; i < logTransImage.rows; i++) {
		for (int j = 0; j < logTransImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				logTransImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(cv::log(logTransImage.at<Vec3b>(i, j)[c] + 1)*alpha);
			}
		}
	}

	return logTransImage;
}

Mat gammaTransform(Mat image, float x, float y) {
	Mat gammaTransImage = image.clone();

	for (int i = 0; i < gammaTransImage.rows; i++) {
		for (int j = 0; j < gammaTransImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				gammaTransImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(x*pow(gammaTransImage.at<Vec3b>(i, j)[c], y));
			}
		}
	}


	return gammaTransImage;
}

Mat countHistogram(Mat image) {
	int numBin = 16;
	vector<int>b_hist(numBin, 0);
	vector<int>g_hist(numBin, 0);
	vector<int>r_hist(numBin, 0);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int b = image.at<Vec3b>(i, j)[0];
			int g = image.at<Vec3b>(i, j)[1];
			int r = image.at<Vec3b>(i, j)[2];

			b_hist[b / numBin]++;
			g_hist[g / numBin]++;
			r_hist[r / numBin]++;
		}
	}

	int hist_h = image.rows*image.cols;
	int hist_w = numBin;
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	for (int i = 1; i < numBin; i++) {
		line(histImage, Point(i, hist_h - b_hist[i]), Point(i - 1, hist_h - b_hist[i - 1]),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(i, hist_h - g_hist[i]), Point(i - 1, hist_h - g_hist[i - 1]),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(i, hist_h - r_hist[i]), Point(i - 1, hist_h - r_hist[i - 1]),
			Scalar(0, 0, 255), 2, 8, 0);

	}

	return histImage;
}