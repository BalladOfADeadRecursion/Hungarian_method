#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int INF = 1000 * 1000 * 1000;

int n;
vector<vector<int>> a;  // Матрица стоимости задач
vector<int> xy, yx;     // Вершины в паросочетаниях
vector<char> vx, vy;     // Метки для вершин в паросочетаниях
vector<int> minrow, mincol;  // Минимальные значения в строках и столбцах

//Функция для реализации метода северо - западного угла
void northwestCornerMethod(vector<vector<int>>& costs, vector<vector<int>>& allocation) {
    int numRows = costs.size() - 1; // Изменено на размеры матрицы затрат
    int numCols = costs[0].size() - 1; // Изменено на размеры матрицы затрат

    int i = 0; // Индекс строки
    int j = 0; // Индекс столбца

    while (i < numRows && j < numCols) {
        // Находим минимум между доступным количеством в строке и столбце
        int quantity = min(costs[i][numCols], costs[numRows][j]);

        // Заполняем базис этим минимумом
        allocation[i][j] = quantity;

        // Обновляем доступные количества
        costs[i][numCols] -= quantity;
        costs[numRows][j] -= quantity;

        // Если строка полностью заполнена, переходим к следующей строке
        if (costs[i][numCols] == 0) {
            i++;
        }

        // Если столбец полностью заполнен, переходим к следующему столбцу
        if (costs[numRows][j] == 0) {
            j++;
        }
    }
}

// Функция для попытки увеличения паросочетания
bool dotry(int i) {
    if (vx[i]) return false;  // Если вершина уже посещена, возвращаем false
    vx[i] = true;  // Помечаем вершину как посещенную
    for (int j = 0; j < n; ++j)
        if (a[i][j] - minrow[i] - mincol[j] == 0)
            vy[j] = true;  // Помечаем соответствующую вершину в другой части
    for (int j = 0; j < n; ++j)
        if (a[i][j] - minrow[i] - mincol[j] == 0 && yx[j] == -1) {
            xy[i] = j;
            yx[j] = i;
            return true;  // Нашли свободную вершину и установили отметку
        }
    for (int j = 0; j < n; ++j)
        if (a[i][j] - minrow[i] - mincol[j] == 0 && dotry(yx[j])) {
            xy[i] = j;
            yx[j] = i;
            return true;  // Рекурсивно пробуем найти другую вершину
        }
    return false;  // Если не нашли подходящую вершину, возвращаем false
}

int main() {
    cout << "Enter the size of the matrix: ";
    cin >> n;  // Вводим размер матрицы

    a.resize(n, vector<int>(n));  // Инициализируем матрицу стоимости

    cout << "Enter the matrix elements:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> a[i][j];  // Заполняем матрицу стоимости

    // Вывод введенной матрицы
    cout << "Entered Matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << a[i][j] << " ";
        cout << endl;
    }

    mincol.assign(n, INF);
    minrow.assign(n, INF);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            minrow[i] = min(minrow[i], a[i][j]);  // Находим минимальное значение в каждой строке
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
            mincol[j] = min(mincol[j], a[i][j] - minrow[i]);  // Вычисляем минимальные значения в столбцах

    xy.assign(n, -1);
    yx.assign(n, -1);
    for (int c = 0; c < n;) {
        vx.assign(n, 0);
        vy.assign(n, 0);
        int k = 0;
        for (int i = 0; i < n; ++i)
            if (xy[i] == -1 && dotry(i))
                ++k;  // Пытаемся увеличить паросочетание
        c += k;
        if (k == 0) {
            int z = INF;
            for (int i = 0; i < n; ++i)
                if (vx[i])
                    for (int j = 0; j < n; ++j)
                        if (!vy[j])
                            z = min(z, a[i][j] - minrow[i] - mincol[j]);  // Находим минимальное не покрытое ребро
            for (int i = 0; i < n; ++i) {
                if (vx[i]) minrow[i] += z;
                if (vy[i]) mincol[i] -= z;  // Пересчитываем метки для вершин
            }
        }
    }

    int ans = 0;
    for (int i = 0; i < n; ++i)
        ans += a[i][xy[i]];  // Вычисляем общую стоимость

    // Вывод минимальной стоимости
    cout << "Minimum Cost: " << ans << endl;

    // Вывод назначенных работ
    /*cout << "Assigned Jobs: ";
    for (int i = 0; i < n; ++i)
        cout << "Job " << xy[i] + 1 << " assigned to Worker " << i + 1 << ", ";
    cout << endl;*/

    return 0;
}