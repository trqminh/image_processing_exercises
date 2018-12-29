#include "filterImage.h"

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

Mat MeanFilterGrayImage(Mat image, int multiplySize)
{
	Mat resImage(image.rows, image.cols, CV_8UC1, Scalar(0));
	int tmp_sum = 0;

	// chỉ số bắt đầu và kết thúc ma trận nhân
	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			tmp_sum = 0;
			for (int k = start; k <= end; k++) {
				for (int h = start; h <= end; h++) {
					if (CheckInImage(i - k, j - h, image)) {
						tmp_sum += image.at<uchar>(i - k, j - h);
					}
				}
			}
			// chia cho kích thước của ma trận ảnh
			resImage.at<uchar>(i, j) = saturate_cast<uchar>(tmp_sum*1.0 / 
				(multiplySize*multiplySize));
		}
	}

	return resImage;
}

Mat MeanFilterColorImage(Mat image, int multiplySize)
{
	Mat resImage(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

	int tmp_sum = 0;

	// chỉ số bắt đầu và kết thúc ma trận nhân
	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < 3; c++) {
				tmp_sum = 0;
				for (int k = start; k <= end; k++) {
					for (int h = start; h <= end; h++) {
						if (CheckInImage(i - k, j - h, image)) {
							tmp_sum += image.at<Vec3b>(i - k, j - h)[c];
						}
					}
				}
				// chỉ số bắt đầu và kết thúc ma trận nhân
				resImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(tmp_sum*1.0 / 
					(multiplySize*multiplySize));
			}
		}
	}

	return resImage;

}

Mat MedFilterGrayImage(Mat image, int multiplySize)
{
	Mat resImage(image.rows, image.cols, CV_8UC1, Scalar(0));

	vector<uchar> find_med; // vector chứa các điểm lân cận

	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			find_med.resize(0);
			for (int k = start; k <= end; k++) {
				for (int h = start; h <= end; h++) {
					if (CheckInImage(i - k, j - h, image)) {
						find_med.push_back(image.at<uchar>(i - k, j - h));
					}
				}
			}
			
			// sắp xếp các điểm lân cận 
			sort(find_med.begin(), find_med.end());

			// Chọn điểm trung vị
			resImage.at<uchar>(i, j) = find_med[find_med.size() / 2];
		}
	}

	return resImage;
}

Mat MedFilterColorImage(Mat image, int multiplySize)
{
	Mat resImage(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

	vector<uchar> find_med;// vector chứa các điểm lân cận

	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < 3; c++) {
				find_med.resize(0);
				for (int k = start; k <= end; k++) {
					for (int h = start; h <= end; h++) {
						if (CheckInImage(i - k, j - h, image)) {
							find_med.push_back(image.at<Vec3b>(i - k, j - h)[c]);
						}
					}
				}

				// sắp xếp các điểm lân cận 
				sort(find_med.begin(), find_med.end());

				// Chọn điểm trung vị
				resImage.at<Vec3b>(i, j)[c] = find_med[find_med.size() / 2];
			}
		}
	}

	return resImage;
}

double Gauss(int i, int j, double sigma)
{
	double tmp = -(i*i + j * j) / (2.0 * sigma*sigma);
	return (1.0 / (sqrt(2 * PI)*sigma))*((double)exp(tmp));
}

Mat GaussFilterGrayImage(Mat image, int multiplySize, double sigma)
{
	Mat resImage(image.rows, image.cols, CV_8UC1, Scalar(0));
	int tmp_sum = 0;

	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			tmp_sum = 0;
			for (int k = start; k <= end; k++) {
				for (int h = start; h <= end; h++) {
					if (CheckInImage(i - k, j - h, image)) {
						// nhân với ma trận Gauss 
						tmp_sum += image.at<uchar>(i - k, j - h)*Gauss(k, h, sigma);
					}
				}
			}
			resImage.at<uchar>(i, j) = saturate_cast<uchar>(tmp_sum);
		}
	}

	return resImage;

}

Mat GaussFilterColorImage(Mat image, int multiplySize, double sigma)
{
	Mat resImage(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

	int tmp_sum = 0;

	int start = -(multiplySize / 2);
	int end = start + multiplySize - 1;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < 3; c++) {
				tmp_sum = 0;
				for (int k = start; k <= end; k++) {
					for (int h = start; h <= end; h++) {
						if (CheckInImage(i - k, j - h, image)) {
							// nhân với ma trận Gauss
							tmp_sum += image.at<Vec3b>(i - k, j - h)[c] * Gauss(k, h, sigma);
						}
					}
				}
				resImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(tmp_sum);
			}
		}
	}

	return resImage;
}