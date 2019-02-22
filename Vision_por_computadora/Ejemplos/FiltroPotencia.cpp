#include "opencv2/opencv.hpp"
#include <math.h> 
using namespace cv;
using namespace std;
int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << "Usage: display_Image ImageToLoadandDisplay" << endl;
		return -1;
	}
	else {

		//double gama = 6;
		double gama = atof(argv[1]);

		VideoCapture cap(0); //Abrimos la camara default
		if (!cap.isOpened()) //checamos si fue exitoso
			return -1;




		namedWindow("grayscale", WINDOW_AUTOSIZE);
		namedWindow("transformada", WINDOW_AUTOSIZE);

		Mat frame;
		cap >> frame; //get a new frame from camera


		Mat grayscale = Mat(frame.rows, frame.cols, CV_8U);

		Mat transformada = Mat(frame.rows, frame.cols, CV_8U);

		for (;;)
		{
			Mat frame;
			cap >> frame; // get a new frame from camera
			for (int x = 0; x < frame.cols; x++) {
				for (int y = 0; y < frame.rows; y++)
				{
					Vec3b punto = frame.at<Vec3b>(y, x);

					float r = punto[2];
					float g = punto[1];
					float b = punto[0];

					grayscale.at<uchar>(y, x) = r * 0.2126 + g * 0.7152 + b * 0.0722;
				}
			}

			//Recorro la matriz grayscale
			for (int x = 0; x < grayscale.cols; x++)
			{
				for (int y = 0; y < grayscale.rows; y++)
				{
					double r = grayscale.at<uchar>(y, x);



					double c = (double)255 / pow(255, gama);

					double s = c * pow(r, gama);


					transformada.at<uchar>(y, x) = s;


				}

			}


			//imshow("imagen", frame);
			imshow("grayscale", grayscale);
			imshow("transformada", transformada);
			waitKey(30);
		}
		return 0;
	}
}
