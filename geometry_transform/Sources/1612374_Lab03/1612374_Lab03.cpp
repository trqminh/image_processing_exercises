#include "transformImage.h"

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

	if (strcmp(argv[2], "--rotate") == 0) {
		float param = atof(argv[3]);

		Mat resImage = RotatePreserveImage(image, param);

		showImage(resImage, "Rotate Image");
		
	}
	else if (strcmp(argv[2], "--rotateN") == 0){
		float param = atof(argv[3]);

		Mat resImage = RotateImage(image, param);

		showImage(resImage, "Rotate Image");

	}
	else if (strcmp(argv[2], "--scale") == 0) {
		float scaleX = atof(argv[3]);
		float scaleY = atof(argv[4]);

		Mat resImage = ScaleImage(image, scaleX, scaleY);

		showImage(resImage, "Scale Image");
	}


	// Hiển thị ảnh gốc
	showImage(image, "Original Image");
	waitKey(0);
	return 0;
}

