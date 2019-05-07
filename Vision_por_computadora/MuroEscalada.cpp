#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
using namespace cv;


const int alpha_slider_max = 255;
int alpha_slider = 128;

int Mascara[3][3] = { { 0,  1,  0},
					  { 1,  1,  1}, /* los espacios y saltos de líneas no son tomados en cuenta */
					  { 0,  1,  0} };

int Mascara2[3][3] = { { 0,  0,  0},
					  { 0,  0,  0}, /* los espacios y saltos de líneas no son tomados en cuenta */
					  { 0,  0,  0} };

int MascaraB[3][3] =      { { 1,  1,  1},
						  { 1, -8,  1}, /* los espacios y saltos de líneas no son tomados en cuenta */
						  { 1,  1,  1} };
void on_trackbar(int, void*)
{

	Mat src, dst;

	std::cout << "Barra 1: " << alpha_slider << std::endl;
	Mat grayscale = cv::imread("imagen.jpg", IMREAD_GRAYSCALE);
	imshow("grayscale", grayscale);

	equalizeHist(grayscale, grayscale);
	imshow("Ecualizada", grayscale);


	Mat borde = Mat(grayscale.rows, grayscale.cols, CV_8U);
	int tem1, tem2, resultado1, resultado2;

	for (int x = 1; x < grayscale.cols - 1; x++) // Control de ventana
	{
		for (int y = 1; y < grayscale.rows - 1; y++) // Control de ventana
		{
			int sumatoria = 0;
			tem1 = 0; tem2 = 0;
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				for (int yy = y - 1; yy <= y + 1; yy++)
				{
					int intensidad = grayscale.at<uchar>(yy, xx);
					resultado1 = intensidad * MascaraB[tem1][tem2];
					sumatoria = sumatoria + resultado1;
					tem1++;
					//std::cout << "Intensidades " << intensidad  << std::endl;						
				}tem1 = 0; tem2++;
			}
			//std::cout << "Valor viejo: " << tem << "      Valor promediado: " << sumatoria / 9 <<std::endl;
			borde.at<uchar>(y, x) = abs((sumatoria));
		}
	}

	
	imshow("MascaraBorde", borde);
	
	Mat binaria = Mat(grayscale.rows, grayscale.cols, CV_8U);
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
	imshow("Binaria", binaria);
	Mat Dilatacion = Mat(grayscale.rows, grayscale.cols, CV_64F);
	double p1 = 0; int resu = 0; tem1 = 0; tem2 = 0;
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
	imshow("Dilatacion", Dilatacion);

	Mat Erosion = Mat(grayscale.rows, grayscale.cols, CV_64F);
	Erosion = binaria.clone();
	p1 = 0; resu = 0;  tem1 = 0; tem2=0;
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
						resu = Mascara2[jj][ii];
						//if (resu == 1)
						//{
						int x = tem1 - 1; int y = tem2 - 1;
						Erosion.at<double>(y, x) = resu;
						//}
						tem2++;
					}tem1++;    tem2 = j;
				}
			}
		}
	}
	imshow("Erosion", Erosion);

}

int main(int, char**)
{	
		namedWindow("Erosion", 1);
		namedWindow("Dilatacion", 1);
		namedWindow("Binaria", 1);
		namedWindow("MascaraBorde", 1);
		namedWindow("Ecualizada", 1);
		namedWindow("grayscale", 1);

		createTrackbar("Rango 1", "grayscale", &alpha_slider, alpha_slider_max, on_trackbar);

		waitKey(0);
		return 0;
}