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
	namedWindow("Contraste", 1);

	Mat frame;
	cap >> frame; 

	Mat original = Mat(frame.rows, frame.cols, CV_8U);
	Mat contraste = Mat(frame.rows, frame.cols, CV_8U);

	int s1, s2, r1, r2;
	
	int iSliderValue1 = 255;
	createTrackbar("Entrada 1", "Contraste", &iSliderValue1, 255);

	int iSliderValue2 = 255;
	createTrackbar("Entrada 2", "Contraste", &iSliderValue2,255);

	int iSliderValue3 = 255;
	createTrackbar("Salida 1", "Contraste", &iSliderValue3, 255);

	int iSliderValue4 = 255;
	createTrackbar("Salida 2", "Contraste", &iSliderValue4, 255);

	for (;;)
	{
		Mat frame;
		cap >> frame; 
		
		cvtColor(frame, original, CV_RGB2GRAY);
		//std::cout << "Barra 1: " << iSliderValue1 <<std::endl;		

		r1 = iSliderValue1;
		r2 = iSliderValue2;

		s1 = iSliderValue3;
		s2 = iSliderValue4;		

		for (int x = 0; x < original.cols; x++)
		{
			for (int y = 0; y < original.rows; y++)
			{
				int intensidad = original.at<uchar>(y, x);

				if (0 <= intensidad && intensidad <= r1) 
				{
					contraste.at<uchar>(y, x) = s1 / r1 * intensidad;
				}
				else
				{
					if (r1 < intensidad && intensidad <= r2)
					{
						contraste.at<uchar>(y, x) = ((s2 - s1) / (r2 - r1)) * (intensidad - r1) + s1;
					}
					else
					{
						if (r2 < intensidad && intensidad <= 255)
						{
							contraste.at<uchar>(y, x) = ((255 - s2) / (255 - r2)) * (intensidad - r2) + s2;
						}
					}
				}
			}
		}		
		imshow("Imagen", original);
		imshow("Contraste", contraste);

		waitKey(30);
	}
	return 0;

}