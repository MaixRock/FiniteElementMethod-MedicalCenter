#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Math.h"
#include "GauusMethod.h"

using namespace std;

void ReadFile(Element* & elmnt,double** &nodes, int &nNodes, int &nElement, string fileName) {

	ifstream in(fileName, ios::in);

	if (in.is_open()) {
		string line;

		getline(in, line);
		istringstream stream(line);

		stream >> nNodes >> nElement;

		nodes = new double* [nNodes];
		elmnt = new Element[nElement];

		double x1, x2, x3;
		for (int i = 0; i < nNodes; i++) {
			getline(in, line);
			istringstream stream(line);

			stream >> x1 >> x2 >> x3;
			
			nodes[i] = new double[3];
			nodes[i][0] = x1;
			nodes[i][1] = x2;
			nodes[i][2] = x3;
		} getline(in, line);

		int a1, a2, a3, a4;
		for (int i = 0; i < nElement; i++) {
			getline(in, line);
			istringstream stream(line);

			elmnt[i].nodes = new double*[4];

			stream >> a1 >> a2 >> a3 >> a4;

			elmnt[i].nodes[0] = nodes[a1];
			elmnt[i].nodes[1] = nodes[a2];
			elmnt[i].nodes[2] = nodes[a3];
			elmnt[i].nodes[3] = nodes[a4];
		}
	}

	in.close();
}

// 1) Задать 15 функций (15 слогаемых)
// 2) Проверка по файлу (EXCEL)
// 3) Создать массив [элемент -> узел]
// 4) Проссумировать слогаемые 

int main() {
	setlocale(LC_ALL, "ru");

	string fileName;
 
	double* test;

	double***** B;
	double***** K;

	Element* elmnt;

	double** nodes;
	double** S;

	int nNodes;
	int nElement;
	int indexElement;

	fileName = "input1.txt";
	ReadFile(elmnt, nodes, nNodes, nElement, fileName);

	//Вывод узлов для каждого элемента
	for (int i = 0; i < nElement; i++) {
		cout << "Element - " << i << endl;
		for (int j = 0; j < nodesInElement; j++) {
			for (int l = 0; l < numberCoordinates; l++) {
				cout << elmnt[i].nodes[j][l] << " ";
			}cout << endl;
		}cout << endl;
	}


	B = new double**** [2];
	K = new double**** [2];
	
	///////////////   
	///    1   ///
	////////////// 

	cout << "\nElement 1:\n" << endl;

	// B - Матрица производных
	B[0] = new double*** [nodesInElement];
	for (int i = 0; i < nodesInElement; i++) {

		B[0][i] = new double** [numberCoordinates];
		for (int j = 0; j < numberCoordinates; j++) {
			B[0][i][j] = dN(elmnt[0].nodes, i, j);
		}
	}

	//S - Матрица (...)
	S = CreateS(0);

	//K - матрица жёсткости
	K[0] = K_Loc(B[0], S);
	
	//Вывод локальной матрицы жесткости
	for (int I = 0; I < nodesInElement; I++) {
		for (int i = 0; i < numberCoordinates; i++) {
			for (int J = 0; J < nodesInElement; J++) {
				for (int j = 0; j < numberCoordinates; j++) {
					cout << K[0][I][J][i][j] << " ";
				}
			}
			cout << endl;
		}
	}

	///////////////   
	///    2   ///
	////////////// 

	cout << "\nElement 2:\n" << endl;

	// B - Матрица производных
	B[1] = new double*** [nodesInElement];
	for (int i = 0; i < nodesInElement; i++) {

		B[1][i] = new double** [numberCoordinates];
		for (int j = 0; j < numberCoordinates; j++) {
			B[1][i][j] = dN(elmnt[1].nodes, i, j);
		}
	}

	//S - Матрица (...)
	S = CreateS(1000);

	//K - матрица жёсткости
	K[1] = K_Loc(B[1], S);

	//Вывод локальной матрицы жесткости
	for (int I = 0; I < nodesInElement; I++) {
		for (int i = 0; i < numberCoordinates; i++) {
			for (int J = 0; J < nodesInElement; J++) {
				for (int j = 0; j < numberCoordinates; j++) {
					cout << K[1][I][J][i][j] << " ";
				}
			}
			cout << endl;
		}
	}

	///////////////   
	///    sum  ///
	////////////// 

	cout << "\nK :\n" << endl;

	double**** KK = new double*** [nNodes];
	for (int i = 0; i < nNodes; i++) {
		KK[i] = new double** [nNodes];
		for (int j = 0; j < nNodes; j++) {
			KK[i][j] = new double* [numberCoordinates];
			for (int l = 0; l < numberCoordinates; l++) {
				KK[i][j][l] = new double[numberCoordinates];
			}
		}
	}

	double** KM = new double* [nNodes*numberCoordinates];
	for (int i = 0; i < nNodes*numberCoordinates; i++) {
		KM[i] = new double [nNodes*numberCoordinates+1];
		KM[i][nNodes * numberCoordinates] = i;
	}
	
	for (int i = 0; i < nNodes * numberCoordinates; i++) {

	}

	for (int I = 0; I < nNodes; I++) {
		for (int J = 0; J < nNodes; J++) {
			for (int i = 0; i < numberCoordinates; i++) {
				for (int j = 0; j < numberCoordinates; j++) {
					KK[I][J][i][j] = 0;

					for (int index = 0; index < nElement;index++) {
						if (checkNode(elmnt[index], nodes[I]) && checkNode(elmnt[index], nodes[J])) {
							int I1 = getIndex(elmnt[index], nodes[I]);
							int J1 = getIndex(elmnt[index], nodes[J]);

							KK[I][J][i][j] += K[index][I1][J1][i][j];
						}
					}
				}
			}
		}
	}

	//Вывод локальной матрицы жесткости
	for (int I = 0; I < nNodes; I++) {
		for (int i = 0; i < numberCoordinates; i++) {
			for (int J = 0; J < nNodes; J++) {
				for (int j = 0; j < numberCoordinates; j++) {
					//cout << KK[I][J][i][j] << " ";

					KM[numberCoordinates*I+i][numberCoordinates*J+j] = KK[I][J][i][j];

					cout << numberCoordinates * I + i << " " << numberCoordinates * J + j <<" ";
				}
			}
			cout << endl;
		}
	}
	
	cout << endl;
	for (int I = 0; I < nNodes*numberCoordinates; I++) {
		for (int J = 0; J < nNodes*numberCoordinates; J++) {
			cout << KM[I][J] <<" ";
		}
		cout << endl;
	}

	double* X = new double[15];
	Gauss(KM, 15, X);

	cout << "\n" << "Решение системы" << endl;
	for (int I = 0; I < nNodes * numberCoordinates; I++) {
		cout << X[I] << " ";
	}

	return 0;
}