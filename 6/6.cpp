#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

// Возвращают значение подынтегральной функции 
// в точке x

// Для первого пункта
double expFunction(double x) {
    return pow(M_E, -(pow(x, 2)));
}
// Для второго пункта
double piApproximate(double x) {
    return 4.0 / (1 + pow(x, 2));
}
// Для третьего пункта
double thirdFunc(double x) {
    if (x < 2) {
        return pow(M_E, pow(x, 2));
    }
    return 1.0 / (4 - sin(16 * M_PI * x));
}

// Возвращает указатель на подынтегральную функцию
double (*getIntegrand(int option))(double) {
    if (option == 1) {
        return &expFunction;
    }
    if (option == 2) {
        return &piApproximate;
    }
    if (option == 3) {
        return &thirdFunc;
    }
    return nullptr;
}



// Считает площадь прямоугольника по двум сторонам
// args == [f(x1), h]
double getRectangleArea(double* args) {
    return args[0] * args[1];
}

// Считает площадь трапеции по двум сторонам a, b и высоте h
// args == [f(x0), f(x1), h]
double getTrapezoidArea(double* args) {
    return ((args[0] + args[1]) * args[2]) / 2;
}

// Считает площадь под параболой по трём точкам
// args == [f(x0), f(x1), f(x2), h]
double getParabolaArea(double* args) {
    return ((args[0] + 4 * args[1] + args[2]) * args[3]) / 6.0;
}

// Возвращает указатель на функцию по выбранному методу
double (*getMethod(int option))(double*) {
    if (option == 1) {
        return &getRectangleArea;
    }
    if (option == 2) {
        return &getTrapezoidArea;
    }
    if (option == 3) {
        return &getParabolaArea;
    }

    return nullptr;
}

// Возвращает список аргументов в зависимости от выбранного метода
double* getArgs(
    double (*method)(double*), 
    double (*integrand)(double),
    int a, double h, double i
) {
    if (method == getRectangleArea) {
        return new double[2] {
            integrand(a + h * i), 
            h
        };
    }
    if (method == getTrapezoidArea) {
        return new double[3] {
            integrand(a + h * i), 
            integrand(a + h * (i + 1)), 
            h
        };
    }
    if (method == getParabolaArea) {
        return new double[4]{
            integrand(a + h * i),
            integrand(a + h * i + h / 2),
            integrand(a + h * (i + 1)),
            h
        };
    }
    return nullptr;
}



// Получает указатель method на способ подсчёта
// указатель integrand на подынтегральную функцию,
// интервал от a до b и 
// кол-во разбиений n
double calculateIntegral(
    double (*method)(double*),
    double (*integrand)(double), 
    double a, double b, 
    int n
) {
    double sum = 0;
    double h = (b - a) / n;
    
    for (int i = 0; i < n; i++) {
        sum += method(
            getArgs(method, integrand, a, h, i)
        );
    }
    return sum;
}



// Записывает значения erf(x) и её приближение в erf.csv
void handleExpFunction() {
    double n;
    cout << "Input number of partitions: ";
    cin >> n;

    double option;
    cout << "1 - Rectangles\n"
            "2 - Trapezoid\n"
            "3 - Simpson\n"
            "Choose method: ";
    cin >> option;
    double (*method)(double*) = getMethod(option);

    ofstream file("erf.csv");
    file << "x\terf\tprogram\n";
    cout << "X        \terf(x)    \tcalculated\tdifference\n";
    for (double x = 0; x <= 2.01; x+=0.1) {
        double calculatedErf = calculateIntegral(
            method, 
            expFunction, 
            0, x, 
            n
        ) * 2 / sqrt(M_PI);
        file << x << "\t" << 
        erf(x) << "\t" << 
        calculatedErf << 
        "\n";

        cout << x << "\t" << 
        erf(x) << "\t" << 
        calculatedErf << "\t" << 
        abs(erf(x) - calculatedErf) << 
        "\n";
    }
}

// Выводит приближённое значение пи
void handlePiFunction() {
    double option;
    cout << "1 - Rectangles\n"
            "2 - Trapezoid\n"
            "3 - Simpson\n"
            "Choose method: ";
    cin >> option;
    double (*method)(double*) = getMethod(option);

    for (int n = 8; n <= 128; n*=4) {
        cout << "n = " << n << ":\n" << calculateIntegral(
            method, 
            piApproximate, 
            0, 1, 
            n
        ) << "\n\n";
    }
}

// Третий пункт
void handleThirdFunction() {
    double n;
    cout << "Input number of partitions: ";
    cin >> n;

    double option;
    cout << "1 - Rectangles\n"
            "2 - Trapezoid\n"
            "3 - Simpson\n"
            "Choose method: ";
    cin >> option;
    double (*method)(double*) = getMethod(option);

    cout << calculateIntegral(
        method, 
        thirdFunc, 
        0, 4, 
        n
    ) << "\n";
}



// ъ
int main() {
    cout << fixed << setprecision(10);
    int option;
    cout << "Choose option (1, 2, 3): ";
    cin >> option;


    if (option == 1) {
        handleExpFunction();
        return 0;
    }

    if (option == 2) {
        handlePiFunction();
        return 0;
    }

    if (option == 3) {
        handleThirdFunction();
        return 0;
    }


    cout << "Bad option\n";
    return -1;
}