#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace cv;

#define PI 3.14159265

// Hàm phụ trợ
bool isGrayImage(Mat); // Kiểm tra có là ảnh xám
void writeToPNGFormat(Mat);  // Viết từ ma trận màu ra file png
void showImage(Mat, string); // Hiển thị ảnh ra màn hình
bool CheckInImage(int x, int y, Mat image); // Kiểm tra điểm ảnh có thuộc ma trận ảnh
double Gauss(int i, int j, double sigma); // Hàm toán tử Guass
double LaplaceOfGauss(int i, int j, double sigma); // Hàm toán tử tại 1 vị trí trong LoG mask

// Các hàm chính
Mat Convolution(Mat image, Mat kernel); // tích chập image với kernel tương ứng
Mat ConvolutionForLaplace(Mat image, Mat kernel); // tích cập image và kernel với zero crossing, dùng cho toán tử laplace
pair<Mat, Mat> getGradKernel(double k); // trả về dạng kernel cơ bản của gradient

// các hàm trả về các kernel theo tên
pair <Mat, Mat> getSobel();
pair <Mat, Mat> getPrewitt();
pair <Mat, Mat> getScharr();
pair <Mat, Mat> getRobert();
Mat getLaplace();
Mat getLoG(double sigma);

//Hàm thực thi toán tử Gradient
Mat operateGradient(Mat image, pair<Mat, Mat> kernel);


