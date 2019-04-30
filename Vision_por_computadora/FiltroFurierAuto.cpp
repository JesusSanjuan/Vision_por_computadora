#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

const int alpha_slider_max = 255;
int alpha_slider=100;

int Mascara[3][3] = { { 0,  1,  0},
					  { 1,  1,  1}, /* los espacios y saltos de líneas no son tomados en cuenta */
					  { 0,  1,  0} };

void on_trackbar(int, void*)
{
	std::cout << "Barra 1: " << alpha_slider << std::endl;


	Mat grayscale = cv::imread("image4a.jpg", IMREAD_GRAYSCALE);

	Mat binaria = Mat(grayscale.rows, grayscale.cols, CV_8U);
	Mat Dilatacion = Mat(grayscale.rows, grayscale.cols, CV_8U);

	int rango1 = alpha_slider;

	for (int x = 0; x < grayscale.cols; x++)
	{
		for (int y = 0; y < grayscale.rows; y++)
		{
			int intensidad = grayscale.at<uchar>(y, x);

			if (intensidad >= rango1)
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

    int p1, resu = 0; int tem1, tem2; int ancho = grayscale.cols; int alto = grayscale.rows; int columna = 1; int fila = 1;
	int fC = 2, ff = 2; fC = fC - columna;  ff = ff - fila;
	int pixel;
	for (int j = fila; j < ancho - ff; j++)
	{
		for (int i = columna; i < alto - fC; i++)
		{
			p1 = binaria.at<uchar>(j, i);
			if (Mascara[fila][columna] == p1)
			{
				tem1 = j; tem2 = i;
				for (int ii = 0; ii < 3; ii++)
				{
					for (int jj = 0; jj < 3; jj++)
					{
						resu = Mascara[ii][jj];
						if (resu == 1)
						{
							Dilatacion.at<uchar>(tem2 - columna, tem1 - fila) = resu;
						}tem2++;
					}tem1++;    tem2 = i;
				}
			}
		}
	}
	Dilatacion.convertTo(Dilatacion, CV_64F);

	imshow("grayscale", grayscale);
	imshow("Binaria", binaria);
	imshow("Dilatacion", Dilatacion);
}

int main(int, char**)
{
	namedWindow("grayscale", 1);
	namedWindow("Binaria", 1);
	namedWindow("Dilatacion", 1);
	
	createTrackbar("Rango 1", "grayscale", &alpha_slider, alpha_slider_max, on_trackbar );
		
	waitKey(0);

    return 0;

}

