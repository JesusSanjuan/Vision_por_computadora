#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;

	namedWindow("Imagen", 1);
	namedWindow("Mascara", 1);

	Mat frame;
	cap >> frame; 

	Mat original = Mat(frame.rows, frame.cols, CV_8U);
	Mat suavisada = Mat(frame.rows, frame.cols, CV_8U);

	

	for (;;)
	{
		Mat frame;
		cap >> frame; 
		
		cvtColor(frame, original, CV_RGB2GRAY);
		suavisada = original;
		//std::cout << "Barra 1: " << iSliderValue1 <<std::endl;		

			

		for (int x = 1; x < original.cols-1; x++) // Control de ventana
		{
			for (int y = 1; y < original.rows-1; y++) // Control de ventana
			{
				int sumatoria = 0;
				for (int xx = x-1; xx < x+1; xx++)
				{
					for (int yy = y-1; yy < y+1; yy++)
					{
						int intensidad = original.at<uchar>(yy, xx);
						sumatoria = sumatoria + intensidad;
					}
				}
				suavisada.at<uchar>(y, x) = sumatoria;						
			}
		}		
		imshow("Imagen", original);
	imshow("Mascara", suavisada);

		waitKey(30);
	}
	return 0;

}