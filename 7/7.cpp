#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

double errorFunction(double x) {
    return (2 * pow(M_E, -pow(x, 2)) / sqrt(M_PI));
}

double rabbitFunc(double a, double r, double f) {
    return 2 * r - a * r * f;
}

double foxFunc(double a, double r, double f) {
    return -f + a * r * f;
}



void printFirstVector(vector< pair<double, double> > vect) {
    for (auto i : vect) {
        cout << i.first << "\t" << erf(i.first) << "\t" << 
                i.second << "\t" << 
                abs(erf(i.first) - i.second) << "\n";
    }
}

// void erfInFile(vector< pair<double, double> > vect) {
//     ofstream file("erf.csv");

//     for (auto i : vect) {
//         file << i.first << "\t" << i.second << "\t" << erf(i.first) << "\n";
//     }

//     file.close();
// }



void printSeondVector(vector< vector<double> > vect) {
    // if (vect.back().at(1) > 10 || vect.back().at(2) > 10) {
    //     return;
    // }
    for (auto i : vect) {
        cout << i.at(0) << "\t" << 
                i.at(1) << "\t" << 
                i.at(2) << "\n";
    }
    cout << "\n\n";
}

void rabbitFoxInFile(vector< vector<double> > vect, int n) {
    string filename = "rabbitFox";
    filename += n;
    filename += ".csv";
    ofstream file(filename);

    for (auto i : vect) {
        file << i.at(0) << "\t" << i.at(1) << "\t" << i.at(2) << "\n";      
    }

    file.close();
}



vector< pair<double, double> > calculateFirst(
    int n
) {
    vector< pair<double, double> > funcValues;
    funcValues.push_back(make_pair(0, 0));
    double h = 2.0 / n;

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

    return funcValues;
}

vector< vector<double> > calculateSecond(
    int n, bool flag
) {
    vector< vector<double> > rabbitFoxValues;
    rabbitFoxValues.resize(1);
    double h = 0.05;
    double a = 0.01;
    double r, f;
    cout << "Input r: ";
    cin >> r;
    cout << "Input f: ";
    cin >> f;
    rabbitFoxValues.at(0).push_back(0);
    rabbitFoxValues.at(0).push_back(r);
    rabbitFoxValues.at(0).push_back(f);

    int i = 1;
    for ( ; i < n; i++) {
        rabbitFoxValues.push_back({
            (double)i, 
            rabbitFoxValues.at(i - 1).at(1) + h * rabbitFunc(
                a, 
                rabbitFoxValues.at(i - 1).at(1), 
                rabbitFoxValues.at(i - 1).at(2)
            ), 
            rabbitFoxValues.at(i - 1).at(2) + h * foxFunc(
                a, 
                rabbitFoxValues.at(i - 1).at(1), 
                rabbitFoxValues.at(i - 1).at(2)
            )
        });

        if (flag) {
            if (
                (rabbitFoxValues.at(i).at(1) < 1) ||
                (rabbitFoxValues.at(i).at(2) < 1)
            ) {
                break;
            }
            continue;
        }

        if (
            (rabbitFoxValues.at(i).at(1) < 1) &&
            (rabbitFoxValues.at(i).at(2) < 1)
        ) {
            break;
        }
    }


    for (int j = 1; j < i; j++) {
        rabbitFoxValues.at(j).at(1) = rabbitFoxValues.at(j - 1).at(1) +
            (h / 2) * (rabbitFunc(
                a,
                rabbitFoxValues.at(j - 1).at(1),
                rabbitFoxValues.at(j - 1).at(2)
            ) + rabbitFunc(
                a,
                rabbitFoxValues.at(j).at(1),
                rabbitFoxValues.at(j).at(2)
            ));
        cout << "j = " << j << " hehe\n";
        rabbitFoxValues.at(j).at(2) = rabbitFoxValues.at(j - 1).at(2) +
            (h / 2) * (foxFunc(
                a,
                rabbitFoxValues.at(j - 1).at(1),
                rabbitFoxValues.at(j - 1).at(2)
            ) + foxFunc(
                a,
                rabbitFoxValues.at(j).at(1),
                rabbitFoxValues.at(j).at(2)
            ));

        if (flag) {
            if (
                (rabbitFoxValues.at(i).at(1) < 1) ||
                (rabbitFoxValues.at(i).at(2) < 1)
            ) {
                break;
            }
            continue;
        }

        if (
            (rabbitFoxValues.at(i).at(1) < 1) &&
            (rabbitFoxValues.at(i).at(2) < 1)
        ) {
            break;
        }
    }

    

    return rabbitFoxValues;
}


int main() {
    cout << fixed << setprecision(10);
    int option;
    cout << "Input option(1, 2): ";
    cin >> option;
    switch (option) {
        case 1: {
            vector< pair<double,double> > funcValues;
            int n;
            cout << "Input number of partitions: ";
            cin >> n;


            funcValues = calculateFirst(n);


            printFirstVector(funcValues);
            // erfInFile(funcValues);


            break;
        }
        case 2: {
            vector< vector<double> > rabbitFoxValues;
            int n;
            cout << "Input number of days: ";
            cin >> n;
            bool flag;
            cout << "0 other 1 equal: ";
            cin >> flag;

            rabbitFoxValues = calculateSecond(n, !flag);


            printSeondVector(rabbitFoxValues);
            // rabbitFoxInFile(rabbitFoxValues, n);


            break;
        }
        default: {
            cout << "\nBad option\n\n";
            return -1;
        }
    }




    return 0;
}

// 1000 1000 оба 

// 3 3 лисы умирают
