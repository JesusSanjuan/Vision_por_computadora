#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

const int alpha_slider_max = 255;
int alpha_slider=128;

int Mascara[3][3] = { { 0,  0,  0},
					  { 0,  0,  0}, /* los espacios y saltos de líneas no son tomados en cuenta */
					  { 0,  0,  0} };

void on_trackbar(int, void*)
{
	std::cout << "Barra 1: " << alpha_slider << std::endl;

	Mat grayscale = cv::imread("Captura.jpg", IMREAD_GRAYSCALE);
	Mat binaria = Mat(grayscale.rows, grayscale.cols, CV_8U);
	Mat Dilatacion = Mat(grayscale.rows, grayscale.cols, CV_64F);
	
	int rango1 = alpha_slider;

	for (int x = 0; x < grayscale.cols; x++)
	{
		for (int y = 0; y < grayscale.rows; y++)
		{
			int intensidad = grayscale.at<uchar>(y, x);
			
			if (intensidad <= rango1)
			{
				binaria.at<uchar>(y, x) = 1;
			}
			else
			{				
				binaria.at<uchar>(y, x) = 0;
			}
		}
	}
	binaria.convertTo(binaria, CV_64F);

	Dilatacion = binaria.clone();
	double p1 = 0; int resu = 0; int tem1, tem2; 
	for (int i = 1; i < grayscale.cols - 1; i++)
	{
		for (int j = 1; j < grayscale.rows - 1; j++)
		{
			p1 = binaria.at<double>(j, i);
			if (0 == p1)
			{
				tem1 = i; tem2 = j;
				for (int ii = 0; ii < 3; ii++)
				{
					for (int jj = 0; jj < 3; jj++)
					{
						resu = Mascara[jj][ii];
						//if (resu == 1)
						//{
							int x = tem1 - 1; int y = tem2 - 1;
							Dilatacion.at<double>(y, x) = resu;
						//}
					    tem2++;
					}tem1++;    tem2 = j;
				}
			}
		}
	}
	imshow("grayscale", grayscale);
	imshow("Binaria", binaria);
	imshow("Erosion", Dilatacion);
}

int main(int, char**)
{
	namedWindow("grayscale", 1);
	namedWindow("Binaria", 1);
	namedWindow("Erosion", 1);
	
	createTrackbar("Rango 1", "grayscale", &alpha_slider, alpha_slider_max, on_trackbar );
		
	waitKey(0);
    return 0;
}

