#pragma once
#include <stdint.h>
#include <vector>
#include <cmath>

using namespace std;

// ----------------------------- Определитель --------------------------
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

//Коэффициенты через координаты узлов
double a(vector<vector<double>> &t, int index) {
    double result = 0;
    uint32_t size = 3;

    double** matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    }

    for (int i = 0; i < size; i++) {

        int k = 0;

        for (int j = 0; j < size; j++) {
            if (i >= index) k = 1;

            matrix[i][j] = t[i+k][j];
        }
    }

    result = det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double b(vector<vector<double>> &t, int index) {
    double result = 0;
    uint32_t size = 3;

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

            matrix[i][j] = t[i + k][j + l-1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double c(vector<vector<double>> &t, int index) {
    double result = 0;
    uint32_t size = 3;

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

            matrix[i][j] = t[i + k][j + l-1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}
double d(vector<vector<double>> &t, int index) {
    double result = 0;
    uint32_t size = 3;

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

            matrix[i][j] = t[i + k][j + l - 1];
        }
    }

    result = -det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}

//Объём тетраэдального элемента
double V(vector<vector<double>> &t) {
    double result = 0;
    uint32_t size = 4;

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

            matrix[i][j] = t[i][j - 1];
        }
    }

    result = (1.0 / 6.0) * det(matrix, size);

    for (int i = 0; i < size; i++) {
        delete matrix[i];
    }
    delete matrix;

    return result;
}

//Функция формы
double N(vector<vector<double>> &t, int i) {
    return (a(t, i) + b(t, i) * t[i][0] + c(t, i) * t[i][1] + d(t, i) * t[i][2])/6*V(t);
}

//Матрицы производных функции формы
//(Принимает узлы элемента)
vector<vector<double>> dN_Node(vector<vector<double>> &t) {
    unsigned int column = 3;
    unsigned int row = 12;

    //double** matrix = new double* [row];
    //for (int i = 0; i < row; i++) {
    //    matrix[i] = new double[column];
    //}

    //matrix[0][0] = v * b(t);

    double v = (1.0 / (6.0 * V(t)));

    vector<vector<double>> matrix;
    vector<double> x1, x2, x3;

    x1.resize(9);
    x2.resize(9);
    x3.resize(9);

    for (int i = 0; i < 4; i++) {
        x1[0] = v * b(t, i); x1[3] = v * c(t, i); x1[8] = v * d(t, i);
        x2[1] = v * c(t, i); x2[4] = v * b(t, i); x2[5] = v * d(t, i);
        x3[2] = v * d(t, i); x3[6] = v * c(t, i); x3[7] = v * b(t, i);

        matrix.push_back(x1);
        matrix.push_back(x2);
        matrix.push_back(x3);
    }

    return matrix;
}

//Произведение матриц 
vector<vector<double>> product(vector<vector<double>> &A, vector<vector<double>> &B) {
    vector<vector<double>> result;

    result.resize(A.size());
    for (int i = 0; i < A.size(); i++) {
        result[i].resize(B[0].size());
    }

    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B[0].size(); j++) {
            double res = 0;
            for (int k = 0; k < A[0].size(); k++) {
                //cout << A[i][k] << " * " << B[k][j] << " + ";
                res += A[i][k] * B[k][j];
            }
            result[i][j] = res;
            //cout << " = " << res << endl;
        }
    }

    return result;
}

//Произведение матриц 
vector<vector<double>> spec_product(vector<vector<double>>& B, vector<vector<double>>& S) {
    vector<vector<double>> result;

    result.resize(B.size());
    for (int i = 0; i < B.size(); i++) {
        result[i].resize(B.size());
    }


    return result;
}

//Функция транспонирования
vector<vector<double>> T(vector<vector<double>> &A) {
    vector<vector<double>> result;

    result.resize(A[0].size());
    for (int i = 0; i < A[0].size(); i++) {
        result[i].resize(A.size());
    }

    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[0].size(); j++) {
            result[i][j] = A[j][i];
        }
    }

    return result;
}
