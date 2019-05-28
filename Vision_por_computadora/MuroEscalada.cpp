#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;



int main(int, char**)
{
	//namedWindow("grayscale", CV_WINDOW_NORMAL);
	namedWindow("DiferenciaT", CV_WINDOW_NORMAL);
	namedWindow("DiferenciaColor", CV_WINDOW_NORMAL);
	namedWindow("Suavisado", CV_WINDOW_NORMAL);
	namedWindow("Result window", CV_WINDOW_NORMAL);
	/*namedWindow("Suavisado", CV_WINDOW_NORMAL);
	namedWindow("Ecualizada", CV_WINDOW_NORMAL);
	//namedWindow("MascaraBorde", CV_WINDOW_NORMAL);
	namedWindow("Threshold", CV_WINDOW_NORMAL);
	namedWindow("Morfologica", CV_WINDOW_NORMAL);*/

	Mat dst;

	Mat grayscale = cv::imread("07a.jpg", IMREAD_GRAYSCALE);
	Mat color = cv::imread("07a.jpg");
	//equalizeHist(grayscale, grayscale);

	Mat grayscale2 = cv::imread("07b.jpg", IMREAD_GRAYSCALE);
	Mat color2 = cv::imread("07b.jpg");
	//equalizeHist(grayscale2, grayscale2);

	Mat diff_total;
	absdiff(grayscale, grayscale2, diff_total);
	imshow("DiferenciaT", diff_total);

	Mat diff_total_color;
	absdiff(color2, color, diff_total_color);
	imshow("DiferenciaColor", diff_total_color);

	diff_total.convertTo(diff_total, -1, 0.5, 0);

	Mat ecualizada = Mat(grayscale.rows, grayscale.cols, CV_8U);
	equalizeHist(diff_total, ecualizada);
	//imshow("Ecualizada", ecualizada);

	Mat blur = Mat(grayscale.rows, grayscale.cols, CV_8U);
	GaussianBlur(ecualizada, blur, Size(7, 7), 1.0);
	medianBlur(blur, blur, 7);
	imshow("Suavisado", blur);

	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	RNG rng(12345);

	findContours(blur, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat drawing = Mat::zeros(grayscale2.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	imshow("Result window", drawing);

		



		/*	Mat morpologica = Mat(grayscale.rows, grayscale.cols, CV_8U);
			int morph_size = 2;
			Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
			morphologyEx(blur, morpologica, MORPH_BLACKHAT, element, Point(-1, -1), 2);
			imshow("Morfologica", morpologica);*/

		/*	Mat Threshold = Mat(grayscale.rows, grayscale.cols, CV_8U);
			adaptiveThreshold(blur, Threshold, 128, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 7, 2);
			imshow("Threshold", Threshold);*/


		/*	Mat borde = Mat(grayscale.rows, grayscale.cols, CV_8U);
			Canny(blur, borde, 50, 150);
			imshow("MascaraBorde", borde);*/

		/*	Mat morpologica = Mat(grayscale.rows, grayscale.cols, CV_8U);
			int morph_size = 4;
			Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
			morphologyEx(Threshold, morpologica, MORPH_GRADIENT, element, Point(-1, -1), 1);
			imshow("Morfologica", morpologica);*/

	waitKey(0);
	return 0;
}