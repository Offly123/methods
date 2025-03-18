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
    ofstream filefile("hehe.txt");
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


// Получает статическую матрицу размеров NxM и вжмыхивает
// её в динамическую (бесполезное камео из 3-ей задачи.
// Можно удалить, но мне жалко).
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


// Значение функции в точке x
double calculateFunction(double x) {
    return 1 / (1 + 25 * pow(x, 2));
}


// Делает вместо y1 - 
// y1 - 2*y2 + y3
// Так надо в судя по умной книжке
void transformValues(double* values, int n) {
    for (int i = 1; i < n - 1; i++) {
        values[i] = values[i] - 2 * values[i + 1] + values[i + 2];
    }
}


// Возвращает таблицу значений функции
// если просто подставлять x
double* getYValues(int n) {
    n--;
    double* values = new double[n];
    for (int i = 0; i < n; i++) {
        values[i] = calculateFunction(-1 + (2.0 / n) * i);
        // Это кокос. Если убрать его отсюда, то программа
        // начнёт ломаться при вводе чётных n.
        // Я не знаю почему, не трогайте кокос.
        cout << "coconut.png ";
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
double** getMatrix(int n, int m) {
    double** matrix = new double*[n];
    double* values = getYValues(n + 2);
    cout << "Basic values:\n";
    transformValues(values, n + 2);
    cout << "Transformed values:\n";
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
    for (int i = 0; i < n; i++) {
        double xi = -1 + ((2.0 / n) * i);
        for (int j = 0; j < 10; j++) {
            double x = -1 + ((2.0 / n) * i) + ((2.0 * j) / (n * 10));
            matrix[i*10 + j][0] = x;
            matrix[i*10 + j][1] = getXByABCD(abcd[i], x, xi);
        }
    }
    matrixInFile(matrix, n * 10, 2);

    return matrix;
}


// Пяу пипяу пипяу пипяупяу
int main() {
    // Получает n и строит матрицу
    //
    // ВАЖНО:
    // При чётных n программа ломается и
    // я понятия не имею почему. Просто не вводите
    // чётные n.
    int n;
    cin >> n;
    int m = n + 1;
    double** matrix = getMatrix(n, m);
    cout << "Default matrix:\n";
    printMatrix(matrix, n, m);


    // Находим значения Mi, записываем в вектор.
    // вектор[0] и вектор[n-1] равны 0, потому что
    // там в умной книжке написано что так надо.
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


    // Получеаем коэффициенты a, b, c, d для каждого
    // промежутка.
    double** abcdMatrix = getABCDMatrix(mValues, n);
    cout << "ABCD values:\n";
    printMatrix(abcdMatrix, n, m);


    // Находим значения функции по коэффициентам 
    // при помощи сплайна. Тут же записываем их в
    // hehe.txt (я не знаю почему не .csv)
    getYBySpline(n, abcdMatrix);


    // Записываем значения функции в real.csv
    // если просто подставить в функцию x
    double* realValues = getYValues(n);
    ofstream real("real.csv");
    for (int i = 0; i < n * 10; i++) {
        real << -1 + (2.0 / (n * 10.0)) * i << "\t";
        real << calculateFunction(-1 + (2.0 / (n * 10.0)) * i);
        real << "\n";
    }


    // Тут как бы надо почистить память но мне лень.
    // A чё за рандомные изменения n вообще не спрашивайте.
    // A кто вы то? Я один тут.
    return 0;
}


// Умная книжка
// https://web.archive.org/web/20090408054627/http://online.redwoods.cc.ca.us/instruct/darnold/laproj/Fall98/SkyMeg/Proj.PDF


// Слава кокосу.