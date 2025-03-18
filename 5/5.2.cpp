#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>


using namespace std;


// Выводит вектор по размерам
void printVector(double* vector, int n) {
    for (int i = 0; i < n; i++)
        cout << vector[i] << endl;
    cout << endl;
}


// Выводит матрицу по размерам
void printMatrix(double** matrix, int n, int m) {
    cout << "N = " << n << "\n";
    for (int i = 0; i < n; i++) {
        // cout << "i = " << i << "\n";
        for (int j = 0; j < m; j++) {
            cout << setw(13) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "hehe\n";
    cout << endl;
}


void matrixInFile(double** matrix, int n, int m) {
    cout << "hehe3\n";
    ofstream filefile("hehe.txt");
    cout << "hehe4\n";
    // filefile.open("hehe.txt");
    // cout << "hehe5\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            filefile << matrix[i][j] << "\t";
        }
        filefile << "\n";
    }
    filefile << "\n";
    filefile.close();
}


void clearMemory(double* vect) {
    delete[]vect;
}


void clearMemory(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[]matrix[i];
    }
    delete[]matrix;
}


// Получает статическую матрицу размеров NxM и вжмыхивает
// её в динамическую
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


// Значение функции в точке х
double calculateFunction(double x) {
    return 1 / (1 + 25 * pow(x, 2));
}


void transformValues(double* values, int n) {
    for (int i = 1; i < n - 1; i++) {
        values[i] = values[i] - 2 * values[i + 1] + values[i + 2];
    }
}


// Возвращает таблицу значений функции
double* getYValues(int n) {
    n--;
    double* values = new double[n];
    for (int i = 0; i <= n; i++) {
        values[i] = calculateFunction(-1 + (2.0 / n) * i);
        // Я не знаю почему, но если убрать тут вывод
        // то программа ломается
        cout << "teamFortress2 coconut.png\n";
        cout << "x = " << -1 + (2.0 / n) * i << "\n";
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
// Вместо y - (y1 - 2*y2 + y3)
double** getMatrix(int n, int m) {
    double** matrix = new double*[n];
    double* values = getYValues(n + 2);
    cout << "Basic values:\n";
    printVector(values, n + 2);
    transformValues(values, n + 2);
    cout << "Transformed values:\n";
    printVector(values, n + 2);
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[m];
    }
    for (int i = 0; i < n - 1; i++) {
        matrix[i][i] = 4;
        matrix[i][i+1] = 1;
        matrix[i+1][i] = 1;
    }
    for (int i = 0; i < n; i++) {
        matrix[i][m-1] = values[i + 1];
    }
    matrix[n-1][n-1] = 4;
    return matrix;
}


// Получает строку row длины m и делит каждый элемент
// на divider
void divideRow(double* row, double divider, int m) {
    for (int i = 0; i < m; i++) {
        row[i] /= divider;
    }
}


// Получает две строки row1 и row2 длины m и вычитает
// из row1 row2
void subtractRow(double* row1, double* row2, int m) {
    for (int i = 0; i < m; i++) {
        row1[i] -= row2[i];
    }
}


// Находит строку с максимальным элементом в столбце i
// и меняет местами со строкой i
void swapRows(double** matrix, int n, int i) {
    double max = 0;
    int maxindex = 0;
    for (int j = i; j < n; j++) {
        if (abs(matrix[j][i]) > max) {
            max = abs(matrix[j][i]);
            maxindex = j;
        }
    }
    double* temp = matrix[maxindex];
    matrix[maxindex] = matrix[i];
    matrix[i] = temp;
}


// Жмых
void zhmihnutMatrix(double** matrix, int n, int m) {
    m--;
    for (int i = 0; i < n / 2; i++) {
        double* temp = matrix[i];
        matrix[i] = matrix[n - i - 1];
        matrix[n - i - 1] = temp;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m / 2; j++) {
            double temp2 = matrix[i][j];
            matrix[i][j] = matrix[i][m - j - 1];
            matrix[i][m - j - 1] = temp2;
        }
    }
}


// Метод Гаусса, тут всё ясно (как же верим)
void gaussMethod(double** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        swapRows(matrix, n, i);
        for (int j = i; j < n; j++) {
            if (matrix[j][i] == 0) {
                continue;
            }
            divideRow(matrix[j], matrix[j][i], m);
        }
        for (int j = i + 1; j < n; j++) {
            if (matrix[j][i] == 0) {
                continue;
            }
            subtractRow(matrix[j], matrix[i], m);
        }
    }
    zhmihnutMatrix(matrix, n, m);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (matrix[j][i] == 0) {
                continue;
            }
            divideRow(matrix[j], matrix[j][i], m);
        }
        for (int j = i + 1; j < n; j++) {
            if (matrix[j][i] == 0) {
                continue;
            }
            subtractRow(matrix[j], matrix[i], m);
        }
    }
    zhmihnutMatrix(matrix, n, m);
}


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
    printVector(vector, n);
    return vector;
}


double** getABCDMatrix(double* mValues, int n) {
    double* values = getYValues(n + 1);
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


double getXByABCD(double* abcd, double x, double xi) {
    return  abcd[0]*pow((x - xi), 3) + 
            abcd[1]*pow((x - xi), 2) + 
            abcd[2]*pow((x - xi), 1) +
            abcd[3];
}


double** getYBySpline(int n, double** abcd) {
    double** matrix = new double*[n * 10];
    cout << n << "\n";
    for (int i = 0; i < n * 10; i++) {
        matrix[i] = new double[2];
    }
    for (int i = 0; i < n; i++) {
        cout << "i = " << i << "\n";
        double xi = -1 + ((2.0 / n) * i);
        for (int j = 0; j < 10; j++) {
            cout << "j = " << j << "\n";
            double x = -1 + ((2.0 / n) * i) + ((2.0 * j) / (n * 10));
            matrix[i*10 + j][0] = x;
            matrix[i*10 + j][1] = getXByABCD(abcd[i], x, xi);
        }
        // cout << "\n";
    }
    cout << "hehe\n";
    // printMatrix(matrix, n * 10, 2);
    cout << "hehe2\n";
    matrixInFile(matrix, n * 10, 2);

    return matrix;
}


// Пяу пипяу пипяу пипяупяу
int main() {
    int n;
    cin >> n;
    int m = n + 1;
    double** matrix = getMatrix(n, m);
    cout << "Default matrix:\n";
    printMatrix(matrix, n, m);


    // Находим значения Mi, записываем в вектор
    cout << "Gauss:\n";
    gaussMethod(matrix, n, m);
    printMatrix(matrix, n, m);
    n += 2;
    double* mValues = new double[n];
    mValues[0] = 0;
    mValues[n - 1] = 0;
    for (int i = 0; i < n - 2; i++) {
        mValues[i + 1] = matrix[i][m - 1];
    }
    m = 4;
    clearMemory(matrix, n - 2);
    n--;
    double** abcdMatrix = getABCDMatrix(mValues, n);
    cout << "ABCD values:\n";
    printMatrix(abcdMatrix, n, m);
    getYBySpline(n, abcdMatrix);

    double* realValues = getYValues(n);
    cout << n;
    ofstream real("real.csv");
    for (int i = 0; i < n * 10; i++) {
        real << calculateFunction(-1 + (2.0 / (n * 10.0))*i);
    }
    clearMemory(mValues);
}