#include "GauusMethod.h"
#include <math.h>
#include <iostream>

using namespace std;

//Генерация матрицы по заданному варианту
void Generation(double** matrix, int size, int Var)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i][j] = 0;
			if (i == j) {
				matrix[i][j] = (Var+i) / 100.0;
			}
			if (abs(j - i) == 1) {
				matrix[i][j] = 1.37;
			}
			if (abs(j - i) == 2) {
				matrix[i][j] = 2.6;
			}
			if (abs(j - i) == 3) {
				matrix[i][j] = 3.15;
			}
			
		}
	}
	for (int i = 0; i < size; i++) {
		matrix[i][size] = i + 1;
	}
	for (int i = 0; i < size; i++) {
		for (int j = size + 1; j < size + 1 + size; j++) {
			if ((j - 1 - size) == i) {
				matrix[i][j] = 1;
			}
			else matrix[i][j] = 0;
		}
	}
}
//В случае нуля на диагонале меняем строки местами по заданным номерам строк
void SwapLine(double** matrix, int a, int b) {
	double* part;

	part = matrix[a];
	matrix[a] = matrix[b];
	matrix[b] = part;

}
double* UpTriangl(double** matrix, int LineCount) {

	double* detArr = new double[LineCount + 1];
	detArr[LineCount] = 1;
	for (int i = 0; i < LineCount; i++) {
		double k = matrix[i][i];
		
		if ((k == 0)&&(i+1 != LineCount)) {
			SwapLine(matrix, i, i+1);
			detArr[LineCount] *= -1;
			k = matrix[i][i];
		}
		detArr[i] = k;

		for (int j = i; j < LineCount+1+ LineCount; j++) {
			matrix[i][j] /= k;
		}

		for (int a = i+1; a < LineCount; a++) {
			k = matrix[a][i];
			for (int b = i; b < LineCount+1+LineCount; b++) {
				matrix[a][b] -= k * matrix[i][b];
			}
		}
	}

	return detArr;
}
double det(double* detArr, int Size) {
	double Res = 1;;

	for (int i = 0; i <= Size; i++) {
		Res *= detArr[i];
	}

	delete detArr;
	return Res;
}
double* Answer(double** matrix, int size) {
	double* answer = new double[size];

	for (int i = (size - 1); i >= 0; i--)
	{
		answer[i] = matrix[i][size];
		for (int j = i+1; j < size; j++) {
			answer[i] -= matrix[i][j]*answer[j];
		}
	}
	return answer;
}
void DownTriangl(double** matrix, int LineCount) {

	for (int i = LineCount - 1; i >= 0; i--)
	{
		double k;

		for (int a = i; a > 0; a--) {
			k = matrix[a-1][i];
			for (int b = i; b < LineCount+1+LineCount; b++) {
				matrix[a-1][b] -= k * matrix[i][b];
			}
		}
	}
}
double ConditionNumber(double** matrix, int size, bool change) {
	double Answer = 0;
	double MAX = 0;

	if (change)
	{
		for (int i = 0; i < size; i++) {
			double SUM = 0;
			for (int j = 0; j < size; j++) {
				SUM += abs(matrix[i][j]);
			}
			if (SUM > MAX) MAX = SUM;
		}
		Answer = MAX;
	}
	else {
		for (int i = 0; i < size; i++) {
			double SUM = 0;
			for (int j = size + 1; j < size + 1 + size; j++) {
				SUM += abs(matrix[i][j]);
			}
			if (SUM > MAX) MAX = SUM;
		}
		Answer = MAX;
	}

	

	return Answer;
}



void Gauss(double** matrix, int Size, double* X, double &Det, double** invers_matrix, double Condition) {
	matrix = new double* [Size];
	for (int i = 0; i < Size; i++) {
		matrix[i] = new double[(Size + 1 + Size)];
	}

	double CN=1;
	Generation(matrix, Size, 15);
	CN *= ConditionNumber(matrix, Size, true);
	double D = det(UpTriangl(matrix, Size), Size);
	double* ResArr = Answer(matrix, Size);
	DownTriangl(matrix, Size);
	CN *= ConditionNumber(matrix, Size, false);
}
void Gauss(double** matrix, int Size, double* X, double& det) {
}
void Gauss(double** matrix, int Size, double* X, double** invers_matrix) {
}
void Gauss(double** matrix, int Size, double* X) {

	for (int i = 0; i < Size; i++) {
		double k = matrix[i][i];

		if ((k == 0) && (i + 1 != Size)) {
			SwapLine(matrix, i, i + 1);
			k = matrix[i][i];
		}

		for (int j = i; j < Size + 1; j++) {
			matrix[i][j] /= k;
		}

		for (int a = i + 1; a < Size; a++) {
			k = matrix[a][i];
			for (int b = i; b < Size + 1; b++) {
				matrix[a][b] -= k * matrix[i][b];
			}
		}
	}

	for (int i = (Size - 1); i >= 0; i--)
	{
		X[i] = matrix[i][Size];
		for (int j = i + 1; j < Size; j++) {
			X[i] -= matrix[i][j] * X[j];
		}
	}

}