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
	namedWindow("grayscale", CV_WINDOW_NORMAL);
	namedWindow("template", CV_WINDOW_NORMAL);
	//namedWindow("mask", CV_WINDOW_NORMAL);
	namedWindow("DiferenciaT", CV_WINDOW_NORMAL);
	namedWindow("Gris_Color", CV_WINDOW_NORMAL);
	namedWindow("MascaraBorde", CV_WINDOW_NORMAL); 
	namedWindow("MascaraBorde_Template", CV_WINDOW_NORMAL);
	namedWindow("Morfologica", CV_WINDOW_NORMAL);
	namedWindow("RESULTADO", CV_WINDOW_NORMAL);
	namedWindow("RESULTADO_Bordes", CV_WINDOW_NORMAL);

	Mat grayscale = cv::imread("FondoC.jpg", IMREAD_GRAYSCALE);	

	Mat grayscale2 = cv::imread("FondoS.jpg", IMREAD_GRAYSCALE);
	imshow("grayscale", grayscale2);

	Mat templat = cv::imread("template.jpg", IMREAD_GRAYSCALE);
	imshow("template", templat);

	/*Mat mask = Mat(templat.rows, templat.cols, CV_32F);
	threshold(templat, mask, 127, 255, THRESH_BINARY_INV);
	imshow("mask", mask);*/

	Mat diff_total;
	absdiff(grayscale, grayscale2, diff_total);
	imshow("DiferenciaT", diff_total);

	/*COnvierte la imagen en escala de grices a color*/
	Mat graracolor;
	cvtColor(grayscale2, graracolor, CV_GRAY2RGB);
	imshow("Gris_Color", graracolor);
	/*COnvierte la imagen en escala de grices a color*/

	Mat borde = Mat(grayscale.rows, grayscale.cols, CV_8U);
	Canny(diff_total, borde, 50, 150);
	imshow("MascaraBorde", borde); 

	/*COnvierte la deteccion de bordes en una imagen a color*/
	Mat MascaraBordecolor;
	cvtColor(borde, MascaraBordecolor, CV_GRAY2RGB);
	imshow("MascaraBordeColor", MascaraBordecolor);
	/*COnvierte la deteccion de bordes en una imagen a color*/

	Mat borde_Template = Mat(grayscale.rows, grayscale.cols, CV_8U);
	Canny(templat, borde_Template, 50, 150);
	imshow("MascaraBorde_Template", borde_Template);

	/*Mat Threshold = Mat(grayscale.rows, grayscale.cols, CV_8U);
	adaptiveThreshold(borde, Threshold, 128, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 7, 2);
	imshow("Threshold", Threshold);*/

	Mat morpologica = Mat(grayscale.rows, grayscale.cols, CV_8U);
	int morph_size = 1;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(borde, morpologica, MORPH_GRADIENT, element, Point(-1, -1), 1);
	imshow("Morfologica", morpologica);
	
	/* Deteccion de figura sobre imagen a escala de grices*/
	int result_cols = grayscale2.cols - templat.cols + 1;
	int result_rows = grayscale2.rows - templat.rows + 1;

	Mat result(result_rows, result_cols, CV_32FC1);

	cv::TemplateMatchModes match_mode = cv::TemplateMatchModes::TM_SQDIFF;
	matchTemplate(grayscale2, templat, result, match_mode);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;

	// obtener max y min junto con sus ubicaciones
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	if (match_mode == TM_SQDIFF || match_mode == TM_SQDIFF_NORMED) {
		matchLoc = minLoc;
	}
	else {
		matchLoc = maxLoc;
	}
	// dibujar el rectangulo en la posicion encontrada
	rectangle(graracolor, matchLoc, Point(matchLoc.x + templat.cols, matchLoc.y + templat.rows), Scalar(0, 0, 255), 2, CV_AA, 0);
	imshow("RESULTADO", graracolor);
	/* Deteccion de figura sobre imagen a escala de grices*/

	/* Deteccion de figura sobre imagen con bordes*/
	int result_cols_bordes = borde.cols - borde_Template.cols + 1;
	int result_rows_bordes = borde.rows - borde_Template.rows + 1;

	Mat result_bordes(result_rows_bordes, result_cols_bordes, CV_32FC1);

	cv::TemplateMatchModes match_mode_bordes = cv::TemplateMatchModes::TM_SQDIFF;
	matchTemplate(borde, borde_Template, result_bordes, match_mode_bordes);
	normalize(result_bordes, result_bordes, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal_bordes, maxVal_bordes;
	Point minLoc_bordes, maxLoc_bordes, matchLoc_bordes;

	// obtener max y min junto con sus ubicaciones
	minMaxLoc(result_bordes, &minVal_bordes, &maxVal_bordes, &minLoc_bordes, &maxLoc_bordes, Mat());

	if (match_mode_bordes == TM_SQDIFF || match_mode_bordes == TM_SQDIFF_NORMED) {
		matchLoc_bordes = minLoc_bordes;
	}
	else {
		matchLoc_bordes = maxLoc_bordes;
	}
	// dibujar el rectangulo en la posicion encontrada
	rectangle(MascaraBordecolor, matchLoc_bordes, Point(matchLoc_bordes.x + templat.cols, matchLoc.y + templat.rows), Scalar(0, 0, 255), 2, CV_AA, 0);
	imshow("RESULTADO_Bordes", MascaraBordecolor);
	/* Deteccion de figura sobre imagen con bordes*/
	
/*
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