// Заголовочник для вывода динамических штук

#include <iostream>
#include <iomanip>

// Получает статическую матрицу размеров NxM и 
// возвращает динамическую
template <int N, int M>
double** staticToDynamic(double[N][M]);

// Выводит матрицу по размерам
void printMatrix(double**, int, int);

// Выводит вектор по размерам
void printVector(double*, int);





// Получает статическую матрицу размеров NxM и 
// возвращает динамическую
template <int N, int M>
double** staticToDynamic(double templ[N][M]) {
    double** matrix = new double* [N];
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
    using namespace std;
    cout << fixed << setprecision(10) << setw(20);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << setw(10) << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}


// Выводит вектор по размерам
void printVector(double* vector, int n) {
    using namespace std;
    cout << fixed << setprecision(10) << setw(20);

    for (int i = 0; i < n; i++)
        cout << vector[i] << " ";
    cout << "\n";
}