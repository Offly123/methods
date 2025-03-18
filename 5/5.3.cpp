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
    for (int i = 0; i < n; i++) {
        // cout << "i = " << i << "\n";
        for (int j = 0; j < m; j++) {
            cout << setw(8) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// Суёт матрицу в файл hehe.txt.
// Вроде используется только для запихивания
// значений сплайна.
void matrixInFile(double** matrix, int n, int m) {
    ofstream filefile("5.3.csv");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            filefile << matrix[i][j] << "\t";
        }
        filefile << "\n";
    }
    filefile << "\n";
    filefile.close();
}


// Очищает память от вектора (полезно)
void clearMemory(double* vect) {
    delete[]vect;
}


// Очищает память от матрицы (полезно)
void clearMemory(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[]matrix[i];
    }
    delete[]matrix;
}


// Делает вместо y1 - 
// y1 - 2*y2 + y3
// Так надо в судя по умной книжке
void transformValues(double* values, int n) {
    for (int i = 0; i < n - 2; i++) {
        values[i] = values[i] - 2 * values[i + 1] + values[i + 2];
    }
}


// Возвращает таблицу значений функции
double* getYValues() {
    double* values;
    values[0] = 4;
    values[1] = -2;
    values[2] = 6;
    values[3] = -3;
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
double** getMatrix() {
    int n = 2;
    int m = 3;
    double** matrix = new double*[n];
    cout << "hehe\n";
    double* values = getYValues();
    printVector(values, 4);
    transformValues(values, 4);
    printVector(values, 4);
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


// Жмых (камео из 3-ей задачи)
// Переворачивает матрицу сначала вертикально,
// a потом горизонтально
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


// Метод Гаусса (камео из 3-ей задачи)
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


// По идее находит матрицу коэффициентов
// a, b, c, d
double** getABCDMatrix(double* mValues, int n) {
    // Кокос вечен (если его удалить всё
    // снова сломается)
    cout << "coconut.png ";
    double* values = getYValues();
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


// Считает непонятно что по коэффициентам abcd
// Вроде Y, но название функции говорит что X,
// впадлу разбираться
double getXByABCD(double* abcd, double x, double xi) {
    return  abcd[0]*pow((x - xi), 3) + 
        abcd[1]*pow((x - xi), 2) + 
        abcd[2]*pow((x - xi), 1) +
        abcd[3];
}


// Получает коэффициенты a, b, c, d и записывает
// значения сплайна в файл
double** getYBySpline(int n, double** abcd) {
    double** matrix = new double*[n * 10];
    for (int i = 0; i < n * 10; i++) {
        matrix[i] = new double[2];
    }
    for (int i = 0; i < 30; i++) {
        cout << "i = " << i << "\n";
        if (i < 10) {
            double x = 2 + (1.0 * i) / 10;
            double xi = 2;
            cout << "x = " << x << "\n";
            cout << "xi = " << xi << "\n";
            matrix[i][0] = x;
            matrix[i][1] = getXByABCD(abcd[0], x, xi);
            continue;
        }
        if (i < 20) {
            double x = 3 + (0.2 * (i - 10));
            double xi = 3;
            cout << "x = " << x << "\n";
            cout << "xi = " << xi << "\n";
            matrix[i][0] = x;
            matrix[i][1] = getXByABCD(abcd[1], x, xi);
            continue;
        }
        if (i < 30) {
            double x = 5 + (0.2 * (i - 20));
            double xi = 5;
            cout << "x = " << x << "\n";
            cout << "xi = " << xi << "\n";
            matrix[i][0] = x;
            matrix[i][1] = getXByABCD(abcd[2], x, xi);
            continue;
        }
    }
    matrixInFile(matrix, n * 10, 2);

    return matrix;
}


// Пяу пипяу пипяу пипяупяу
int main() {
    int n = 2;
    int m = 3;
    double** matrix = getMatrix();
    cout << "Default matrix:\n";
    printMatrix(matrix, n, m);


    // Находим значения Mi, записываем в вектор.
    // вектор[0] и вектор[n-1] равны 0, потому что
    // там в умной книжке написано что так надо
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
    printVector(mValues, 4);
    m = 4;
    clearMemory(matrix, n - 2);
    n--;


    // Получеаем коэффициенты a, b, c, d для каждого
    // промежутка.
    double** abcdMatrix = getABCDMatrix(mValues, n);
    cout << "ABCD values:\n";
    printMatrix(abcdMatrix, n, m);


    // Находим значения функции по коэффициентам 
    // при помощи сплайна. Тут же записываем их в
    // 5.3.csv
    getYBySpline(n, abcdMatrix);
    return 0;
}


// Умная книжка
// https://web.archive.org/web/20090408054627/http://online.redwoods.cc.ca.us/instruct/darnold/laproj/Fall98/SkyMeg/Proj.PDF