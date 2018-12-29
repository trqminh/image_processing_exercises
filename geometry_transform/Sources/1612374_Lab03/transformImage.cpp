#include "transformImage.h"

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

pair<float, float> BackwardRotateFunction(int x, int y, float param, Mat image)
{
	float resX, resY;
	param = (param*PI*1.0) / 180; // chuyển từ độ sang radiant

	// xoay quanh tâm ảnh
	x -= (image.rows) / 2; 
	y -= (image.cols) / 2;

	//hàm ngược của phép quay góc
	resX = (float)((x*1.0 + y * tan(param)) / (cos(param) + sin(param)*tan(param)));
	resY = (float)((y*1.0 - x * tan(param)) / (cos(param) + sin(param)*tan(param)));

	//xoay quanh tâm ảnh
	resX += (image.rows*1.0) / 2;
	resY += (image.cols*1.0) / 2;

	// trả về cặp tọa độ x,y tương ứng
	return make_pair(resX, resY);
}

pair<float, float> BackwardScaleFunction(int x, int y, float scaleX, float scaleY)
{
	float resX, resY;

	// hàm ngược của phép scale ảnh
	resX = x * 1.0 / scaleX;
	resY = y * 1.0 / scaleY;

	return make_pair(resX, resY);
}

vector<uchar> interpolateColor(Mat image, float x, float y)
{
	// lấy giá trị sàn cho x,y
	int l = (int)floor(x); 
	int k = (int)floor(y);

	// tính khoảng cách đến điểm giá trị sàn
	float a = y * 1.0 - k;
	float b = x * 1.0 - l;

	vector<uchar> f(3, 255);

	// nội suy giá trị màu dựa vào các điểm lân cận
	if (CheckInSizeImage(l + 1, k + 1, image))
	{
		for (int c = 0; c < 3; c++) {
			//Tính f(x,k) = (1-a)*f(l,k) + a*f(l+1,k)
			uchar f1 = (uchar)((1 - a)*image.at<Vec3b>(l, k)[c] + a * image.at<Vec3b>(l+1, k)[c]);
			//Tính f(x,k+1) = (1-a)*f(l,k+1) + a*f(l+1,k+1)
			uchar f2 = (uchar)((1 - a)*image.at<Vec3b>(l, k + 1)[c] + a * image.at<Vec3b>(l + 1, k+1)[c]);
			//Tính f(x,y) = b*f(x,k) + (1-b)*f(x,k+1)
			f[c] = (uchar)(b * f2 + (1 - b)*f1);
		}
	}
	return f;
}

bool CheckInSizeImage(float x, float y, Mat image)
{
	//kiểm tra điểm x,y có thuộc ảnh
	return x >= 0 && y >= 0 && x < image.rows && y < image.cols;
}

//---
Mat RotateImage(Mat image, float param)
{
	Mat resultImage(image.rows, image.cols, CV_8UC3, Scalar(255, 255, 255));

	for (int i = 0; i < resultImage.rows; i++) {
		for (int j = 0; j < resultImage.cols; j++) {
			// tìm ảnh ngược của điểm trên ảnh gốc
			pair<float, float> reverseXY = BackwardRotateFunction(i, j, param, image);
			if (!CheckInSizeImage(reverseXY.first, reverseXY.second, image))
				continue;

			// nội suy giá trị màu
			vector<uchar> f_color = interpolateColor(image, reverseXY.first, reverseXY.second);
			for (int c = 0; c < 3; c++) {
				resultImage.at<Vec3b>(i, j)[c] = f_color[c];
			}
		}
	}

	return resultImage;
}

Mat MoveImage(Mat srcImage, pair<int,int> moveDistance, int desImageRows, int desImageCols)
{
	int xmove = moveDistance.first;
	int ymove = moveDistance.second;

	Mat moveImage(desImageRows, desImageCols, CV_8UC3, Scalar(255, 255, 255));

	for (int i = 0; i < moveImage.rows; i++) {
		for (int j = 0; j < moveImage.cols; j++) {
			for (int c = 0; c < 3; c++) {
				if (CheckInSizeImage(i - xmove, j - ymove, srcImage)) {
					moveImage.at<Vec3b>(i, j)[c] = srcImage.at<Vec3b>(i - xmove, j - ymove)[c];
				}
			}
		}
	}

	return moveImage;
}

Mat RotatePreserveImage(Mat image, float param)
{
	int newRows = image.rows + image.cols;
	int newCols = image.rows + image.cols;

	Mat moveImage = MoveImage(image, make_pair(newRows / 2 - image.rows / 2,
		newCols / 2 - image.cols / 2), newRows, newCols);

	// Tạo ảnh kết quả
	Mat originResultImage(newRows, newCols, CV_8UC3, Scalar(255, 255, 255));

	for (int i = 0; i < originResultImage.rows; i++) {
		for (int j = 0; j < originResultImage.cols; j++) {
			// tìm ảnh ngược của điểm trên ảnh gốc
			pair<float, float> reverseXY = BackwardRotateFunction(i, j, param, moveImage);
			if (!CheckInSizeImage(reverseXY.first, reverseXY.second, moveImage))
				continue;

			// nội suy giá trị màu
			vector<uchar> f_color = interpolateColor(moveImage, reverseXY.first, reverseXY.second);
			for (int c = 0; c < 3; c++) {
				originResultImage.at<Vec3b>(i, j)[c] = f_color[c];
			}
		}
	}

	// Tính toán khung ảnh mới để chứa vừa đủ ảnh
	float fix_param = param;
	// tìm góc quay cho tính toán khung ảnh
	while (fix_param >= 180)
	{
		fix_param -= 180;
	}

	if (fix_param > 90)
	{
		fix_param = 180 - fix_param;
	}

	fix_param = (fix_param*PI*1.0) / 180;

	int fixRows = (int)(image.rows*cos(fix_param) + 
		image.cols*sin(fix_param));
	int fixCols = (int)(image.rows*sin(fix_param) + 
		image.cols*cos(fix_param));

	//Tạo ảnh tịnh tiến vào khung ảnh mới
	Mat fixedResultImage = MoveImage(originResultImage, make_pair(fixRows / 2 - newRows / 2,
		fixCols / 2 - newCols / 2), fixRows, fixCols);

	return fixedResultImage;
}

Mat ScaleImage(Mat image, float scaleX, float scaleY)
{
	//Tính toán kích thước ảnh mới
	int newRows = image.rows*scaleX;
	int newCols = image.cols*scaleY;

	Mat resultImage(newRows, newCols, CV_8UC3, Scalar(255, 255, 255));
	
	for (int i = 0; i < resultImage.rows; i++) {
		for (int j = 0; j < resultImage.cols; j++) {
			// tìm ảnh ngược của điểm trên ảnh gốc
			pair<float, float> reverseXY = BackwardScaleFunction(i, j, scaleX,scaleY);
			if (!CheckInSizeImage(reverseXY.first, reverseXY.second, image))
				continue;

			// Nội suy giá trị màu
			vector<uchar> f_color = interpolateColor(image, reverseXY.first, reverseXY.second);
			for (int c = 0; c < 3; c++) {
				resultImage.at<Vec3b>(i, j)[c] = f_color[c];
			}
		}
	}


	return resultImage;
}