#include "filterImage.h"

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

	if (strcmp(argv[2], "--mg") == 0) {
		int param = atoi(argv[3]);
		if (isGrayImage(image)) {
			cvtColor(image, image, CV_BGR2GRAY);
			Mat resImage = MeanFilterGrayImage(image, param);

			showImage(resImage, "Mean Filter Grayscale");
		}
		else {
			cout << "Invalid input!!";
		}
		
	}
	else if (strcmp(argv[2], "--mc") == 0){
		int param = atoi(argv[3]);
		Mat resImage = MeanFilterColorImage(image, param);

		showImage(resImage, "Mean Filter Color");


	}
	else if (strcmp(argv[2], "--meg") == 0) {
		int param = atoi(argv[3]);
		if (isGrayImage(image)) {
			cvtColor(image, image, CV_BGR2GRAY);
			Mat resImage = MedFilterGrayImage(image, param);

			showImage(resImage, "Median Filter Grayscale");
		}
		else {
			cout << "Invalid input!!";
		}
	}
	else if (strcmp(argv[2], "--mec") == 0) {
		int param = atoi(argv[3]);
		Mat resImage = MedFilterColorImage(image, param);

		showImage(resImage, "Median Filter Color");
	}
	else if (strcmp(argv[2], "--gg") == 0) {
		if (isGrayImage(image))
		{
			int size = atoi(argv[3]);
			double sigma = atof(argv[4]);
			cvtColor(image, image, CV_BGR2GRAY);

			Mat resImage = GaussFilterGrayImage(image, size, sigma);

			showImage(resImage, "Gauss Filter Gray");
		}
		else {
			cout << "Invalid input!!";
		}
	}
	else if (strcmp(argv[2], "--gc") == 0) {
		int size = atoi(argv[3]);
		double sigma = atof(argv[4]);
		
		Mat resImage = GaussFilterColorImage(image, size, sigma);

		showImage(resImage, "Gauss Filter Color");
	}

	// Hiển thị ảnh gốc
	showImage(image, "Original Image");
	waitKey(0);
	return 0;
}

