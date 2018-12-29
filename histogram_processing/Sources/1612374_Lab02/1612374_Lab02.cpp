#include "HistogramProcessing.h"

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

	if (strcmp(argv[2], "--hqgray") == 0) {
		if (isGrayImage(image)) {
			cvtColor(image, image, CV_BGR2GRAY);
			Mat histEqual = HistogramEqualizationGrayImage(image);
			Mat histEqualOpencv = HistEqualizationGrayImageOpencv(image);

			showImage(histEqual, "My Histogram Equalization");
			showImage(histEqualOpencv, "Opencv Histogram Equalization");
		}
		else {
			cout << "Chi thuc hien voi anh xam";
		}

	}
	else if (strcmp(argv[2], "--hqrgb") == 0){
		Mat resultImage = HistogramEqualizationRGBImage(image);
		Mat histEqualOpencv = HistEqualizationRGBImageOpencv(image);

		showImage(resultImage, "My Histogram Equalization RGB");
		showImage(histEqualOpencv, "Opencv Histogram Equalization");
	}
	else if (strcmp(argv[2], "--hqhsv") == 0) {
		Mat resultImage = HistogramEqualizationOnHSV(image);
		Mat histEqualOpencv = HistEqualizationHSVImageOpencv(image);

		showImage(histEqualOpencv, "Opencv HSV Histogram Equalization on H");
		showImage(resultImage, "My HSV Histogram Equalization on H");
	}

	// Hiển thị ảnh gốc
	showImage(image, "Original Image");
	waitKey(0);
	return 0;
}

