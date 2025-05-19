#include <iostream>
#include <cmath>
#include <iomanip>
#include <gauss.h>

using namespace std;

void printVector(double* vect, int size) {
    for (int i = 0; i < size; i++) {
        cout << vect[i] << ' ';
    }
    cout << "\n\n";
}

void printMatrix(double** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << setw(11) << matrix[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}



// Точные значения функции
double accurateFunction(double x, double t) {
    return pow(M_E, -t) * sin(x);
}

double defaultU_X(double x) {
    return sin(x);
}

// Матрица погрешностей для всех значений
double** getDiffeernceMatrix(
    double** matrix, 
    double minX, double maxX, int partitionsX, 
    double minT, double maxT, int partitionsT
) {
    double stepX = (maxX - minX) / partitionsX;
    double stepT = (maxT - minT) / partitionsT;
    double** diff = new double*[partitionsT++];

    for (int i = 0; i < partitionsT; i++) {
        diff[i] = new double[partitionsX];
        for (int j = 0; j < partitionsX; j++) {
            double x = minX + stepX * j;
            double t = minT + stepT * i;
            diff[i][j] = (abs(accurateFunction(x, t) - matrix[i][j])) / accurateFunction(x, t) * 100.0;
        }
    }

    return diff;
}

// Получить граничные значения
// (первая строка и нулевой-последний элементы остальных строк)
void setBoundaries(
    double** matrix,
    double minX, double maxX, int partitionsX, 
    double minT, double maxT, int partitionsT
) {
    double stepX = (maxX - minX) / (partitionsX - 1);
    double stepT = (maxT - minT) / (partitionsT - 1);
    // Заполняем нулевые и последние элементы каждой строки
    for (int i = 0; i < partitionsT; i++) {
        matrix[i][0] = 0;

        double t = (minT + i * stepT);
        matrix[i][partitionsX - 1] = pow(M_E, -t);
    }

    // Заполняем первую строку
    for (int i = 1; i < partitionsX - 1; i++) {
        double x = minX + stepX * i;
        matrix[0][i] = defaultU_X(x);
    }
}



// Явная схема (плохая)
void handleExplicit(
    double minX, double maxX, int partitionsX, 
    double minT, double maxT, int partitionsT
) {
    double stepX = (maxX - minX) / partitionsX;
    double stepT = (maxT - minT) / partitionsT;
    double** matrix = new double*[partitionsT++];
    partitionsX++;

    // Выделяем память
    // T - строки
    // X - столбцы
    for (int i = 0; i < partitionsT; i++) {
        matrix[i] = new double[partitionsX];
    }

    // Устанавливаем крайние элементы
    setBoundaries(
        matrix, 
        minX, maxX, partitionsX, 
        minT, maxT, partitionsT
    );


    // Заполняем всю оставшуюся матрицу
    for (int i = 1; i < partitionsT; i++) {
        for (int j = 1; j < partitionsX - 1; j++) {
            matrix[i][j] = matrix[i - 1][j] + stepT * (matrix[i - 1][j + 1] - 2 * matrix[i - 1][j] + matrix[i - 1][j - 1]);
        }
    }

    printMatrix(matrix, partitionsT, partitionsX);

    double** diff = getDiffeernceMatrix(
        matrix,
        minX, maxX, partitionsX - 1,
        minT, maxT, partitionsT - 1
    );
    cout << "Differences: \n";
    printMatrix(diff, partitionsT, partitionsX);
}

// 
double** getBaseMatrix(
    double** matrix,
    double* previousVector,
    double lastValue,
    int n, int m, double r
) {
    // cout << "\nr = " << r << "\n";
    double** base = new double*[n];
    for (int i = 0; i < n; i++) {
        base[i] = new double[m];
        base[i][m - 1] = previousVector[i];
    }
    base[n - 1][m - 1] += r * lastValue;
    for (int i = 0; i < n - 1; i++) {
        base[i][i] = 1 + 2 * r;
        base[i][i + 1] = -r;
        base[i + 1][i] = -r;
    }
    base[n - 1][n - 1] = 1 + 2 * r;

    return base;
}

// Взять значения из центра (от решения гауссом)
double* getMiddleValues(double** matrix, int n, int m) {
    double* vector = new double[m - 1];

    for (int i = 0; i < m - 2; i++) {
        vector[i] = matrix[n][i + 1];
    }
    return vector;
}

// Получить последний столбец (решение гауссом)
double* getLastValues(double** matrix, int n, int m) {
    double* vector = new double[n - 1];

    for (int i = 0; i < n; i++) {
        vector[i] = matrix[i][m - 1];
    }

    return vector;
}

// Вставить значения, которые не в краях (от T(1) до T(N-1) )
void insertMiddleValues(double** matrix, double* vector, int n, int m) {
    for (int i = 1; i < m - 1; i++) {
        // cout << i << "\n";
        matrix[n][i] = vector[i - 1];
    }
}

// Неявная схема (хорошая)
void handleImplicit(
    double minX, double maxX, int partitionsX, 
    double minT, double maxT, int partitionsT
) {
    double stepX = (maxX - minX) / partitionsX;
    double stepT = (maxT - minT) / partitionsT;
    // cout << "stepX = " << stepX << "\n";
    // cout << "stepT = " << stepT << "\n";
    double r = stepT / pow(stepX, 2);
    double** matrix = new double*[partitionsT++];
    partitionsX++;

    // Выделяем память
    // T - строки
    // X - столбцы
    for (int i = 0; i < partitionsT; i++) {
        matrix[i] = new double[partitionsX];
    }

    setBoundaries(
        matrix,
        minX, maxX, partitionsX,
        minT, maxT, partitionsT
    );
    // cout << "Default matrix: \n";
    // printMatrix(matrix, partitionsT, partitionsX);

    for (int i = 1; i < partitionsT; i++) {
        double* middleVector = getMiddleValues(matrix, i - 1, partitionsX);

        double** base = getBaseMatrix(matrix, middleVector, matrix[i][partitionsX - 1], partitionsX - 2, partitionsX - 1, r);
        // cout << "Base: \n";
        // printMatrix(base, partitionsX - 2, partitionsX - 1);

        gaussMethod(base, partitionsX - 2, partitionsX - 1);
        // cout << "Base resheno: \n";
        // printMatrix(base, partitionsX - 2, partitionsX - 1);
        double* lastValues = getLastValues(base, partitionsX - 2, partitionsX - 1);
        // cout << "lastVector: \n";
        // printVector(lastValues, partitionsX - 2);

        insertMiddleValues(matrix, lastValues, i, partitionsX);
        // cout << "New matrix: \n";
        // printMatrix(matrix, partitionsT, partitionsX);
    }

    cout << "Final values: \n";
    printMatrix(matrix, partitionsT, partitionsX);
    double** diff = getDiffeernceMatrix(
        matrix,
        minX, maxX, partitionsX - 1,
        minT, maxT, partitionsT - 1
    );
    cout << "Differences: \n";
    printMatrix(diff, partitionsT, partitionsX);
}



int main() {
    cout << fixed << setprecision(8);

    int method;
    cout << "Choose method (1 - explicit, 2 - implicit): ";
    cin >> method;

    if (method != 1 && method != 2) {
        cout << "\nBad method\n\n";
        return -1;
    }



    const double minX = 0.0;
    const double maxX = M_PI / 2;
    int partitionsX;
    cout << "Input partitionsX: ";
    cin >> partitionsX;

    const double minT = 0.0;
    double maxT;
    cout << "Input T: ";
    cin >> maxT;
    int partitionsT;
    cout << "Input partitionsT: ";
    cin >> partitionsT;



    // Неявный метод
    if (method == 1) {
        handleExplicit(minX, maxX, partitionsX, minT, maxT, partitionsT);
    }

    // Явный метод
    if (method == 2) {
        handleImplicit(minX, maxX, partitionsX, minT, maxT, partitionsT);
    }



    return 0;
}
