#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>

#define M_PI 3.14159265358979323846

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
	unsigned t0, t1;
	Mat I = cv::imread("lena.jpg", IMREAD_GRAYSCALE);
	t0 = clock();
	if (I.empty()) {
		cout << "Error opening image" << endl;
		return -1;
	}

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	dft(complexI, complexI);            // this way the result may fit in the source matrix
	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;
	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
	normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
											// viewable image form (float between values 0 and 1).
	cv::Mat inverseTransform;	
/*	cv::dft(complexI, inverseTransform,  cv::DFT_REAL_OUTPUT | cv::DFT_INVERSE );
	//inverseTransform.convertTo(inverseTransform, CV_8U);
	normalize(inverseTransform, inverseTransform, 0, 1, NORM_MINMAX);
/*	Mat inverseTransform;
	dft(complexI, inverseTransform, DFT_INVERSE + DFT_REAL_OUTPUT);
	inverseTransform.convertTo(inverseTransform, CV_8U);*/

	//Mat inverseTransform2 = idft(complexI, inverseTransform,0,0);

	//Mat inverseTransform_removenoise;
	dft(complexI, inverseTransform, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
	Mat final(inverseTransform, Rect(0, 0, I.cols, I.rows));
	

	imshow("Input Image", I);    // Show the result
	imshow("spectrum magnitude", magI);
	imshow("FINAL", final);
	//imshow("Reconstructed", inverseTransform);
	
	

	t1 = clock();

	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	cout << "Tiempo de ejecucion: " << time << endl;

	waitKey(0);
	
	return 0;

}
