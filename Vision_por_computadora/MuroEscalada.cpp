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
cv::VideoCapture camera(0);
cv::Mat Primerfotocon;
if (!camera.isOpened())
	return -1;

namedWindow("Actual", CV_WINDOW_NORMAL);
namedWindow("template", CV_WINDOW_NORMAL);
namedWindow("Gris_Color", CV_WINDOW_NORMAL);
namedWindow("RESULTADO", CV_WINDOW_NORMAL);

std::cout << "Esperando Esc para tomar la segundoa foto con las piedras";

Mat actual;
camera >> actual;

for (;;)
{
	Mat actual;
	camera >> actual;

	if (cv::waitKey(1) == 27)
	{		
		camera >> Primerfotocon;
		cvtColor(Primerfotocon, Primerfotocon, CV_RGB2GRAY);
		namedWindow("CON", CV_WINDOW_NORMAL);
		imshow("CON", Primerfotocon);
		imwrite("template.jpg", Primerfotocon);
	}
	Mat grayscale2;
	cvtColor(actual, grayscale2, CV_RGB2GRAY);
	imshow("Actual", grayscale2);

	Mat templat = cv::imread("template.jpg", IMREAD_GRAYSCALE);
	imshow("template", templat);

	/*Convierte la imagen en escala de grices a color*/
	Mat graracolor;
	cvtColor(grayscale2, graracolor, CV_GRAY2RGB);
	imshow("Gris_Color", graracolor);
	/*Convierte la imagen en escala de grices a color*/
								
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
	waitKey(30);
}
return 0;
}