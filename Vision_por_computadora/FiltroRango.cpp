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


	namedWindow("grayscale", 1);

	Mat frame;
	cap >> frame; //get a new frame from camera


	Mat grayscale = Mat(frame.rows, frame.cols, CV_8U);

	//Create trackbar to change brightness
	int iSliderValue1 = 255;
	createTrackbar("Brightness", "grayscale", &iSliderValue1, 255);

	//Create trackbar to change contrast
	int iSliderValue2 = 255;
	createTrackbar("Contrast", "grayscale", &iSliderValue2,255);


	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera

		Mat grayscale;
		cvtColor(frame, grayscale, CV_RGB2GRAY);
		std::cout << "Barra 1: " << iSliderValue1 <<std::endl;
		//std::cout << "Barra 2: " << iSliderValue2 << std::endl;

		int rango1 = iSliderValue1;
		int rango2 = iSliderValue2;

		for (int x = 0; x < grayscale.cols; x++)
		{
			for (int y = 0; y < grayscale.rows; y++)
			{
				int intensidad = grayscale.at<uchar>(y, x);
				
				if (intensidad >= rango1 && intensidad <= rango2)
				{
					grayscale.at<uchar>(y, x) = 255;
				}

			}
		}


		//grayscale.convertTo(grayscale, -1, dContrast, iBrightness);

		
		
		imshow("grayscale", grayscale);

		waitKey(30);
	}
	return 0;

}