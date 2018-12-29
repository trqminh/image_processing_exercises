#include "HistogramProcessing.h"

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

//---
vector<vector<int> > CountHistogram(Mat image, int numBin) {
	vector<vector<int> > hist;

	hist.resize(image.channels()); // số histogram theo số kênh màu

	for (int i = 0; i < hist.size(); i++) {
		hist[i].resize(numBin, 0);
	}

	// cộng vào bin các thành phần màu từ ma trận
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < hist.size(); c++) {
				int value = image.at<Vec3b>(i, j)[c];
				hist[c][value / (256 / numBin)]++;
			}
		}
	}

	return hist;

}

Mat histogramImage(Mat image) {
	vector<vector<int> > hist = CountHistogram(image, 256);
	int numBin = hist[0].size();
	int hist_h = 400;
	int hist_w = 512;
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	int bin_w = hist_w / numBin;

	if (hist.size() == 1)
	{
		int max_hist = 0;
		for (int i = 0; i < numBin; i++) {
			max_hist = max(max_hist, hist[0][i]);
		}

		int ratio = (int)(max_hist / hist_h);
		for (int i = 1; i < numBin; i++) {
			line(histImage, Point(i*bin_w, hist_h - (int)(hist[0][i] / ratio)), 
				Point(bin_w*(i - 1), hist_h - (int)(hist[0][i - 1] / ratio)),
				Scalar(0, 0, 0), 2, 8, 0);
		}

	}
	else {

		vector<int>b_hist = hist[0];
		vector<int>g_hist = hist[1];
		vector<int>r_hist = hist[2];

		int max_hist_red = 0;
		int max_hist_blue = 0;
		int max_hist_green = 0;
		for (int i = 0; i < numBin; i++) {
			max_hist_blue = max(max_hist_blue, b_hist[i]);
			max_hist_green = max(max_hist_green, g_hist[i]);
			max_hist_red = max(max_hist_red, r_hist[i]);
		}

		// tính tỉ lệ giữa mức cao nhất tại mỗi bin và chiều cao của histogram image
		int ratio_red = (int)(max_hist_red / hist_h);
		int ratio_green = (int)(max_hist_green / hist_h);
		int ratio_blue = (int)(max_hist_blue / hist_h);

		// vẽ đường thẵng nối cái đỉnh của các bin màu để tạo histogram
		for (int i = 1; i < numBin; i++) {
			line(histImage, Point(i*bin_w, hist_h - (int)(b_hist[i] / ratio_blue)), 
				Point(bin_w*(i - 1), hist_h - (int)(b_hist[i - 1] / ratio_blue)),
				Scalar(255, 0, 0), 2, 8, 0);
			line(histImage, Point(i*bin_w, hist_h - (int)(g_hist[i] / ratio_green)),
				Point(bin_w*(i - 1), hist_h - (int)(g_hist[i - 1] / ratio_green)),
				Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, Point(i*bin_w, hist_h - (int)(r_hist[i] / ratio_red)), 
				Point(bin_w*(i - 1), hist_h - (int)(r_hist[i - 1] / ratio_red)),
				Scalar(0, 0, 255), 2, 8, 0);
		}
	}

	return histImage;
}

Mat HistogramEqualizationGrayImage(Mat image) {
	Mat resultImage = image.clone();

	if (image.channels() != 1)
		return resultImage;

	//B1: tính histogram độ xám
	vector<vector<int> > hist = CountHistogram(image, 256);

	int accumulateArray[256];
	memset(accumulateArray, 0, sizeof(accumulateArray));
	accumulateArray[0] = hist[0][0];

	//B2: Tính histogram độ sáng tích lũy
	for (int i = 1; i < 256; i++) {
		accumulateArray[i] = accumulateArray[i - 1] + hist[0][i];
	}

	//B3: Normalize
	for (int i = 0; i < 256; i++) {
		accumulateArray[i] = (int)((255.0 / (image.rows*image.cols))*accumulateArray[i]);
	}

	// B4: Tạo ảnh kết quả
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			resultImage.at<uint8_t>(i, j) = accumulateArray[image.at<uint8_t>(i, j)];
		}
	}

	return resultImage;
}

Mat HistogramEqualizationRGBImage(Mat image){
	Mat resultImage = image.clone();

	//B1: Tính histogram
	vector<vector<int> > hist = CountHistogram(image, 256);

	vector<vector<int> > accArr;
	accArr.resize(3);

	//B2,3: Tính histogram độ sáng tích lũy, normalize
	for (int i = 0; i < 3; i++) {
		accArr[i].resize(256, 0);
		accArr[i][0] = hist[i][0];
		for (int j = 1; j < 256; j++) {
			accArr[i][j] = accArr[i][j - 1] + hist[i][j];
		}

		for (int j = 0; j < 256; j++) {
			accArr[i][j] = (int)((255.0 / (image.rows*image.cols))*accArr[i][j]);
		}
	}
	
	//B4: Tạo ảnh kết quả 

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < 3; c++) {
				resultImage.at<Vec3b>(i, j)[c] = accArr[c][image.at<Vec3b>(i, j)[c]];
			}
		}
	}

	return resultImage;
}

Mat cvtRGBtoHSV(Mat image) {
	// Reference:https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	Mat hsvImage(image.rows, image.cols, CV_32FC3, Scalar(0.0, 0.0, 0.0));

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int b = image.at<Vec3b>(i, j)[0];
			int g = image.at<Vec3b>(i, j)[1];
			int r = image.at<Vec3b>(i, j)[2];

			int M = max(b, max(g, r));
			int m = min(b, min(g, r));
			int delta = M - m;
			float h, s, v;
			v = M;
			
			if (delta < 0.01) {
				s = 0;
				h = 0;
			}
			else {
				if (M <= 0) {
					s = 0;
					h = NAN;
				}
				else {
					s = delta*1.0 / M;

					if (r >= M)
						h = (g - b) / (delta*1.0);
					else if (g >= M)
						h = 2.0 + (b - r) / (delta*1.0);
					else
						h = 4.0 + (r - g) / (delta*1.0);

					h *= 60.0;
					if (h < 0) {
						h += 360.0;
					}
				}
			}

			hsvImage.at<Vec3f>(i, j)[0] = h;
			hsvImage.at<Vec3f>(i, j)[1] = s;
			hsvImage.at<Vec3f>(i, j)[2] = v;

		}
	}

	return hsvImage;
}

Mat cvtHSVtoRGB(Mat image) {
	// Reference:https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	Mat rgbImage(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			float h = image.at<Vec3f>(i, j)[0];
			float s = image.at<Vec3f>(i, j)[1];
			float v = image.at<Vec3f>(i, j)[2];

			int r, g, b;

			if (s <= 0.0) {       // < is bogus, just shuts up warnings
				r = v;
				g = v;
				b = v;
			}
			else {
				float hh, p, q, t, ff;

				hh = h;
				if (hh >= 360.0) hh = 0.0;
				hh /= 60.0;
				long k = (long)hh;
				ff = hh - k;
				p = v * (1.0 - s);
				q = v * (1.0 - (s * ff));
				t = v * (1.0 - (s * (1.0 - ff)));

				switch (k) {
				case 0:
					r = v;
					g = t;
					b = p;
					break;
				case 1:
					r = q;
					g = v;
					b = p;
					break;
				case 2:
					r = p;
					g = v;
					b = t;
					break;

				case 3:
					r = p;
					g = q;
					b = v;
					break;
				case 4:
					r = t;
					g = p;
					b = v;
					break;
				case 5:
				default:
					r = v;
					g = p;
					b = q;
					break;
				}
			}

			rgbImage.at<Vec3b>(i, j)[0] = r;
			rgbImage.at<Vec3b>(i, j)[1] = g;
			rgbImage.at<Vec3b>(i, j)[2] = b;
		}
	}

	return rgbImage;
}

Mat HistogramEqualizationOnHSV(Mat image) {
	Mat hsvImage = cvtRGBtoHSV(image);

	//B1: tính histogram độ xám
	vector<vector<int> > hist = CountHistogram(hsvImage, 256);

	int accumulateArray[256];
	memset(accumulateArray, 0, sizeof(accumulateArray));
	accumulateArray[0] = hist[0][0];

	//B2: Tính histogram độ sáng tích lũy
	for (int i = 1; i < 256; i++) {
		accumulateArray[i] = accumulateArray[i - 1] + hist[0][i];
	}

	//B3: Normalize
	for (int i = 0; i < 256; i++) {
		accumulateArray[i] = (int)((255.0 / (image.rows*image.cols))*accumulateArray[i]);
	}

	// B4: Tạo ảnh kết quả
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			hsvImage.at<Vec3f>(i, j)[0] = accumulateArray[(int)hsvImage.at<Vec3f>(i, j)[0]];
		}
	}

	Mat resultImage = cvtHSVtoRGB(hsvImage);
	return resultImage;
}

Mat HistEqualizationGrayImageOpencv(Mat image) {
	Mat resultImage;
	equalizeHist(image, resultImage);

	return resultImage;
}

Mat HistEqualizationRGBImageOpencv(Mat image) {
	// Reference: https://stackoverflow.com/questions/15007304/histogram-equalization-not-working-on-color-image-opencv
	vector<Mat> channels;
	split(image, channels);
	Mat B, G, R;

	equalizeHist(channels[0], B);
	equalizeHist(channels[1], G);
	equalizeHist(channels[2], R);
	vector<Mat> combined;
	combined.push_back(B);
	combined.push_back(G);
	combined.push_back(R);
	Mat result;
	merge(combined, result);
	return result;
}

Mat HistEqualizationHSVImageOpencv(Mat image) {
	Mat hsvimage;
	cvtColor(image, hsvimage, CV_BGR2HSV);

	vector<Mat> channels;
	split(hsvimage, channels);
	Mat H;

	equalizeHist(channels[0], H);

	vector<Mat> combined;
	combined.push_back(H);
	combined.push_back(channels[1]);
	combined.push_back(channels[2]);
	Mat result;
	merge(combined, result);

	cvtColor(result, result, COLOR_HSV2BGR);

	return result;
}