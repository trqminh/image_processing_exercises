#include "edgeDetection.h"

int main(int argc, char** argv) {

	//enable when show off all
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

	////Show all
	//cvtColor(image, image, CV_BGR2GRAY);
	//Mat sobel = operateGradient(image, getSobel());
	//showImage(sobel, "Sobel edge detection");

	//Mat prewitt = operateGradient(image, getPrewitt());
	//showImage(prewitt, "prewitt edge detection");

	//Mat lap = ConvolutionForLaplace(image, getLaplace());
	//showImage(lap, "laplacian");

	//Mat LoG = Convolution(image, getLoG(0.6));
	//showImage(LoG, "laplace of gauss");

	//Mat scharr = operateGradient(image, getScharr());
	//showImage(scharr, "scharr");

	//Mat robert = operateGradient(image, getRobert());
	//showImage(robert, "robert");


	if (strcmp(argv[2], "--gra-sobel") == 0) {
		
		cvtColor(image, image, CV_BGR2GRAY);
		Mat sobel = operateGradient(image, getSobel());
		

		showImage(sobel, "Sobel edge detection");
	}
	else if (strcmp(argv[2], "--gra-prewitt") == 0) {
		cvtColor(image, image, CV_BGR2GRAY);
		Mat prewitt = operateGradient(image, getPrewitt());

		
		showImage(prewitt, "prewitt edge detection");
	}
	else if (strcmp(argv[2], "--laplacian") == 0) {
		cvtColor(image, image, CV_BGR2GRAY);

		Mat lap = ConvolutionForLaplace(image, getLaplace());

		showImage(lap, "laplacian");
	}
	else if (strcmp(argv[2], "--log") == 0) {
		cvtColor(image, image, CV_BGR2GRAY);

		double sigma = atof(argv[3]);

		Mat LoG = Convolution(image, getLoG(sigma));

		showImage(LoG, "laplace of gauss");
	}
	else if (strcmp(argv[2], "--gra-scharr") == 0) {
		cvtColor(image, image, CV_BGR2GRAY);

		Mat scharr = operateGradient(image, getScharr());
		showImage(scharr, "scharr");
	}
	else if (strcmp(argv[2], "--gra-roberts") == 0) {
		cvtColor(image, image, CV_BGR2GRAY);

		Mat robert = operateGradient(image, getRobert());
		showImage(robert, "robert");
	}

	// Hiển thị ảnh gốc
	showImage(image, "Original Image");
	waitKey(0);
	return 0;
}

