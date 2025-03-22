#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

using namespace std;


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


// Выводит матрицу по размерам
void printMatrix(double** matrix, int n, int m) {
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
    for (int i = 0; i < n; i++)
        cout << vector[i] << " ";
    cout << "\n\n";
}


// Получает на вход число, по нему выбирает матрицу
// и возвращает её
double** getMatrix(int option) {
    if (option == 1) {
        double templ[6][2] = {
            {1,  2.0},
            {3,  10.1},
            {5,  22.6},
            {7,  37.1},
            {9,  54.5},
            {11, 73.2}
        };
        return staticToDynamic<6, 2>(templ);
    }
    if (option == 2) {
        double templ[6][2] = {
            {1, 1.0},
            {2, 1.5},
            {3, 3.0},
            {4, 4.5},
            {5, 7.0},
            {6, 8.5}
        };
        return staticToDynamic<6, 2>(templ);
    }
    return nullptr;
}


// Работает вместе с getMatrix(), возвращает кол-во
// строк в матрице
int getN(int option) {
    if (option == 1 || option == 2) {
        return 6;
    }
    return -1;
}


// Работает вместе с getMatrix(), возвращает кол-во
// столбцов в матрице
int getM(int option) {
    if (option == 1 || option == 2) {
        return 2;
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


// Метод Гаусса
void gaussMethod(double** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        swapRows(matrix, n, i);
        for (int j = i; j < n; j++) {
            divideRow(matrix[j], matrix[j][i], m);
        }
        for (int j = i + 1; j < n; j++) {
            subtractRow(matrix[j], matrix[i], m);
        }
    }
    zhmihnutMatrix(matrix, n, m);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            divideRow(matrix[j], matrix[j][i], m);
        }
        for (int j = i + 1; j < n; j++) {
            subtractRow(matrix[j], matrix[i], m);
        }
    }
    zhmihnutMatrix(matrix, n, m);
}


// Сумма значений x^degree
double getXSum(
    double** functionValues, int n, 
    bool useLogarithm = false,
    int degree = 1
) {
    double sum = 0;


    if (useLogarithm) {
        for (int i = 0; i < n; i++) {
            sum += pow(log(functionValues[i][0]), degree);
        }
        return sum;
    }


    for (int i = 0; i < n; i++) {
        sum += pow(functionValues[i][0], degree);
    }


    return sum;
}


// Сумма значений y^degree
double getYSum(
    double** functionValues, int n, 
    bool useLogarithm = false,
    int degree = 1
) {
    double sum = 0;


    if (useLogarithm) {
        for (int i = 0; i < n; i++) {
            sum += pow(log(functionValues[i][1]), degree);
        }
        return sum;
    }


    for (int i = 0; i < n; i++) {
        sum += pow(functionValues[i][1], degree);
    }


    return sum;
}


// Возвращает сумму произведений значений x и y 
// из изначальной таблицы
double getXYSum(
    double** functionValues, int n, 
    bool useLogarithmX = false, bool useLogarithmY = false,
    int degreeX = 1, int degreeY = 1
) {
    double sum = 0;


    if (useLogarithmX && useLogarithmY) {
        for (int i = 0; i < n; i++) {
        sum += pow(log(functionValues[i][0]), degreeX) * 
               pow(log(functionValues[i][1]), degreeY);
        }
        return sum;
    }


    if (useLogarithmX) {
        for (int i = 0; i < n; i++) {
        sum += pow(log(functionValues[i][0]), degreeX) * 
               pow(functionValues[i][1], degreeY);
        }
        return sum;
    }


    if (useLogarithmY) {
        for (int i = 0; i < n; i++) {
        sum += pow(functionValues[i][0], degreeX) * 
               pow(log(functionValues[i][1]), degreeY);
        }
        return sum;
    }    


    for (int i = 0; i < n; i++) {
        sum += pow(functionValues[i][0], degreeX) * 
               pow(functionValues[i][1], degreeY);
    }


    return sum;
}


// Возвращает матрицу - систему для линейного уравнения
// если useLogarithm - получится матрица для степенной функции
double** getLinearMatrix(
    double** functionValues, 
    int n, int m, 
    bool useLogarithmX = false, bool useLogarithmY = false
) {
    double** matrix = new double*[2];
    for (int i = 0; i < 2; i++) {
        matrix[i] = new double[3];
    }


    // Я хз как это сделать по нормальному 
    // так что все значения ручками вбиваем
    //
    // Первая строка
    matrix[0][0] = getXSum(functionValues, n, useLogarithmX, 2);
    matrix[0][1] = getXSum(functionValues, n, useLogarithmX);
    matrix[0][2] = getXYSum(functionValues, n, useLogarithmX, useLogarithmY);

    // Вторая строка
    matrix[1][0] = getXSum(functionValues, n, useLogarithmX);
    matrix[1][1] = n;
    matrix[1][2] = getYSum(functionValues, n, useLogarithmY);
    
    return matrix;
}


// Возвращает матрицу - систему для квадратичного уравнения
double** getQuadraticMatrix(double** functionValues, int n, int m) {
    double** matrix = new double*[3];
    for (int i = 0; i < 3; i++) {
        matrix[i] = new double[4];
    }


    // Иногда надо пострадывать
    //
    // Первая строка
    matrix[0][0] =  getXSum(functionValues, n, false, 4);
    matrix[0][1] =  getXSum(functionValues, n, false, 3);
    matrix[0][2] =  getXSum(functionValues, n, false, 2);
    matrix[0][3] = getXYSum(functionValues, n, false, false, 2, 1);
    
    // Вторая строка
    matrix[1][0] =   getXSum(functionValues, n, false, 3);
    matrix[1][1] =   getXSum(functionValues, n, false, 2);
    matrix[1][2] =   getXSum(functionValues, n, false, 1);
    matrix[1][3] =  getXYSum(functionValues, n, false, false, 1, 1);

    // Третья строка
    matrix[2][0] =  getXSum(functionValues, n, false, 2);
    matrix[2][1] =  getXSum(functionValues, n, false, 1);
    matrix[2][2] =  n;
    matrix[2][3] =  getYSum(functionValues, n, false, 1);
    
    return matrix;
}


// Возвращает коэффициенты из решённой матрицы
double* getCoeffs(double** matrix, int n, int m) {
    double* coeffs = new double[n];


    for (int i = 0; i < n; i++) {
        coeffs[i] = matrix[i][m - 1];
    }


    return coeffs;
}


// Функции получают массив коэффициентов и возвращают
// значание
double calculateLinear(double* coeffs, double x) {
    return coeffs[0] * x + coeffs[1];
}

double calculateDegree(double* coeffs, double x) {
    return coeffs[1] * pow(x, coeffs[0]);
}

double calculateExponential(double* coeffs, double x) {
    return coeffs[1] * pow(M_E, coeffs[0] * x);
}

double calculateQuadratic(double* coeffs, double x) {
    return coeffs[0] * pow(x, 2) + coeffs[1] * x + coeffs[2];
}


// Штирлиц гулял по парку весной и увидел почки на деревьях
//
//
//
//
//
// "Снова Мюллер зверствует", - подумал Штирлиц
int main() {
    int option;
    cout << "Choose option (1-2): ";
    cin >> option;


    double** functionValues = getMatrix(option);
    int n = getN(option);
    int m = getM(option);
    printMatrix(functionValues, n, m);
    cout << "\n\n";


    // Находим коэффициенты линейной функции
    cout << "Linear: \n";
    double** linearMatrix = 
        getLinearMatrix(functionValues, n, m);


    gaussMethod(linearMatrix, 2, 3);
    double* linearCoeffs = getCoeffs(linearMatrix, 2, 3);
    printVector(linearCoeffs, 2);


    // Находим коэффициенты степенной функци
    cout << "Degree: \n";
    double** degreeMatrix = 
        getLinearMatrix(functionValues, n, m, true, true);


    gaussMethod(degreeMatrix, 2, 3);
    double* degreeCoeffs = getCoeffs(degreeMatrix, 2, 3);
    degreeCoeffs[1] = pow(M_E, degreeCoeffs[1]);
    printVector(degreeCoeffs, 2);


    // Находим коэффициенты показательной функции
    cout << "Exponential: \n";
    double** exponentialMatrix = 
        getLinearMatrix(functionValues, n, m, false, true);


    gaussMethod(exponentialMatrix, 2, 3);
    double* exponentialCoeffs = getCoeffs(exponentialMatrix, 2, 3);
    exponentialCoeffs[1] = pow(M_E, exponentialCoeffs[1]);
    printVector(exponentialCoeffs, 2);


    // Находим коэффициенты квадратичной функции
    cout << "Quadratic: \n";
    double** quadraticMatrix = 
        getQuadraticMatrix(functionValues, n, m);

    
    gaussMethod(quadraticMatrix, 3, 4);
    double* quadraticCoeffs = getCoeffs(quadraticMatrix, 3, 4);
    printVector(quadraticCoeffs, 3);
    cout << "\n\n";


    // Считаем сумму квадратов погрешностей
    double sumLinear = 0;
    double sumDegree = 0;
    double sumExponential = 0;
    double sumQuadratic = 0;
    for (int i = 0; i < n; i++) {
        // Я не знаю как это записать красиво чтобы
        // это не была строка на 100 символов
        sumLinear += pow(
            (calculateLinear(
                linearCoeffs, 
                functionValues[i][0]
            ) - functionValues[i][1]), 
            2
        );
        sumDegree += pow(
            (calculateDegree(
                degreeCoeffs, 
                functionValues[i][0]
            ) - functionValues[i][1]), 
            2
        );
        sumExponential += pow(
            (calculateExponential(
                exponentialCoeffs, 
                functionValues[i][0]
            ) - functionValues[i][1]), 
            2
        );
        sumQuadratic += pow(
            (calculateQuadratic(
                quadraticCoeffs, 
                functionValues[i][0]
            ) - functionValues[i][1]), 
            2
        );
    }
    cout << sumLinear << "\n";
    cout << sumDegree << "\n";
    cout << sumExponential << "\n";
    cout << sumQuadratic << "\n";


    return 0;
}