#pragma once

void Gauss(double** matrix, int Size, double* X);

void Generation(double**,int, int);
void SwapLine(double**, int, int);
double* UpTriangl(double**, int);
double det(double* ,int);
double* Answer(double**, int);
void DownTriangl(double**, int);
double ConditionNumber(double** , int, bool);