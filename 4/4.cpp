#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

// Получает статическую матрицу размеров NxM и вжмыхивает
// её в динамическую
template <int N, int M>
double** staticToDynamic(double templ[N][M]) {
    double** matrix = new double*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new double[M];
        for (int j = 0; j < M; j++) {
            matrix[i][j] = templ[i][j];
        }
    }
    return matrix;
}


// Выводит матрицу по размерам
void printMatrix(double** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << setw(10) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// Выводит вектор по размерам
void printVector(double* vector, int n) {
    for (int i = 0; i < n; i++)
        cout << vector[i] << " ";
    cout << endl;
}


// Получает на вход число, по нему выбирает матрицу
// и возвращает её
double** getMatrix(int option) {
    if (option == 1) {
        double templ[3][4] = {
            {1.0, 0.8, 0.64, erf(0.8)},
            {1.0, 0.9, 0.81, erf(0.9)},
            {1.0, 1.1, 1.21, erf(1.1)}
        };
        return staticToDynamic<3, 4>(templ);
    }
    if (option == 2) {
        double templ[3][4] = {
            {0.1, 0.2, 0.3, 0.1},
            {0.4, 0.5, 0.6, 0.3},
            {0.7, 0.8, 0.9, 0.5}
        };
        return staticToDynamic<3, 4>(templ);
    }
    if (option == 3) {
        double templ[2][3] = {
            {1e-4, 1, 1},
            {   1, 2, 4}
        };
        return staticToDynamic<2, 3>(templ);
    }
    if (option == 4) {
        double templ[3][4] = {
            {2.34, -4.21, -11.61, 14.41},
            {8.04,  5.22,   0.27, -6.44},
            {3.92, -7.99,   8.37, 55.56}
        };
        return staticToDynamic<3, 4>(templ);
    }
    if (option == 5) {
        double templ[5][6] = {
            { 4.43, -7.21,  8.05, 1.23, -2.56,  2.62},
            {-1.29,  6.47,  2.96, 3.22,  6.12, -3.97},
            { 6.12,  8.31,  9.41, 1.78, -2.88, -9.12},
            {-2.57,  6.93, -3.74, 7.41,  5.55,  8.11},
            { 1.46,  3.62,  7.83, 6.25, -2.35,  7.23}
        };
        return staticToDynamic<5, 6>(templ);
    }
    if (option == 6) {
        double templ[4][5] = {
            {12.14,  1.32,  -0.78,  -2.75,  14.78},
            {-0.89, 16.75,   1.88,  -1.55, -12.14},
            { 2.65, -1.27, -15.64,  -0.64, -11.65},
            { 2.44,  1.52,   1.93, -11.43,   4.26}
        };
        return staticToDynamic<4, 5>(templ);
    }
    if (option == 7) {
        double templ[3][4] = {
            {4, -1,  1, 3},
            {1,  5, -1, 2},
            {1, -1, -5, 6},
        };
        return staticToDynamic<3, 4>(templ);
    }
    return nullptr;
}


// Работает вместе с getMatrix(), возвращает кол-во
// строк в матрице
int getN(int option) {
    if (option == 5) {
        return 5;
    }
    if (option == 1 || option == 2 || option == 4 || option == 7) {
        return 3;
    }
    if (option == 3) {
        return 2;
    }
    if (option == 6) {
        return 4;
    }
    return -1;
}


// Работает вместе с getMatrix(), возвращает кол-во
// столбцов в матрице
int getM(int option) {
    if (option == 5) {
        return 6;
    }
    if (option == 1 || option == 2 || option == 4 || option == 7) {
        return 4;
    }
    if (option == 3) {
        return 3;
    }
    if (option == 6) {
        return 5;
    }
    return -1;
}


// Получает строку row длины m и делит каждый элемент
// на divider
void divideRow(double* row, double divider, int m) {
    for (int i = 0; i < m; i++) {
        row[i] /= divider;
    }
}


// О боже
double* yakobi(double** matrix, double* answers, int n, int m) {
    for (int i = 0; i < n; i++) {
        matrix[i][0] = answers[i];
    }
    for (int i = 0; i < n; i++) {
        answers[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < m - 1; j++) {
            if (i == j) {
                answers[i] += matrix[i][j] * matrix[0][0];
                continue;
            }

            answers[i] += matrix[i][j] * matrix[j][0];
        }
        answers[i] += matrix[i][m-1];
    }

    return answers;
}


// О боже
double* zeidel(double** matrix, double* answers, int n, int m) {
    for (int i = 0; i < n; i++) {
        matrix[i][0] = answers[i];
    }
    for (int i = 0; i < n; i++) {
        matrix[i][0] = 0;
        for (int j = 1; j < m - 1; j++) {
            if (i == j) {
                matrix[i][0] += matrix[i][j] * matrix[0][0];
                continue;
            }

            matrix[i][0] += matrix[i][j] * matrix[j][0];
        }
        matrix[i][0] += matrix[i][m-1];
    }

    for (int i = 0; i < n; i++) {
        answers[i] = matrix[i][0];
    }

    return answers;
}


// Метод итераций (о боже)
double* iterationMethod(double** matrix, int n, int m) {
    ofstream numbers("yakobi.txt");
    // ofstream numbers("zeidel.txt");
    numbers << "n\tdiff\n";
    double* answers = new double[n];
    for (int i = 0; i < n; i++) {
        answers[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        divideRow(matrix[i], matrix[i][0], m);
    }

    cout << "After divisoin: \n";
    printMatrix(matrix, n, m);

    double maxd = 0;
    double e = 1e-10;
    int iterationCount = 0;
    double* answersOld = new double[n];
    do {
        iterationCount++;
        double** matrixNew = new double*[n];
        for (int i = 0; i < n; i++) {
            matrixNew[i] = new double[m];
            for (int j = 0; j < m; j++) {
                matrixNew[i][j] = matrix[i][j];
            }
        }


        for (int i = 0; i < n; i++) {
            answersOld[i] = answers[i];
        }
        answers = yakobi(matrixNew, answers, n, m);
        // answers = zeidel(matrixNew, answers, n, m);

        for (int i = 0; i < n; i++) {
            answersOld[i] -= answers[i];
        }
        maxd = 0;
        for (int i = 0; i < n; i++) {
            if (maxd < abs(answersOld[i])) {
                maxd = abs(answersOld[i]);
            }
        }
        cout << "Iterations: \n" << iterationCount << endl; 
        cout << "answers: \n";
        numbers << iterationCount << '\t' << maxd << endl;
        printVector(answers, n); 
        cout << "maxd: \n" << maxd << "\n\n\n";
        for (int i = 0; i < n; i++) {
            answersOld[i] = answers[i];
        }
        for (int i = 0; i < n; i++) {
            delete[]matrixNew[i];
        }
        delete[]matrixNew;
    } while (e < maxd && iterationCount < 1000);
    delete[]answersOld;
    return answers;
}


// Меняет местами i-й i-й элементы и i-й 0-й. Потом умножает
// элементы столбцы от 1 до m-1 потому что так надо
void changeMatrix(double** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        double temp = matrix[i][0];
        matrix[i][0] = matrix[i][i];
        matrix[i][i] = temp;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < m - 1; j++) {
            matrix[i][j] *= -1;
        }
    }
}


// Не смотря ни на что, это всё ещё инт майн
int main() {
    int option;
    cout << "Choose option (1-7): ";
    cin >> option;

    
    double** matrix = getMatrix(option);
    int n = getN(option);
    int m = getM(option);
    if (matrix == nullptr) {
        cout << "Bad getMatrix() option: " << option << "\n";
        return -1;
    }

    changeMatrix(matrix, n, m);
    cout << endl << n << "x" << m << " matrix:" << endl;
    printMatrix(matrix, n, m);

    
    iterationMethod(matrix, n, m);


    return 0;
}