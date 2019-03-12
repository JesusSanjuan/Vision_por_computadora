#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
using namespace cv;

int main(int, char**)
{

	namedWindow("Grises", 1);
	namedWindow("MascaraBorde", 1);
	

	Mat color = imread("image4.jpg");	
	Mat gris = Mat(color.rows, color.cols, CV_8U);
	Mat borde = Mat(color.rows, color.cols, CV_8U);	
	
	cvtColor(color, gris, CV_RGB2GRAY);
	for (;;)
	{	
		int tem1, tem2, resultado1,resultado2;
		int Mascara[3][3] = { { 0,  1,  0},
		                      { 1, -4,  1}, /* los espacios y saltos de líneas no son tomados en cuenta */
		                      { 0,  1,  0} };

		for (int x = 1; x < gris.cols-1; x++) // Control de ventana
		{
			for (int y = 1; y < gris.rows-1; y++) // Control de ventana
			{
				int sumatoria = 0;	
				tem1 = 0; tem2 = 0;
				for (int xx = x-1; xx <= x+1; xx++)
				{
					for (int yy = y-1; yy <= y+1; yy++)
					{
						int intensidad = gris.at<uchar>(yy, xx);
						resultado1 = intensidad * Mascara[tem1][tem2];
						sumatoria = sumatoria + resultado1;
						tem1++;
						//std::cout << "Intensidades " << intensidad  << std::endl;						
					}tem1 = 0; tem2++;
				}				
				//std::cout << "Valor viejo: " << tem << "      Valor promediado: " << sumatoria / 9 <<std::endl;
				borde.at<uchar>(y, x) = abs((sumatoria));
			}
		}	
		
		imshow("Grises", gris);
		imshow("MascaraBorde", borde);
		waitKey(30);
	}
	return 0;

}