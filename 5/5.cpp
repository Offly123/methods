#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <vector>
#include <fstream>
#include <string>


using namespace std;


double calculateFunction(double x) {
    return 1 / (1 + 25 * pow(x, 2));
}


void printVector(vector<double> vect) {
    for (auto x : vect) {
        cout << x << endl;
    }
}


void printTable(map<double, double> table) {
    for (auto& pair : table) {
        cout << setw(12) << pair.first << "   " <<
        pair.second << endl;
    }
    cout << endl;
}



void tableInFile(map<double, double> table, string filename) {
    ofstream values(filename, ios::app);
    for (auto& pair : table) {
        values << pair.first << "\t" << pair.second << "\n";
    }
    values << "\n";
    values.close();
}


// Возвращает значение X одного чебышевского узла
double getChebishevNode(int k, int n, const double A, const double B) {
    return  (A + B) / 2 + 
            ((B - A) * cos(M_PI * (2 * k - 1) / (2 * n))) / 2;
}


// Возвращает вектор значений Х из таблицы
vector<double> getXValues(map<double, double> table) {
    vector<double> Xvalues;

    for (auto dot: table) {
        Xvalues.push_back(dot.first);
    }

    return Xvalues;
}


// Возвращает вектор значений Y из таблицы
vector<double> getYValues(map<double, double> table) {
    vector<double> Yvalues;

    for (auto dot: table) {
        Yvalues.push_back(dot.second);
    }

    return Yvalues;
}


// Возвращает таблицу значений для равноотстоящих узлов
map<double, double> getEqualDistant(
    int n, 
    const double A, 
    const double B
) {
    map<double, double> table;


    for (double i = -1.0; i <= 1.0; i += (B - A) / n) {
        table.insert({i, calculateFunction(i)});
    }


    return table;
}


// Возвращает таблицу значений для чебышевских узлов
map<double, double> getChebishevNodTable(
    int n, 
    const double A, 
    const double B
) {
    map<double, double> table;


    for (int k = 1; k <= n; k++) {
        double node = getChebishevNode(k, n, A, B);
        table.insert({
            node, 
            calculateFunction(node)
        });
    }

    
    return table;
}


// Вычисляет один член полинома Лагранжа
double getLagranzhMember(vector<double> vect, double x, double i) {
    double mult = 1.0;
    int n = vect.size();


    for (int j = 0; j < n; j++) {
        if (j == i) {
            continue;
        }
        mult *= (x - vect[j]) / (vect[i] - vect[j]);
    }


    return mult;
}


// Строит таблицу значений для полинома Лагранжа
map<double, double> getLagranzhTable(
    map<double, double> table,
    const int n,
    const double A,
    const double B
) {
    map<double, double> newTable;
    map<double, double> hehe = getEqualDistant(n * 2, A, B);
    vector<double>xvalues = getXValues(table);
    vector<double>yvalues = getYValues(table);


    double sum;
    for (int k = 0; k <= n * 2; k++) {
        sum = 0;
        for (int i = 0; i <= n * 2; i++) {
            sum += yvalues[i] * getLagranzhMember(
                                    xvalues,
                                    xvalues[k],
                                    i
                                );
        }
        newTable[xvalues[k]] = sum;
    }


    return newTable;
}


// Скобочка закрывается
int main() {
    ofstream hehe("real.txt");
    hehe.close();
    ofstream hehe2("equal.txt");
    hehe2.close();
    ofstream hehe3("chebishev.txt");
    hehe3.close();
    const double A = -1.0;
    const double B =  1.0;


    for (int n = 10; n <= 20; n+=5) {
        map<double, double> tableEqual = getEqualDistant(n, A, B);
        map<double, double> tableChebishev = getChebishevNodTable(n, A, B);

        cout << "N = " << n << "\n";


        // Вывод значений функции
        printTable(tableEqual);
        tableInFile(tableEqual, "equal.txt");
        printTable(tableChebishev);
        tableInFile(tableChebishev, "chebishev.txt");


        // Вывод полинома лагранжа для 
        // равноотстоящих точек (в консоль и файл)
        map<double, double> temp = getEqualDistant(n * 2, A, B);
        map<double, double> lagranzhEqual = getLagranzhTable(temp, n, A, B);
        printTable(lagranzhEqual);
        tableInFile(lagranzhEqual, "equal.txt");


        // Вывод полинома лагранжа для 
        // чебышевских точек (в консоль и файл)
        temp = getChebishevNodTable(n * 2, A, B);
        map<double, double> lagranzhChebishev = getLagranzhTable(temp, n, A, B);
        printTable(lagranzhChebishev);
        tableInFile(lagranzhChebishev, "chebishev.txt");


        cout << "\n\n\n";
    }


    return 0;
}