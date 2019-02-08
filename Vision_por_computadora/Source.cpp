#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;


	namedWindow("imagen", 1);
	namedWindow("grayscale", 1);

	Mat frame;
	cap >> frame; //get a new frame from camera


	Mat grayscale = Mat(frame.rows, frame.cols, CV_8U);

	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		for (int x = 0; x < frame.cols; x++)
			for (int y = 0; y < frame.rows; y++)
			{
				Vec3b punto = frame.at<Vec3b>(y, x);

				float r = punto[2];
				float g = punto[1];
				float b = punto[0];

				grayscale.at<uchar>(y, x) = r * 0.2126 + g * 0.7152 + b * 0.0722;
			}


		imshow("imagen", frame);
		imshow("grayscale", grayscale);
		waitKey(30);
	}
	return 0;

}