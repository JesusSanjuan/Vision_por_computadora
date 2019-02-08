#include "opencv2/opencv.hpp"
using namespace cv;
int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;



	Mat frame;
	cap >> frame; //get a new frame from camera



	std::cout << "Resolucion de camara Rows: " << frame.rows << "  Cols: " << frame.cols << std::endl;
	printf(" rows %i \n", frame.rows);
	printf(" cols %i \n", frame.cols);

	//resolucion 640  * 480

	Mat image;
	image = imread("image.jpg");   // Read the file

	printf(" rows imagen %i \n", image.rows);
	printf(" cols imagen %i \n", image.cols);



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


				//printf(" r  %f \n", r)
				//Verdes bajos [49,50,50] 
				//Verdes altos [80, 255, 255]

				if ((r >= 100 && r <= 255) && (g >= 100 && g <= 255) && (b >= 100 && b <= 255)) {

					frame.at<Vec3b>(y, x) = image.at<Vec3b>(y, x);
				}


			}

		namedWindow("imagen", WINDOW_AUTOSIZE);

		imshow("imagen", frame);
		waitKey(30);
	}
	return 0;

}