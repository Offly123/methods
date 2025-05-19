// Заголовочник с методом Гаусса

void divideRow(double*, double, int);

void subtractRow(double*, double*, int);

void swapRows(double**, int, int);

void invertMatrix(double**, int, int);

void gaussMethod(double**, int, int);





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
void invertMatrix(double** matrix, int n, int m) {
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
    invertMatrix(matrix, n, m);
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
    invertMatrix(matrix, n, m);
}
