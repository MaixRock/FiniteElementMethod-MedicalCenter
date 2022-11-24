#pragma once
#include <stdint.h>
#include <cmath>

const int nodesInElement = 4; //Число узлов для одного элемента
const int numberCoordinates = 3; //Число координат в узле

using namespace std;
// ----------------- Определитель ---------------- //
// Вычисляет определитель матрицы T размерностью N                               
double det(double** T, uint32_t N)
{
    double det__;
    int sub_j, s;
    double** subT;    // Субматрица как набор ссылок на исходную матрицу
    switch (N)
    {
    case 1:
        return T[0][0];
    case 2:
        return T[0][0] * T[1][1] - T[0][1] * T[1][0];
    default:
        if (N < 1) return nan("1");  // Некорректная матрица
        subT = new double* [N - 1];  // Массив ссылок на столбцы субматрицы
        det__ = 0;
        s = 1;        // Знак минора
        for (int i = 0; i < N; i++)  // Разложение по первому столбцу
        {
            sub_j = 0;
            for (int j = 0; j < N; j++)// Заполнение субматрицы ссылками на исходные столбцы
                if (i != j)      // исключить i строку
                    subT[sub_j++] = T[j] + 1;  // здесь + 1 исключает первый столбец

            det__ = det__ + s * T[i][0] * det(subT, N - 1);
            s = -s;
        };
        delete[] subT;
        return det__;
    };
};

// ----------------- Коэффициенты ---------------- //
// Определяются через координаты узлов
double a(double** nodes, int index) {
    double result = 0;
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int i = 0; i < size; i++) {

        int k = 0;

        for (int j = 0; j < size; j++) {
            if (i >= index) k = 1;

            matrix[i][j] = nodes[i + k][j];
        }
    }

    result = det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double b(double** nodes, int index) {
    double result = 0;
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }


    for (int i = 0; i < size; i++) {

        int k = 0;
        int l = 0;

        for (int j = 0; j < size; j++) {
            if (j == 0) {
                matrix[i][j] = 1;
                continue;
            }
            if (i >= index) k = 1;
            if (j >= 1) l = 1;

            matrix[i][j] = nodes[i + k][j + l - 1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double c(double** nodes, int index) {
    double result = 0;
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int i = 0; i < size; i++) {

        int k = 0;
        int l = 0;

        for (int j = 0; j < size; j++) {
            if (j == 0) {
                matrix[i][j] = 1;
                continue;
            }
            if (i >= index) k = 1;
            if (j >= 2) l = 1;

            matrix[i][j] = nodes[i + k][j + l - 1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double d(double** nodes, int index) {
    double result = 0;
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int i = 0; i < size; i++) {

        int k = 0;
        int l = 0;

        for (int j = 0; j < size; j++) {
            if (j == 0) {
                matrix[i][j] = 1;
                continue;
            }
            if (i >= index) k = 1;
            if (j >= 3) l = 1;

            matrix[i][j] = nodes[i + k][j + l - 1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}

// ----------------- Объем элемента ----------------//
//  Объем тетраэдального элемента
double V(double** nodes) {
    double result = 0;
    int size = nodesInElement;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j == 0) {
                matrix[i][j] = 1;
                continue;
            }

            matrix[i][j] = nodes[i][j - 1];
        }
    }

    result = (1.0 / 6.0) * det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}

// ----------------- Функция формы ---------------- //
double N(double** t, int i) {
    return (a(t, i) + b(t, i) * t[i][0] + c(t, i) * t[i][1] + d(t, i) * t[i][2]) / 6 * V(t);
}

// ------------ Производные функции формы --------- //
double** dN(double** nodes, int index, int crdinate) {
    double result = 0;
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    double v = (1.0 / (6.0 * V(nodes)));

    for (int i = 0; i < size; i++) {

        for (int j = 0; j < size; j++) {
            if (crdinate == i) {
                switch (j)
                {
                case 0: matrix[i][j] = v*b(nodes, index);
                        break;
                case 1: matrix[i][j] = v*c(nodes, index);
                        break;
                case 2: matrix[i][j] = v*d(nodes, index);
                        break;
                    default:
                        break;
                }
            }
            else {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}

//-------------- Матрица жёсткости ----------------//
double** K_ij(double**** B, int I, int J, double** S) {
    
    int size = numberCoordinates;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int R = 0; R < size;R++) {
        for (int Q = 0; Q < size; Q++) {
            
            double res = 0;
            for (int j = 0; j < size; j++) {
                for (int i = 0; i < size; i++) {
                    for (int l = 0; l < size; l++) {
                        res +=
                            B[I][R][j][i] *
                            B[J][Q][i][l] *
                            S[l][j];
                    }
                }
            }

            matrix[R][Q] = res;
        }
    }
    
    return matrix;
}
double**** K_Loc(double**** B, double** S) {
    
    int size = nodesInElement;

    double**** K = new double*** [size];
    for (int i = 0; i < size; i++) {
        K[i] = new double**[size];
        for (int j = 0; j < size - 1;j++) {
            K[i][j] = new double*[size - 1];
            for (int l = 0; l < size - 1; l++) {
                K[i][j][l] = new double[size - 1];
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            K[i][j] = K_ij(B, i, j, S);
        }
    }

    return K;
}

//---------------- Матрицы -----------------//
double** CreateS() {

    int size = numberCoordinates;

    double** S = new double* [size];
    for (int i = 0; i < size; i++) {
        S[i] = new double[size];
    }

    double res = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            res += 100;
            S[i][j] = res;
        }
    }

    return S;
}
