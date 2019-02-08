#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;


	namedWindow("grayscale", 1);

	Mat frame;
	cap >> frame; //get a new frame from camera


	Mat grayscale = Mat(frame.rows, frame.cols, CV_8U);



	//	for (;;)
		//{		
	for (int x = 0; x < frame.cols; x++)
	{
		for (int y = 0; y < frame.rows; y++)
		{
			Vec3b punto = frame.at<Vec3b>(y, x);

			float r = punto[2];
			float g = punto[1];
			float b = punto[0];

			grayscale.at<uchar>(y, x) = r * 0.2126 + g * 0.7152 + b * 0.0722;
		}
	}
	//imshow("imagen", frame); cuv8
	imshow("grayscale", grayscale);

	//	}
	int Histo[256] = {};

	for (int i = 0; i < grayscale.cols; i++)
	{
		for (int j = 0; j < grayscale.rows; j++)
		{
			uchar punto = grayscale.at<uchar>(j, i);

			int valoractual = punto;

			Histo[valoractual]++;
		}
	}
	int max = 0;
	int posMax = 0;
	for (int i = 0; i < 256; i++)
	{
		if (Histo[i] > max) {
			max = Histo[i];
			posMax = i;
		}
	}

	Mat Histograma =Mat(max+1, 255+1, CV_8U);

	

	for (int x = 0; x < 256; x++)
	{				
			//Histograma.at<uchar>(Histo[x], x) = 0;
		line(Histograma, Point(x, Histo[x]), Point(x + 1, Histo[x]), Scalar(255, 255, 255), 4, 0, 0);
		
	}
	

	std::cout << "Valor Maximo: "<< std::endl;

	namedWindow("Histograma",cv::WINDOW_AUTOSIZE);
	imshow("Histograma", Histograma);
	waitKey(0);
	return 0;

}