#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
#include <math.h>
using namespace cv;


Mat Contorno(Mat Imagen, int Mascara[3][3],int rows, int cols)
{
	Mat resultado= Mat(rows, cols, CV_8U);
	int tem1, tem2, resultado1, resultado2;


	for (int x = 1; x < Imagen.cols - 1; x++) // Control de ventana
	{
		for (int y = 1; y < Imagen.rows - 1; y++) // Control de ventana
		{
			int sumatoria = 0;
			tem1 = 0; tem2 = 0;
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				for (int yy = y - 1; yy <= y + 1; yy++)
				{
					int intensidad = Imagen.at<uchar>(yy, xx);
					resultado1 = intensidad * Mascara[tem1][tem2];
					sumatoria = sumatoria + resultado1;
					tem1++;
					//std::cout << "Intensidades " << intensidad  << std::endl;						
				}tem1 = 0; tem2++;
			}
			resultado.at<uchar>(y, x) = abs((sumatoria));
		}
	}

	return resultado;
}

Mat ProceFinal(Mat Imagen, Mat Imagen2, int rows, int cols)
{
	Mat resultado = Mat(rows, cols, CV_8U);
	int resu=0, tem=0, tem2=0;
	for (int x = 1; x < Imagen.cols - 1; x++) // Control de ventana
	{
		for (int y = 1; y < Imagen.rows - 1; y++) // Control de ventana
		{
			int intensidad  = Imagen.at<uchar>(y, x);
			int intensidad2 = Imagen2.at<uchar>(y, x);
			tem = pow(intensidad, 2); tem2 = pow(intensidad2, 2);
			resu = tem + tem2;
			resu = sqrt(resu);
			//resultado.at<uchar>(y, x) = (resu);
			resultado.at<uchar>(y, x) = abs((resu));
		}
	}
	return resultado;
}

int main(int, char**)
{
	VideoCapture cap(0); //Abrimos la camara default
	if (!cap.isOpened()) //checamos si fue exitoso
		return -1;

	namedWindow("Grises", 1);
	namedWindow("MascaraBorde", 1);
	
	Mat frame;
	cap >> frame; //get a new frame from camera
	
	Mat gris = Mat(frame.rows, frame.cols, CV_8U);
	Mat borde1 = Mat(frame.rows, frame.cols, CV_8U);
	Mat borde2 = Mat(frame.rows, frame.cols, CV_8U);
	Mat bordeF = Mat(frame.rows, frame.cols, CV_8U);

	const int tamaño = 3;
	int Mascara[tamaño][tamaño] = { { -1,  -1,  -1},
									{  0,  0,  0}, /* los espacios y saltos de líneas no son tomados en cuenta */
									{  1,  1,  1} };
	int Mascara2[tamaño][tamaño] = { { 1,  0,  -1},
									{ 1,  0,  -1}, /* los espacios y saltos de líneas no son tomados en cuenta */
									{ 1,  0,  -1} };
	

	
	for (;;)
	{	
		Mat frame;
		cap >> frame;
		cvtColor(frame, gris, CV_RGB2GRAY);
		
		borde1 = Contorno(gris, Mascara, frame.rows, frame.cols);
		borde2 = Contorno(gris, Mascara2, frame.rows, frame.cols);
		bordeF = ProceFinal(borde1, borde2, frame.rows, frame.cols);
				
		imshow("Grises", gris);
		imshow("MascaraBorde", bordeF);
		waitKey(5);
	}
	return 0;

}

