#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc.hpp"
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <iostream>
using namespace cv;



int main(int, char**)
{	
		namedWindow("grayscale", CV_WINDOW_NORMAL);
		namedWindow("Suavisado", CV_WINDOW_NORMAL);
		namedWindow("Ecualizada", CV_WINDOW_NORMAL);
		//namedWindow("MascaraBorde", CV_WINDOW_NORMAL);
		namedWindow("Threshold", CV_WINDOW_NORMAL);
		namedWindow("Morfologica", CV_WINDOW_NORMAL);

		Mat dst;
			
		Mat grayscale = cv::imread("07a.jpg", IMREAD_GRAYSCALE);
		imshow("grayscale", grayscale);

		Mat ecualizada = Mat(grayscale.rows, grayscale.cols, CV_8U);
		equalizeHist(grayscale, ecualizada);
		imshow("Ecualizada", ecualizada);

		Mat blur = Mat(grayscale.rows, grayscale.cols, CV_8U);
		GaussianBlur(ecualizada,blur, Size(7, 7),1.0);
		medianBlur(blur, blur, 7);
		imshow("Suavisado", blur);

		

	/*	Mat morpologica = Mat(grayscale.rows, grayscale.cols, CV_8U);
		int morph_size = 2;
		Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		morphologyEx(blur, morpologica, MORPH_BLACKHAT, element, Point(-1, -1), 2);
		imshow("Morfologica", morpologica);*/


		Mat Threshold = Mat(grayscale.rows, grayscale.cols, CV_8U);
		adaptiveThreshold(blur, Threshold, 128, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 7, 2);
		imshow("Threshold", Threshold);


		/*Mat borde = Mat(grayscale.rows, grayscale.cols, CV_8U);
		Canny(blur, borde, 50, 150);
		imshow("MascaraBorde", borde);*/


 		Mat morpologica = Mat(grayscale.rows, grayscale.cols, CV_8U);
		int morph_size = 4;
		Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		morphologyEx(Threshold, morpologica, MORPH_GRADIENT, element, Point(-1, -1), 1);		
		imshow("Morfologica", morpologica);

		waitKey(0);
		return 0;
}