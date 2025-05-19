#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Дифференциальное уравнение для erf(x)
double errorFunction(double x) {
    return (2 * pow(M_E, -pow(x, 2)) / sqrt(M_PI));
}

// Вывести вектор
void printErfVector(vector< pair<double, double> > vect) {
    for (auto i : vect) {
        cout << i.first << "\t" << erf(i.first) << "\t" << 
                i.second << "\t" << 
                abs(erf(i.first) - i.second) << "\n";
    }
}

// Посчитать erf(x)
double calculateErf(
    double x, int n
) {
    vector< pair<double, double> > funcValues;
    funcValues.push_back(make_pair(0, 0));
    double h = x / n;

    for (int i = 0; i < n; i++) {
        funcValues.push_back(make_pair(
            i * h, 
            h * errorFunction(i * h)
        ));
    }

    for (int i = 1; i <= n; i++) {
        funcValues.at(i) = make_pair(
            i * h,
            funcValues.at(i - 1).second + h * (errorFunction(h * (i - 1)) + errorFunction(h * i)) / 2
        );
    }

    return funcValues.back().second - 0.5;
}

double calculateFirst() {
    const int n = 10000;
    const double EPS = 1e-10;
    double x = 0.0;
    double xNew = x - calculateErf(x, n) / errorFunction(x);

    while (abs(x - xNew) > EPS) {
        x = xNew;
        xNew = xNew - calculateErf(xNew, n) / errorFunction(xNew);
    }

    return xNew;
}



// Получить коэффициенты p, q и s
double getP(double a, double b, double c) {
    return b - pow(a, 2) / 3.0;
}

double getQ(double a, double b, double c) {
    return c - a * b / 3.0 + 2.0 * pow(a / 3.0, 3);
}

double getS(double p, double q) {
    return pow(pow(p / 3.0, 3) + pow(q / 2.0, 2), 0.5);
}

void handleSecond() {
    double alpha;
    cout << "Input alpha: ";
    cin >> alpha;
    double a = 3.0;
    double b = pow(alpha, 2);
    double c = 3.0 * pow(alpha, 2);

    double p = getP(a, b, c);
    double q = getQ(a, b, c);
    double s = getS(p, q);

    // cout << "q = " << q << "\n";
    // cout << "s = " << s << "\n";
    double plus =   pow(abs(-q / 2.0 + s), 1.0 / 3.0) * ((-q / 2.0 + s) > 0 ? 1 : -1) + 
                    pow(abs(-q / 2.0 - s), 1.0 / 3.0) * ((-q / 2.0 - s) > 0 ? 1 : -1);
    // cout << "plus = " << plus << "\n";
    // cout << "tempminus = " << -q / 2.0 + s << "\n";
    // cout << "tempplus = " << -q / 2.0 - s << "\n";

    double minus =  pow(abs(-q / 2.0 + s), 1.0 / 3.0) * ((-q / 2.0 + s) > 0 ? 1 : -1) - 
                    pow(abs(-q / 2.0 - s), 1.0 / 3.0) * ((-q / 2.0 - s) > 0 ? 1 : -1);
    // cout << "minus = " << minus << "\n";

    cout << "x1 = " << plus - a / 3 << "\n";
    cout << "x3 = " << (-plus) / 2 - a / 3 << "-" << minus * sqrt(3) / 2 << "i\n";
    cout << "x2 = " << (-plus) / 2 - a / 3 << "+" << minus * sqrt(3) / 2 << "i\n";
}

double third(double a, double b, double c, double d, double x) {
    return a * pow(x, 3) + b * pow(x,2) + c * x + d;
}

double thirdDerivative(double a, double b, double c, double x) {
    return 3 * a * pow(x, 2) + 2 * b * x + c;
}

double* getSquareEquation(double a, double b, double c, double d, double x) {
    double* newCoeffs = new double[3];
    double newA = a;
    double newB = x * newA + b;
    double newC = x * newB + c;
    // cout << newA << "   " << newB << "   " << newC <<  "\n";
    newCoeffs[0] = newA;
    newCoeffs[1] = newB;
    newCoeffs[2] = newC;
    return newCoeffs;
}

void calculateSquareEquation(double a, double b, double c) {
    double D = b * b - 4 * a * c;
    if (D >= 0) {
        cout << "x1 = " << (- b + sqrt(D)) / (2 * a);
        cout << "x2 = " << (- b - sqrt(D)) / (2 * a);
        return;
    }

    cout << "x1 = " << -b / (2 * a) << "+" << sqrt(abs(D)) / (2 * a) << "i\n";
    cout << "x1 = " << -b / (2 * a) << "-" << sqrt(abs(D)) / (2 * a) << "i\n";
}

void handleThird() {
    const double EPS = 1e-10;

    double alpha;
    cout << "Input alpha: ";
    cin >> alpha;
    double a = 1;
    double b = 3.0;
    double c = pow(alpha, 2);
    double d = 3.0 * pow(alpha, 2);

    double x = 0;
    double xNew = x - third(a, b, c, d, x) / thirdDerivative(a, b, c, x);
    while(abs(x - xNew) > EPS) {
        x = xNew;
        xNew = xNew - third(a, b, c, d, xNew) / thirdDerivative(a, b, c, xNew);
    }
    cout << "x = " << x << "\n";
    double* newCoeffs = getSquareEquation(a, b, c, d, x);
    double newA = newCoeffs[0];
    double newB = newCoeffs[1];
    double newC = newCoeffs[2];
    calculateSquareEquation(newA, newB, newC);
}

int main() {
    int option;
    cout << "Input task number (1-3): ";
    cin >> option;

    if (option == 1) {
        cout << "x = " << calculateFirst() << "\n";
        return 0;
    }
    if (option == 2) {
        handleSecond();
        return 0;
    }
    if (option == 3) {
        handleThird();
        return 0;
    }

    cout << "\nBad option\n\n";

    return -1;
}
