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

//Các hàm chính
vector<vector<int> > CountHistogram(Mat image, int numBin); //tính histogram của ảnh theo số bin là numBin
Mat cvtRGBtoHSV(Mat image); // Chuyển từ ma trận RGB sang HSV
Mat cvtHSVtoRGB(Mat image); // Chuyển từ ma trận HSV sang RGB

//Cân bằng lược đồ trên kênh H của ảnh
//Input: Ma trận ảnh màu RGB
//Output: Ma trận ảnh màu RGB sau khi có thay đổi
Mat HistogramEqualizationOnHSV(Mat image);
Mat HistEqualizationHSVImageOpencv(Mat image); //sử dụng hàm có sẵn opencv

//Cân bằng lược đồ xám
//Input: Ma trận ảnh màu xám
//Output: Ma trận ảnh màu xám sau khi có thay đổi
Mat HistogramEqualizationGrayImage(Mat image);
Mat HistEqualizationGrayImageOpencv(Mat image); //sử dụng hàm có sẵn opencv

//Cân bằng lược đồ trên ba kênh R,G,B
//Input: Ma trận ảnh màu RGB
//Output: Ma trận ảnh màu RGB sau khi có thay đổi
Mat HistogramEqualizationRGBImage(Mat image);
Mat HistEqualizationRGBImageOpencv(Mat image); //sử dụng hàm có sẵn opencv


// Hàm phụ trợ
Mat histogramImage(Mat image); // trả về ma trận ảnh Histogram trực quan từ ma trận ảnh đưa vào
bool isGrayImage(Mat); // Kiểm tra có là ảnh xám
void writeToPNGFormat(Mat);  // Viết từ ma trận màu ra file png
void showImage(Mat, string); // Hiển thị ảnh ra màn hình