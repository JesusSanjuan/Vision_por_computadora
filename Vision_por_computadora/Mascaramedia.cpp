#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

int main(int, char**)
{

	namedWindow("Color", 1);
	namedWindow("Grises", 1);
	namedWindow("Mascara", 1);
	

	Mat color = imread("image2.jpg");	
	Mat gris = Mat(color.rows, color.cols, CV_8U);
	Mat suavisada = Mat(color.rows, color.cols, CV_8U);	

	for (;;)
	{
		cvtColor(color, gris, CV_RGB2GRAY);
		suavisada = gris;	

		for (int x = 1; x < gris.cols-1; x++) // Control de ventana
		{
			for (int y = 1; y < gris.rows-1; y++) // Control de ventana
			{
				int sumatoria = 0;	
				for (int xx = x-1; xx <= x+1; xx++)
				{
					for (int yy = y-1; yy <= y+1; yy++)
					{
						int intensidad = gris.at<uchar>(yy, xx);
						//std::cout << "Intensidades " << intensidad  << std::endl;
						sumatoria = sumatoria + intensidad;
					}
				}
				//std::cout << "Sumatorios " << (sumatoria/9) << std::endl;
				int tem = gris.at<uchar>(y, x);
				std::cout << "Valor viejo: " << tem << "      Valor promediado: " << sumatoria / 9 <<std::endl;
				suavisada.at<uchar>(y, x) = (sumatoria/9);						
			}
		}	
		imshow("Color", color);
		imshow("Grises", gris);
		imshow("Mascara", suavisada);
		waitKey(30);
	}
	return 0;

}