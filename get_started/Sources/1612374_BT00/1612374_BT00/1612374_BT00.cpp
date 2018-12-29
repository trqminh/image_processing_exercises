#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <string>
#include <ctime>
using namespace std;
using namespace cv;

//Các hàm chính

// Biến đổi ảnh màu thành ảnh trắng đen
// Input: ma trận điểm ảnh
// Output: ma trận điểm ảnh sau khi biến đổi
Mat convertToGrayImage(Mat); 

// Biến đổi ảnh xám sang ảnh màu
// Input: ma trận ảnh xám
// Output: ma trận ảnh sau khi biến đổi
Mat convertFromGrayImageToColorImage(Mat);

// Thay đổi độ sáng
// Input: ma trận ảnh cần thay đổi độ sáng, hệ số alpha tương ứng
// Output: ma trận ảnh sau khi biến đổi
Mat changeBrightness(Mat, int); 

// Thay đổi độ tương phản
// Input: ma trận ảnh cần thay đổi độ tương phản, hệ số alpha tương ứng
// Output: ma trận ảnh sau khi biến đổi
Mat changeContrast(Mat, float);

//---------------------------------------------------------------------
// Hàm phụ trợ
void writeToPNGFormat(Mat);  // Viết từ ma trận màu ra file png
void showImage(Mat, string); // Hiển thị ảnh ra màn hình


int main(int argc, char** argv)
{
	if (argc != 3 && argc != 4)
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

	if (strcmp(argv[2], "gray") == 0) {
		Mat grayImage = convertToGrayImage(image);
		writeToPNGFormat(grayImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(grayImage, "Gray Image");
	}
	else if (!strcmp(argv[2], "brightness")) {
		int alpha = atoi(argv[3]);
		Mat newImage = changeBrightness(image, alpha);
		
		writeToPNGFormat(newImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(newImage, "Brightness Image");
	}
	else if (!strcmp(argv[2], "contrast")) {
		float alpha = atof(argv[3]);
		Mat newImage = changeContrast(image, alpha);
		
		writeToPNGFormat(newImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(newImage, "Contrast Image");
	}
	else if (strcmp(argv[2], "graytocolor") == 0) {
		Mat newImage = convertFromGrayImageToColorImage(image);
		
		writeToPNGFormat(newImage); // viết ảnh file png

		//hiển thị ảnh ra màn hình
		showImage(newImage, "Color Image");
	}
	else {
		cout << "Tham so khong hop le" << endl;
	}

	// Hiển thị ảnh gốc
	showImage(image, "Display window");

	waitKey(5000);
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

Mat convertToGrayImage(Mat image) {
	Mat grayImage;
	cvtColor(image, grayImage, CV_BGR2GRAY); //  hàm chuyển từ ảnh màu sang ảnh xám

	return grayImage;
}

Mat convertFromGrayImageToColorImage(Mat image) {
	//Ý tưởng của hàm dựa trên link:
	//https://www.researchgate.net/publication/310463241_Creating_a_Color_Map_to_be_used_to_Convert_a_Gray_Image_to_Color_Image

	Mat colorImage = image.clone();
	for (int i = 0; i < colorImage.rows; i++) {
		for (int j = 0; j < colorImage.cols; j++) {
			int gray_level = colorImage.at<Vec3b>(i, j)[0];
			if (gray_level >= 0 && gray_level <= 49) {
				//dark
				colorImage.at<Vec3b>(i, j)[0] = colorImage.at<Vec3b>(i, j)[1] = colorImage.at<Vec3b>(i, j)[2] = 0;
			}
			else if (gray_level >= 50 && gray_level <= 119) {
				//brown
				colorImage.at<Vec3b>(i, j)[0] = 33;
				colorImage.at<Vec3b>(i, j)[1] = 67;
				colorImage.at<Vec3b>(i, j)[2] = 101;

			}
			else if (gray_level >= 120 && gray_level <= 200) {
				//pink
				colorImage.at<Vec3b>(i, j)[0] = 180;
				colorImage.at<Vec3b>(i, j)[1] = 105;
				colorImage.at<Vec3b>(i, j)[2] = 255;
			}
			else {
				//yellow
				colorImage.at<Vec3b>(i, j)[0] = 0;
				colorImage.at<Vec3b>(i, j)[1] = 255;
				colorImage.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}

	return colorImage;
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