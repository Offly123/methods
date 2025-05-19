#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

void printVector(vector< pair<double, double> > vect) {
    for (auto i : vect) {
        cout << i.first << " " << i.second << "\n";
    }
}

void vectorInCSV(vector< pair<double, double> > vect) {
    ofstream file("dots.csv");

    file << "x\ty\n";
    for (auto i : vect) {
        file << i.first << "\t" << i.second << "\n";
    }

    file.close();
}


double triangleFunc(double x, double n) {
    if (x < n) {
        return 10 * x / n;
    }
    return 10 * (x - 20) / (n - 20);
}

double secondFunc(double x, double n) {
    return sqrt(11 - n * pow(sin(x), 2) );
}

double circleFunc(double x, double n) {
    return sqrt(pow(n, 2) - pow(x, 2));
}

double (*getFunction(double option))(double, double) {
    if (option == 1) {
        return &triangleFunc;
    }
    if (option == 2) {
        return &secondFunc;
    }
    if (option == 3) {
        return &circleFunc;
    }
    return nullptr;
}



double getRadiusSquared(double x, double y) {
    return pow(x, 2) + pow(y, 2);
}

double polarFuncSquared(double phi, double n) {
    return (11 + n) * pow(cos(phi), 2) + 
           (11 - n) * pow(sin(phi), 2);
}

double getPhiByXY(double x, double y) {
    if (x > 0) {
        return atan(y / x);
    }
    if (x < 0) {
        return M_PI + atan(y / x);
    }
    if (y > 0) {
        return M_PI / 2;
    }
    if (y < 0) {
        return - M_PI / 2;
    }
    return 0;
}



double generateDouble(
    double min,
    double max
) {
    return (double)rand() / RAND_MAX * (max - min) + min;
}

pair<double, double> getRandomDot(
    double minx, double maxx, 
    double miny, double maxy
) {
    return pair<double, double>(
        generateDouble(minx, maxx), 
        generateDouble(miny, maxy)
    );
}

vector< pair<double, double> > generateDotList(
    double minx, double maxx, 
    double miny, double maxy,
    int n
) {
    vector< pair<double, double> > dotList;

    for (int i = 0; i < n; i++) {
        dotList.push_back(getRandomDot(
            minx, maxx,
            miny, maxy
        ));
    }

    return dotList;
}



double calculateDotsInside(
    vector< pair<double, double> > dotList,
    int option, int variant
) {
    double (*function)(double, double) = getFunction(option);
    double m = 0;
    for (auto i : dotList) {
        if (function(i.first, variant) > i.second) {
            m++;
        }
    }

    return m;
}

double calculateDotsCircle(
    vector< pair<double, double> > dotList,
    int variant
) {
    double m = 0;
    for (auto i : dotList) {
        if (pow(i.first, 2) + pow(i.second, 2) <= pow(variant, 2)) {
            m++;
        }
    }
    return m;
}

double calculatePolar(
    vector< pair<double, double> > dotList,
    int variant
) {
    double m = 0;
    for (auto i : dotList) {
        if (
            getRadiusSquared(i.first, i.second) < 
            polarFuncSquared(getPhiByXY(i.first, i.second), variant)
        ) {
            m++;
        }
    }

    return m;
}




int main() {
    srand(time(0));
    cout << fixed << setprecision(10);

    int n, variant, option;
    double minx, maxx, miny, maxy;
    cout << "Input variant: ";
    cin >> variant;
    cout << "Input option (1, 2, 3, 4): ";
    cin >> option;
    cout << "Input minx: ";
    cin >> minx;
    cout << "Input maxx: ";
    cin >> maxx;
    cout << "Input miny: ";
    cin >> miny;
    cout << "Input maxy: ";
    cin >> maxy;
    cout << "Input number of dots: ";
    cin >> n;

    vector< pair<double,double> > dotList = generateDotList(
        minx, maxx, 
        miny, maxy,
        n
    );

    vectorInCSV(dotList);

    double m;
    switch (option) {
        case 1: {
            // x 0 20
            // y 0 10
            m = calculateDotsInside(dotList, option, variant);
            cout << "s = " << m / (double)n * (maxx - minx) * (maxy - miny) << "\n";

            break;
        }
        case 2: {
            // x 0    5
            // y 0 ~3.5
            m = calculateDotsInside(dotList, option, variant);
            cout << "s = " << m / (double)n * (maxx - minx) * (maxy - miny) << "\n";

            break;
        }
        case 3: {
            // x -5 5
            // y -5 5
            m = calculateDotsCircle(dotList, variant);
            cout << "pi = " << 4.0 * m / n << "\n";
            cout << "eps = " << abs(3.14159265358979 - 4.0 * m / n) << "\n";

            break;
        }
        case 4: {
            // x   -4   4
            // y -2.6 2.6
            m = calculatePolar(dotList, variant);
            
            cout << "s = " << m / (double)n * (maxx - minx) * (maxy - miny) << "\n";

            break;
        }
        default: {
            cout << "Bad option \n";
            return -1;
        }
    }

    cout << "m = " << m << "\nn = " << n << "\n";
    cout << "Max difference: " << 10.0 * sqrt(((m / n) - pow((m / n), 2)) / n) << "\n";

    return 0;
}