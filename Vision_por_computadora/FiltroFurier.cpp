#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
#include <math.h>

using namespace cv;

int main(int, char**)
{

	namedWindow("Grises", 1);
	namedWindow("Mascara", 1);


	Mat color = imread("image2.jpg");// CV_LOAD_GRAYSCALE
	Mat gris = Mat(color.rows, color.cols, CV_8U);
	Mat furier = Mat(color.rows, color.cols, CV_8U);
	cvtColor(color, gris, CV_RGB2GRAY);
	for (;;)
	{		

		for (int x = 0; x < gris.cols - 1; x++) // Control de ventana
		{
			for (int y = 0; y < gris.rows - 1; y++) // Control de ventana
			{
			
				double intensidad = gris.at<uchar>(y, x);
				furier.at<uchar>(y, x) = intensidad * pow((-1),(y+x));                                                                                                                                                                                                                                                                                                      
						
			}
		}
		imshow("Grises", gris);
		imshow("Mascara", furier);
		waitKey(30);
	}
	return 0;

}
