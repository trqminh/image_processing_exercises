#include "edgeDetection.h"

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
	
	//3 kênh có cùng giá trị
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

bool CheckInImage(int x, int y, Mat image)
{
	return x >= 0 && x < image.rows && y >= 0 && y < image.cols;
}

double Gauss(int i, int j, double sigma)
{
	double tmp = -(i*i + j * j) / (2.0 * sigma*sigma);
	return (1.0 / (sqrt(2 * PI)*sigma))*((double)exp(tmp));
}

double LaplaceOfGauss(int i, int j, double sigma)
{
	double tmp = (i*i + j * j) / (2.0 * sigma*sigma);

	double heso = 1.0 / (PI*sigma*sigma*sigma*sigma);
	
	return heso * (tmp - 1) * exp(-tmp);
}

Mat Convolution(Mat image, Mat kernel)
{
	Mat resultImage(image.rows, image.cols, CV_64FC1);
	int kernel_size = kernel.rows;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			double tmp_sum = 0;
			for (int k = -kernel_size / 2; k <= kernel_size / 2; k++) {
				for (int h = -kernel_size / 2; h <= kernel_size / 2; h++) {
					if (CheckInImage(i + k, j + h, image)) {
						tmp_sum += 1.0*image.at<uchar>(i + k, j + h)*
							kernel.at<double>(k + kernel_size / 2, h + kernel_size / 2);
					}
				}

			}
			resultImage.at<double>(i, j) = tmp_sum;
		}
	}

	return resultImage;
}

Mat ConvolutionForLaplace(Mat image, Mat kernel) {
	Mat resultImage(image.rows, image.cols, CV_8UC1);
	int kernel_size = kernel.rows;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			double tmp_sum = 0;
			for (int k = -kernel_size / 2; k <= kernel_size / 2; k++) {
				for (int h = -kernel_size / 2; h <= kernel_size / 2; h++) {
					if (CheckInImage(i + k, j + h, image)) {
						tmp_sum += 1.0*image.at<uchar>(i + k, j + h)*
							kernel.at<double>(k + kernel_size / 2, h + kernel_size / 2);
					}
				}

			}
			resultImage.at<uchar>(i, j) = saturate_cast<uchar>(tmp_sum);
		}
	}

	return resultImage;
}

pair<Mat, Mat> getGradKernel(double k) {


	Mat Wx(3, 3, CV_64FC1, new double[9]{ 1, 0, -1, k, 0, -k, 1, 0, -1 });
	Mat Wy(3, 3, CV_64FC1, new double[9]{ 1, k, 1, 0, 0, 0, -1, -k, -1 });


	return make_pair(1.0 / (k + 2) * Wx, 1.0 / (k + 2) * Wy);
}

pair <Mat, Mat> getSobel() {
	return getGradKernel(2.0);
}

pair <Mat, Mat> getPrewitt() {
	return getGradKernel(1.0);
}

pair <Mat, Mat> getScharr() {
	Mat Wx(3, 3, CV_64FC1, new double[9]{ -3,0,3,-10,0,10,-3,0,3 });
	Mat Wy(3, 3, CV_64FC1, new double[9]{ -3,-10,-3,0,0,0,3,10,3 });

	return make_pair(Wx, Wy);
}

pair<Mat, Mat> getRobert() {
	Mat Wx(3, 3, CV_64FC1, new double[9]{ 0,0,-1,0,1,0,0,0,0 });
	Mat Wy(3, 3, CV_64FC1, new double[9]{ -1,0,0,0,1,0,0,0,0 });

	return make_pair(Wx, Wy);
}

Mat getLaplace() {
	Mat Wx(3, 3, CV_64FC1, new double[9]{ 1,1,1,1,-8,1,1,1,1 });

	return Wx;
}

Mat getLoG(double sigma)
{
	double *x = new double[81];
	memset(x, 0, sizeof(x));
	Mat kern(9, 9, CV_64FC1, &x);

	for (int i = 0; i < kern.rows; i++) {
		for (int j = 0; j < kern.cols; j++) {
			kern.at<double>(i, j) = LaplaceOfGauss(i, j, sigma);
		}
	}

	return kern;
}


Mat operateGradient(Mat image, pair<Mat,Mat> kernel) {
	Mat gx = Convolution(image, kernel.first);
	Mat gy = Convolution(image, kernel.second);

	Mat resultImage = image.clone();

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			resultImage.at<uchar>(i, j) =
				saturate_cast<uchar>(sqrt(pow(gx.at<double>(i, j), 2) + 
					pow(gy.at<double>(i, j), 2)));
		}
	}
	
	return resultImage;
}


