#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

const int alpha_slider_max = 255;
int alpha_slider = 100;
VideoCapture cap(0);

int Mascara[3][3] = { { 0,  1,  0},
					  { 1,  1,  1}, /* los espacios y saltos de líneas no son tomados en cuenta */
					  { 0,  1,  0} };

int Mascara2[3][3] = { { 0,  0,  0},
					   { 0,  0,  0}, /* los espacios y saltos de líneas no son tomados en cuenta */
					   { 0,  0,  0} };

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;

	namedWindow("grayscale", WINDOW_AUTOSIZE);
	namedWindow("Binaria", WINDOW_AUTOSIZE);
	//namedWindow("Dilatacion", WINDOW_AUTOSIZE);
	//namedWindow("Erosion", WINDOW_AUTOSIZE);
	namedWindow("Cierre", WINDOW_AUTOSIZE);

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
		Mat Dilatacion = Mat(grayscale.rows, grayscale.cols, CV_64F);
		//Mat Erosion = Mat(grayscale.rows, grayscale.cols, CV_64F);
		Mat Cierre = Mat(grayscale.rows, grayscale.cols, CV_64F);
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

		double p1 = 0; int resu = 0; int tem1, tem2;
		for (int i = 1; i < grayscale.cols - 1; i++)
		{
			for (int j = 1; j < grayscale.rows - 1; j++)
			{
				p1 = binaria.at<double>(j, i);
				if (1 == p1)
				{
					tem1 = i; tem2 = j;
					for (int ii = 0; ii < 3; ii++)
					{
						for (int jj = 0; jj < 3; jj++)
						{
							resu = Mascara[jj][ii];
							if (resu == 1)
							{
								int x = tem1 - 1; int y = tem2 - 1;
								Dilatacion.at<double>(y, x) = resu;
							}
							tem2++;
						}tem1++;    tem2 = j;
					}
				}
			}
		}

		Cierre = Dilatacion.clone();
		p1 = 0;  resu = 0;  tem1=0; tem2=0;
		for (int i = 1; i < grayscale.cols - 1; i++)
		{
			for (int j = 1; j < grayscale.rows - 1; j++)
			{
				p1 = Dilatacion.at<double>(j, i);
				if (0 == p1)
				{
					tem1 = i; tem2 = j;
					for (int ii = 0; ii < 3; ii++)
					{
						for (int jj = 0; jj < 3; jj++)
						{
							resu = Mascara2[jj][ii];
							//if (resu == 1)
							//{
							int x = tem1 - 1; int y = tem2 - 1;
							Cierre.at<double>(y, x) = resu;
							//}
							tem2++;
						}tem1++;    tem2 = j;
					}
				}
			}
		}


		imshow("grayscale", grayscale);
		imshow("Binaria", binaria);
		//imshow("Dilatacion", Dilatacion);
		//imshow("Erosion", Erosion);
		imshow("Cierre", Cierre);
		waitKey(30);
	}

	return 0;
}
