// Заголовочник для нахождения кубического сплайна

#include <gauss.h>
#include <dynamics.h>

// Делает вместо y1 - 
// y1 - 2*y2 + y3
void transformValues(double* values, int n) {
    for (int i = 1; i < n - 1; i++) {
        values[i] = values[i] - 2 * values[i + 1] + values[i + 2];
    }
}


// Возвращает таблицу значений функции
// если просто подставлять x
double* getYValues(
    double (*func)(double),
    double a, double b,
    int n
) {
    double h = (b - a) / n;
    double* values = new double[n];
    for (int i = 0; i < n; i++) {
        values[i] = func(a + h * i);
    }

    return values;
}


// Возвращает матрицу вида
// {4, 1, 0, ..., 0, 0, 0, y}
// {1, 4, 1, ..., 0, 0, 0, y}
// {0, 1, 4, ..., 0, 0, 0, y}
// {....................., y}
// {0, 0, 0, ..., 4, 1, 0, y}
// {0, 0, 0, ..., 1, 4, 1, y}
// {0, 0, 0, ..., 0, 1, 4, y}
// Вместо y - 
// (y1 - 2*y2 + y3)
// Размеры вроде N-2 на M
double** getMatrix(
    double (*func)(double),
    double a, double b,
    int n
) {
    double** matrix = new double*[n];
    double* values = getYValues(
        func,
        a, b,
        n
    );
    transformValues(values, n + 2);
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n + 1];
    }
    for (int i = 0; i < n - 1; i++) {
        matrix[i][i] = 4;
        matrix[i][i+1] = 1;
        matrix[i+1][i] = 1;
    }
    for (int i = 0; i < n; i++) {
        matrix[i][n] = values[i + 1];
    }
    matrix[n-1][n-1] = 4;
    return matrix;
}


// Находит коэффициенты a, b, c, d
// для каждого промежутка
double* getABCDVector(
    double m1, double m2, 
    double y1, double y2, 
    int n
) {
    double h = 2.0 / n;
    n-=2;

    double* vector = new double[4];
    vector[0] = (m2 - m1) / (6 * h);
    vector[1] = m1 / 2;
    vector[2] = (y2-y1) / h - (h * (m2 + 2 * m1)) / 6;
    vector[3] = y1;

    return vector;
}

double* getMValues(
    double** matrix,
    int n, int m
) {
    double* mValues = new double[n];

    for (int i = 0; i < n; i++) {
        mValues[i] = matrix[i][m - 1];
    }

    return mValues;
}

// Находит матрицу коэффициентов
// a, b, c, d
double** getABCDMatrix(
    double (*foo)(double),
    double* mValues, 
    double a, double b,
    int n
) {
    double* values = getYValues(
        foo,
        a, b,
        n
    );
    double** matrix = new double*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[4];
        matrix[i] = getABCDVector(
            mValues[i], mValues[i+1],
            values[i], values[i+1],
            n
        );
    }
    return matrix;
}

double* getCCoefficients(
    double** matrix,
    int n
) {
    double* cCOefficients = new double[n];

    for (int i = 0; i < n; i++) {
        cCOefficients[i] = matrix[i][2];
    }

    return cCOefficients;
}