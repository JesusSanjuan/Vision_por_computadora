#include<opencv2/opencv.hpp>

using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

	namedWindow("red", 1);
	namedWindow("green", 1);
	namedWindow("blue", 1);

	Mat frame;

	cap >> frame;

	Mat r = Mat(frame.rows, frame.cols, CV_8U);
	Mat g = Mat(frame.rows, frame.cols, CV_8U);
	Mat b = Mat(frame.rows, frame.cols, CV_8U);

	for (;;)
	{
		Mat frame;
		cap >> frame;
		for (int x = 0; x < frame.cols; x++)
		{
			for (int y = 0; y < frame.rows; y++)
			{
				Vec3b punto = frame.at<Vec3b>(y, x);
				r.at<uchar>(y, x) = punto[2];
				g.at<uchar>(y, x) = punto[1];
				b.at<uchar>(y, x) = punto[0];
			}
		}

		imshow("red", r);
		imshow("green", g);
		imshow("blue", b);
		waitKey(60);
	}
	return 0;
}
