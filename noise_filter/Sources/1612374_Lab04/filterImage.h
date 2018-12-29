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

//Các hàm chính
//Các hàm chính đều có đầu vào gồm 2 tham số: ma trận ảnh, kích thước nhân
//Trừ 2 hàm lọc gauss có thêm tham số sigma
//Đầu ra của các hàm chính dưới đây đều có đầu ra là ma trận ảnh sau khi lọc

Mat MeanFilterGrayImage(Mat image, int multiplySize);
Mat MeanFilterColorImage(Mat image, int multiplySize);
Mat MedFilterGrayImage(Mat image, int multiplySize);
Mat MedFilterColorImage(Mat image, int multiplySize);
Mat GaussFilterGrayImage(Mat image, int multiplySize, double sigma);
Mat GaussFilterColorImage(Mat image, int multiplySize, double sigma);