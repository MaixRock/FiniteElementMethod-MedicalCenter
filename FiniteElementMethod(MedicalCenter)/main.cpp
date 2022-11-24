#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Math.h"
#include "GauusMethod.h"

using namespace std;

void ReadFile(double** &nodes, int &size, string fileName) {

	ifstream in(fileName, ios::in);

	if (in.is_open()) {
		string line;

		getline(in, line);
		istringstream stream(line);

		nodes = new double* [size];
		
		for (int i = 0; i < size; i++) {
			getline(in, line);
			istringstream stream(line);

			double x1, x2, x3;
			stream >> x1 >> x2 >> x3;
			
			nodes[i] = new double[3];
			nodes[i][0] = x1;
			nodes[i][1] = x2;
			nodes[i][2] = x3;
		}
	}

	in.close();
}

//int mainOld() {
//	setlocale(LC_ALL, "ru");
//
//	string filename = "input.txt";
//	vector<vector<double>> nodes;
//	
//	ReadFile(nodes, filename);
//	
//	cout <<"Кол-во узлов: " << nodes.size()<<"\n\n";
//
//	for (int i = 0; i < 4; i++) {
//		cout << "xyz: ";
//		for (int j = 0; j < 3; j++) {
//			cout << nodes[i][j] << " ";
//		}
//		cout << endl;
//	}
//
//	cout << endl;
//	cout.precision(3);
//	cout.setf(ios::fixed);
//
//	for (int i = 0; i < nodes.size();i++) {
//		cout << "N("<<i<<") = " << N(nodes, i) << endl;
//	}
//	cout << endl;
//	cout << "V = " << V(nodes) << endl;
//
//	cout << "\n Коиффиеценты:\n";
//	for (int i = 0; i < 4; i++) {
//		cout << a(nodes, i) <<" "<< b(nodes, i) << " " << c(nodes, i) << " " << d(nodes, i) << endl;
//	} cout << endl;
//
//	cout << "\n Матрица 12х9:\n";
//	vector<vector<double>> test = dN_Node(nodes);
//	for (int i = 0; i < test.size(); i++) {
//		for (int j = 0; j < test[i].size(); j++) {
//			cout << test[i][j] << " ";
//		}
//		cout << endl;
//	}cout << endl;
//
//	vector<vector<double>> testT = T(test);
//
//	vector<vector<double>> S;
//	S.resize(9); 
//	cout << "\ S:\n"; 
//	double dop = 0;
//	for (int i = 0; i < S.size(); i++) {
//		S[i].resize(1);
//		for (int j = 0; j < S[i].size(); j++) {
//			dop += 100;
//			S[i][j] = dop;
//			cout << S[i][j] << " ";
//		}
//		cout << endl;
//	}cout << endl;
//
//	cout << "\ Транспонированная матрциа:\n";
//	for (int i = 0; i < testT.size(); i++) {
//		for (int j = 0; j < testT[i].size(); j++) {
//			cout << testT[i][j] << " ";
//		}
//		cout << endl;
//	}cout << endl;
//
//	vector<vector<double>> prodTest = product(test, testT);
//	cout << "\n Произведение (Матрица 12х12):\n";
//	for (int i = 0; i < prodTest.size(); i++) {
//		for (int j = 0; j < prodTest[i].size(); j++) {
//			cout << prodTest[i][j] << " ";
//		}
//		cout << endl;
//	}cout << endl;
//
//	return 0;
//}


// 1) Задать 15 функций (15 слогаемых)
// 2) Проверка по файлу (EXCEL)
// 3) Создать массив [элемент -> узел]
// 4) Проссумировать слогаемые 

int main() {
	setlocale(LC_ALL, "ru");

	string fileName;
 
	double* test;

	double**** B;
	double**** K;

	int** elements;
	
	double** nodes;
	double** S;

	int sizeNodes;

	fileName = "input.txt";
	ReadFile(nodes, sizeNodes, fileName);

	// B - Матрица производных
	B = new double*** [nodesInElement];
	for (int i = 0; i < nodesInElement; i++) {

		B[i] = new double** [numberCoordinates];
		for (int j = 0; j < numberCoordinates; j++) {
			B[i][j] = dN(nodes, i, j);
		}
	}

	//S - Матрица (...)
	S = CreateS();

	//K - матрица жёсткости
	K = K_Loc(B, S);
	
	for (int I = 0; I < nodesInElement; I++) {
		for (int i = 0; i < numberCoordinates; i++) {
			for (int J = 0; J < nodesInElement; J++) {
				for (int j = 0; j < numberCoordinates; j++) {
					cout << K[I][J][i][j] << " ";
				}
			}
			cout << endl;
		}
	}

	test = &K[0][0][0][2];

	cout << "test = " << *test << endl;

	int k=0;
	cin >> k;

	return 0;
}