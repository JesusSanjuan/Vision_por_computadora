#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;


	namedWindow("imagen", 1);
	//namedWindow("grayscale", 1);
	namedWindow("Superpos", 1);

	/*Lectura de imagen*/
	Mat img = imread("image.jpg");
	namedWindow("image", 1);
	imshow("image", img);

	Mat frame;
	cap >> frame; //get a new frame from camera


	//Mat grayscale = Mat(frame.rows, frame.cols, CV_8U);
	Mat Super = Mat(frame.rows, frame.cols, CV_8U);
	std::cout << "Resolucion de camara Rows: " << frame.rows << "  Cols: "<< frame.cols <<std::endl;

	std::cout << "Resolucion de imagen Rows " << img.rows << "  Cols: " << img.cols << std::endl;
	for (;;) 
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		for (int x = 0; x < frame.cols; x++)
		{
			for (int y = 0; y < frame.rows; y++)
			{
				Vec3b punto = frame.at<Vec3b>(y, x);
				Vec3b punto2 = img.at<Vec3b>(y, x);
				int  p2 = punto[2];
				int  p1 = punto[1];
				int  p0 = punto[0];
				//std::cout << "Rojo: " << p2 << "  Verde: " << p1 <<" Azul:"<< p0<< std::endl;
				if (punto[1]==0 && punto[2]>=200 && punto[0]==0)
				{
					std::cout << "Verde"<<std::endl;
					//Super.at<Vec3b>(y, x) = punto2;
				}
				//Super.at<Vec3b>(y, x) = punto;
			
				/*float r = punto[2];
				float g = punto[1];
				float b = punto[0];*/

				//grayscale.at<uchar>(y, x) = r * 0.2126 + g * 0.7152 + b * 0.0722;
			}
		}


		imshow("imagen", frame);
		imshow("Superpos", Super);
		//imshow("grayscale", grayscale);
		waitKey(30);
	}
	return 0;

}

