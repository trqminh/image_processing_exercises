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
pair<float, float> BackwardRotateFunction(int x, int y, float param, Mat image);//Hàm ngược phép quay
pair<float, float> BackwardScaleFunction(int x, int y, float scaleX, float scaleY);//Hàm ngược phép scale
vector<uchar> interpolateColor(Mat image, float x, float y); //Hàm nội suy giá trị màu
bool CheckInSizeImage(float x, float y, Mat image);//Hàm kiểm tra tọa độ x,y có thuộc ảnh

//Hàm tịnh tiến ảnh từ 1 gốc sang 1 ảnh có kích thước khác với một độ dời nhất định
Mat MoveImage(Mat srcImage, pair<int, int> moveDistance, int desImageRows, int desImageCols);

//Các hàm chính

//Xoay không bảo toàn ảnh
Mat RotateImage(Mat image, float param);

//Xoay bảo toàn ảnh
Mat RotatePreserveImage(Mat image, float param);

//Scale ảnh
Mat ScaleImage(Mat image, float scaleX, float scaleY);