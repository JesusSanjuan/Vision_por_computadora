#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

const int alpha_slider_max = 255;
int alpha_slider = 100;
VideoCapture cap(0);

const int tamano=5;
const int centro = 2;

int Mascara[tamano][tamano] = { { 0,  0,  0, 0, 0 },
								  { 0,  0,  0, 0, 0 }, /* los espacios y saltos de líneas no son tomados en cuenta */
								  { 0,  0,  0, 0, 0 },
								  {	0,  0,  0, 0, 0 },
								  { 0,  0,  0, 0, 0 } };

int Mascara2[tamano][tamano] = { { 0,  0,  1, 0, 0 },
								   { 0,  1,  1, 1, 0 }, /* los espacios y saltos de líneas no son tomados en cuenta */
								   { 1,  1,  1, 1, 1 },
								   { 0,  1,  1, 1, 0 },
								   { 0,  0,  1, 0, 0 } };


int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;

	namedWindow("grayscale", WINDOW_AUTOSIZE);
	namedWindow("Binaria", WINDOW_AUTOSIZE);
	//namedWindow("Erosion", WINDOW_AUTOSIZE);
	namedWindow("Apertura", WINDOW_AUTOSIZE);
	Mat frame;
	cap >> frame; //get a new frame from camera	

	createTrackbar("Rango 1", "grayscale", &alpha_slider, alpha_slider_max);

	for (;;)
	{
		Mat frame;
		cap >> frame;

		Mat grayscale;
		cvtColor(frame, grayscale, CV_RGB2GRAY);

		Mat binaria = Mat(grayscale.rows, grayscale.cols, CV_8U);
		Mat Erosion = Mat(grayscale.rows, grayscale.cols, CV_64F);
		Mat Apertura = Mat(grayscale.rows, grayscale.cols, CV_64F);
		for (int x = 0; x < grayscale.cols; x++)
		{
			for (int y = 0; y < grayscale.rows; y++)
			{
				int intensidad = grayscale.at<uchar>(y, x);

				if (intensidad <= alpha_slider)
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

		Erosion = binaria.clone();
		double p1 = 0; int resu = 0; int tem1, tem2;
		for (int i = centro; i < grayscale.cols - centro; i++)
		{
			for (int j = centro; j < grayscale.rows - centro; j++)
			{
				p1 = binaria.at<double>(j, i);
				if (0 == p1)
				{
					tem1 = i; tem2 = j;
					for (int ii = 0; ii < tamano; ii++)
					{
						for (int jj = 0; jj < tamano; jj++)
						{
							resu = Mascara[jj][ii];
							//if (resu == 1)
							//{
							int x = tem1 - centro; int y = tem2 - centro;
							Erosion.at<double>(y, x) = resu;
							//}
							tem2++;
						}tem1++;    tem2 = j;
					}
				}
			}
		}

		p1 = 0;  resu = 0;  tem1 = 0; tem2 = 0;
		for (int i = centro; i < grayscale.cols - centro; i++)
		{
			for (int j = centro; j < grayscale.rows - centro; j++)
			{
				p1 = Erosion.at<double>(j, i);
				if (1 == p1)
				{
					tem1 = i; tem2 = j;
					for (int ii = 0; ii < tamano; ii++)
					{
						for (int jj = 0; jj < tamano; jj++)
						{
							resu = Mascara2[jj][ii];
							if (resu == 1)
							{
								int x = tem1 - centro; int y = tem2 - centro;
								Apertura.at<double>(y, x) = resu;
							}
							tem2++;
						}tem1++;    tem2 = j;
					}
				}
			}
		}

		imshow("grayscale", grayscale);
		imshow("Binaria", binaria);
		//imshow("Erosion", Erosion);
		imshow("Apertura", Apertura);
		waitKey(30);
	}

	return 0;
}
