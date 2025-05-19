// Заголовочник для подсчёта интеграла

#include <splines.h>
#include <iostream>

// Получают
// подынтегральную функцию
// интервал от a до b
// число разбиений
double getRectangleArea(
    double (*)(double),
    double, double,
    int
);
double getTrapezoidArea(
    double (*)(double),
    double, double,
    int
);
double getParabolaArea(
    double (*)(double),
    double, double,
    int
);
double getSplineArea(
    double (*)(double),
    double, double,
    int
);

// Возвращает указатель на выбранный метод (функцию из трёх выше)
double (*getMethod(int))(
    double (*)(double),
    double, double,
    int
);


// Получает указатель method на способ подсчёта
// указатель integrand на подынтегральную функцию,
// интервал от a до b и 
// кол-во разбиений n
double calculateIntegral(
    double (*method)(
        double (*)(double),
        double, double,
        int
    ),
    double (*integrand)(double), 
    double a, double b, 
    int n
);





// Считает площадь прямоугольника по двум сторонам
// args == [f(x1), h]
double getRectangleArea(
    double (*integrand)(double),
    double a, double b,
    int n
) {
    double h = (b - a) / n;
    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += integrand(a + h * i) * h;
    }

    return sum;
}

// Считает площадь трапеции по двум сторонам a, b и высоте h
// args == [f(x0), f(x1), h]
double getTrapezoidArea(
    double (*integrand)(double),
    double a, double b,
    int n
) {
    double h = (b - a) / n;
    double sum = 0;

    for (int i = 0; i < n - 1; i++) {
        sum += (integrand(a + h * i) + integrand(a + h * (i + 1))) * h / 2;
    }

    return sum;
}

// Считает площадь под параболой по трём точкам
// args == [f(x0), f(x1), f(x2), h]
double getParabolaArea(
    double (*integrand)(double),
    double a, double b,
    int n
) {
    double h = (b - a) / n;
    double sum = 0;

    for (int i = 1; i < n - 1; i+=2) {
        sum += integrand(a + h * (i - 1)) +
               integrand(a + h * (i + 0)) * 4 +
               integrand(a + h * (i + 1));
    }


    return sum * h / 3;
}

double getVzorvanniySum(
    double (*integrand)(double),
    double a, double b,
    int n,
    double* cCoefficients
) {
    double h = (b - a) / n;
    return h * (5 * integrand(a) + 
                13 * integrand(a + h) +
                13 * integrand(b - h) +
                5 * integrand(b)) / 12 -
                pow(h, 3) * (cCoefficients[1] + cCoefficients[n - 1]) / 36;
}

double getSplineArea(
    double (*integrand)(double),
    double a, double b,
    int n
) {
    double** matrix = getMatrix(integrand, a, b, n);
    gaussMethod(matrix, n, n + 1);
    // printMatrix(matrix, n, n + 1);
    double* mValues = getMValues(matrix, n, n + 1);
    double** abcd = getABCDMatrix(
        integrand, mValues, a, b, n
    );

    double* cCoefficients = getCCoefficients(abcd, n);
    // printVector(cCoefficients, n);
    // std::cout << "\n\n\n";

    double sum = 0;
    double h = (b - a) / n;

    for (int i = 2; i < n - 2; i++) {
        sum += integrand(a + h * i);
    }
    // std::cout << "sum = " << sum << "\n";
    // std::cout << getVzorvanniySum(integrand, a, b, n, cCoefficients);
    sum = sum * h + getVzorvanniySum(integrand, a, b, n, cCoefficients);


    return sum;
}



// Возвращает указатель на выбранный метод 
double (*getMethod(int option))(
    double (*)(double),
    double, double,
    int
) {
    switch (option) {
        case 1: {
            return &getRectangleArea;
        }
        case 2: {
            return &getTrapezoidArea;
        }
        case 3: {
            return &getParabolaArea;
        }
        case 4: {
            return &getSplineArea;
        }
        default: {
            return nullptr;
        }
    }
}



// Получает указатель method на способ подсчёта
// указатель integrand на подынтегральную функцию,
// интервал от a до b и 
// кол-во разбиений n
double calculateIntegral(
    double (*method)(
        double (*)(double),
        double, double,
        int
    ),
    double (*integrand)(double), 
    double a, double b, 
    int n
) {
    return method(integrand, a, b, n);
}   