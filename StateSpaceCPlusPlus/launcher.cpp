/***********************************************************************
FILE LAUNCHER.CPP CONTAINS SIMPLE MODEL OF STATE SPACE

FILE UTMOST REVIEW DONE ON (26.12.2019) BY ARTUR K. 
***********************************************************************/

#include "pch.h"
#include <iostream>
#include <fstream>

#include "UHMath.h"
#include "Timer/CTimer.h"

using namespace UH;

typedef unsigned int uint32;

struct SysCharacteristics
{
	double			timeToMax;
	double			ymax;
	double			overshoot;
	double			yss;
	double			ess;
	double*			pYArr;
	uint32			arrSize;
};

// ****** TODO: Consider implementing ss(...) method.
//SysCharacteristics ss(const Matrix2x2& A, const ColumnVec2& B, const RowVec2& C, const double simulationTimeS, const double dt = 0.01);

int main()
{
    std::cout << "Welcome!\n"; 

	CTimer timer1;

	//State space model
	Matrix2x2 A(0, 1, -3, -2);
	ColumnVec2 B(0, 1);
	RowVec2 C(1, 0);
	//D matrix doesn't exist in this example D = 0;

	//Initial conditions
	ColumnVec2 X0(0, 0);

	//u is desired condition for 2 by 2 A matrix,
	//we have only a single controllable variable.
	double u = 1; 

	double initialTime = 0.0;                       
	double dt = 0.01; //Step size
	double simulationTimeS = 160.0; //Simulation time in seconds
	int n = round((simulationTimeS - initialTime) / dt); //Number of iterations

	//Allocate arrays
	ColumnVec2* pXArr = new ColumnVec2[n];	//X
	double* pYArr = new double[n];			//Y
	memset(pYArr, 0, sizeof(double) * n);

	//Set initial value
	pXArr[0] = X0;

	timer1.start();

	//Simulation of the response of the system for the period of simulationTimeS.
	for (auto i = 1; i < n; i++)
	{
		pXArr[i] = pXArr[i - 1] + (A * pXArr[i - 1] + B * u) * dt;
	}

	for (auto j = 0; j < n; j++)
	{
		//Y is the output
		pYArr[j] = C * pXArr[j];
	}

	auto timeElapsed = timer1.getElapsedTimeUs();

	//Control law
	//Matrix2x2 Q(1,0,0,2);
	//double R = 1;
	//auto K = lqr(A,B,Q,R);
	// ****** TODO: Implement lqr method.

	//Closed loop system then look like this
	//ss((A-B*K),B,C,D);

	std::ofstream outFile;
	outFile.open("outputs.txt");
	outFile << "Time(s) Column1 Column2 Y";
	for (auto i = 0; i < n; i++)
	{
		outFile << "\n" << i * dt << " " << pXArr[i][0] << " " << pXArr[i][1] << " " << pYArr[i];
	}
	outFile.close();

	//Clean-up after ourselves
	delete[] pXArr;
	delete[] pYArr;

	std::cout << "Done, see output.txt file.\n";
	std::cout << "Time elapsed: " << timeElapsed << "us\n";
	std::cin.get();
}
