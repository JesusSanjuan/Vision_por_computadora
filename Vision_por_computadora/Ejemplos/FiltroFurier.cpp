#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>

#define M_PI 3.14159265358979323846

using namespace cv;
using namespace std;

int main(int, char**)
{

	unsigned t0, t1;
	Mat modificada = cv::imread("image0.jpg", IMREAD_GRAYSCALE);
	t0 = clock();		
	Mat original = modificada.clone();
	modificada.convertTo(modificada, CV_64F);
	

	int M = original.cols;//width
	int N = original.rows;//high
	printf("Dimensiones de imagen width: %d  y high: %d \n", M, N);


	printf("INICIO Paso 1 de 6. Elevacion \n");
	for (int x = 0; x < M; x++)
	{
		for (int y = 0; y < N; y++)
		{
			modificada.at<double>(y, x) = modificada.at<double>(y, x)*pow(-1.0, (double)(x +y));
		}
	}
	printf("\tFINALIZA Paso 1 de 6. Elevacion \n");
	
	printf("INICIO Paso 2 de 6. Transformada de Furier\n");
	//modificada.convertTo(modificada2, CV_64F);
	// Mat modificada2= modificada.clone();
	Mat DFT = modificada.clone();
	Mat DFTImageRE = modificada.clone();
	Mat DFTImageIM = modificada.clone();
	Mat DFTInverse = Mat(modificada.rows, modificada.cols, CV_64F);
	Mat salida = Mat(modificada.rows, modificada.cols, CV_64F);
	Mat salida2 = Mat(modificada.rows, modificada.cols, CV_64F);
	//Paso 2: Aplicamos la transformada

	for (int x = 0; x < M; x++)
	{
		for (int y = 0; y < N; y++)
		{
			double ak = 0.0;
			double bk = 0.0;
			for (int j = 0; j < M; j++)
			{
				for (int k = 0; k < N; k++)
				{

					double divM = double(x*j) / double(M);
					double divN = double(y*k) / double(N);

					double equiz = 2.0 * M_PI * divM;
					double ye = 2.0 * M_PI * divN;

					ak += modificada.at<double>(k, j)*cos(equiz + ye);
					bk += modificada.at<double>(k, j)*(-1.0)*sin(equiz + ye);

				}
			}

			double ResX = 0.0, ResY = 0.0;
			double div = double(1.0) / double(M*N);
			ResX = double(ak)*div;
			ResY = double(bk)*div;//(1/M*N)

			DFTImageRE.at<double>(y, x) = ResX;
			DFTImageIM.at<double>(y, x) = ResY;

			double cuadrados = pow(ResX, 2.0) + pow(ResY, 2.0);
			DFT.at<double>(y, x) = sqrt(cuadrados);

		}
	}
	printf("\tFINALIZA Paso 2 de 6.  Transformada de Furier\n");
	printf("INICIO Paso 3 de 6. Creacion de mascara cilindrica terminada\n");
	/*
//Creacion de mascara cilindrica
	
	Mat mascaraCilindrica = Mat(N, M, CV_64F);
	int DUV;
	int D0 = 50;
	for (int u = 0; u < M; u++)
	{
		for (int v = 0; v < N; v++)
		{
			DUV = sqrt(pow(u - (M / 2), 2) + pow(v - (N / 2), 2));
			if (DUV <= D0)
			{
				mascaraCilindrica.at<double>(v, u) = 1;
			}
			else
			{
				mascaraCilindrica.at<double>(v, u) = 0;
			}
		}
	}*/
	//Creacion de mascara gausiana
	Mat mascaraCilindrica = Mat(N, M, CV_64F);
	int DUV;
	int D0 = 50;
	for (int u = 0; u < M; u++)
	{
		for (int v = 0; v < N; v++)
		{
			DUV = sqrt(pow(u - (M / 2), 2) + pow(v - (N / 2), 2));			
		    mascaraCilindrica.at<double>(v, u) = exp((-(pow(DUV,2)))/(2*pow(D0,2)));
			
		}
	}
	printf("\tFINALIZA Paso 3 de 6. Creacion de mascara cilindrica terminada\n");
	printf("INICIA Paso 4 de 6. Aplicacion de mascara terminada\n");
	//Aplicacion de mascara
    for (int x = 0; x < M; x++)
	{
		for (int y = 0; y < N; y++)
		{
			double intensidad = DFTImageRE.at<double>(y, x);
			double intensidad2 = mascaraCilindrica.at<double>(y, x);
			double intensidad3 = DFTImageIM.at<double>(y, x);
			double valor1 = intensidad  * intensidad2;
			double valor2 = intensidad3 * intensidad2;
			DFTImageRE.at<double>(y, x) = valor1;
			DFTImageIM.at<double>(y, x) = valor2;
		}
	}
	printf("\tFINALIZA Paso 4 de 6. Aplicacion de mascara terminada\n");
	printf("INICIA Paso 5 de 6. Inversa de transformada de furier correcta\n");
	//Paso 3: Aplicamos la inversa
	for (int x = 0; x < M; x++)
	{
		for (int y = 0; y < N; y++)
		{
			double akR = 0;
			double bkR = 0;

			double akI = 0;
			double bkI = 0;
			for (int j = 0; j < M; j++)
			{
				for (int k = 0; k < N; k++)
				{

					double divM = double(x*j) / double(M);
					double divN = double(y*k) / double(N);

					double equiz = 2.0*M_PI * divM;
					double ye = 2.0*M_PI * divN;


					akR += DFTImageRE.at<double>(k, j)*cos(equiz + ye);
					//bkR += DFTImageRE.at<double>(k, j)*(1.0)*sin(equiz + ye);

					//akI += DFTImageIM.at<double>(k, j)*cos(equiz + ye);
					bkI += DFTImageIM.at<double>(k, j)*(1.0)*sin(equiz + ye);
				}
			}
			double sumReal = akR - bkI;
			//double sumImag = bkR+akI;
			//DFTInverse.at<double>(y, x) = abs(sumReal) + abs(sumImag);
			DFTInverse.at<double>(y, x) = (sumReal);
			//DFTInverse.at<double>(N-y-1, M-x-1) = abs(sumReal) + abs(sumImag);
		}
	}
	printf("\tFINALIZA Paso 5 de 6. Inversa de transformada de furier correcta\n");
	printf("INICIA Paso 6 de 6. Paso final elevacion\n");
	//DFTInverse.convertTo(salida, CV_8U);

	for (int x = 0; x < DFTInverse.cols; x++)
	{
		for (int y = 0; y < DFTInverse.rows; y++)
		{
			salida.at<double>(y, x) = DFTInverse.at<double>(y, x)*pow(-1.0, (double)x + y);
		}
	}
	salida.convertTo(salida, CV_8U);
	printf("\tFINALIZA Paso 6 de 6. Paso final elevacion\n");
	

	imshow("Original", original);
	imshow("Modificada", modificada);
	imshow("Mascara", mascaraCilindrica);
	imshow("DFT", DFT);
	imshow("DFTImageRE", DFTImageRE);
	imshow("DFTImageIM", DFTImageIM);
	imshow("DFTinvertida", DFTInverse);
	imshow("OriginalProce", salida);
	
	t1 = clock();

	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	cout << "Tiempo de ejecucion: " << time << endl;

	waitKey(0);
	
	return 0;

}
