#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc.hpp"
#include <stdlib.h>
#include <iostream>
using namespace cv;



int main(int, char**)
{	
		namedWindow("grayscale", CV_WINDOW_NORMAL);
		namedWindow("Suavisado", CV_WINDOW_NORMAL);
		//namedWindow("Ecualizada", CV_WINDOW_NORMAL);
		namedWindow("MascaraBorde", CV_WINDOW_NORMAL);
		
		
		Mat dst;
			
		Mat grayscale = cv::imread("07a.jpg", IMREAD_GRAYSCALE);
		imshow("grayscale", grayscale);

		Mat blur = Mat(grayscale.rows, grayscale.cols, CV_8U);
		GaussianBlur(grayscale,blur, Size(3, 3),1.0);
		imshow("Suavisado", blur);

		/*Mat ecualizada = Mat(grayscale.rows, grayscale.cols, CV_8U);
		equalizeHist(blur, ecualizada);
		imshow("Ecualizada", ecualizada);*/


		Mat borde = Mat(grayscale.rows, grayscale.cols, CV_8U);
		Canny(blur, borde, 50, 150);
		imshow("MascaraBorde", borde);


		waitKey(0);
		return 0;
}