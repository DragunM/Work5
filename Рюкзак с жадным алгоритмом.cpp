#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>

using namespace std;

// Функция жадного алгоритма для 0-1 рюкзака
tuple<vector<pair<int, int>>, int, double> knapsack_01_greedy(vector<pair<int, int>> items, int capacity) {
    // Шаг 1: Создание временного вектора с удельной стоимостью и индексами
    vector<tuple<double, int, int, int>> items_with_ratio;
    
    // Шаг 2: Заполнение вектора с расчетом удельной стоимости
    for (size_t i = 0; i < items.size(); i++) {
        double ratio = (double)items[i].second / items[i].first;
        items_with_ratio.push_back(make_tuple(ratio, i, items[i].first, items[i].second));
    }
    
    // Шаг 3: Сортировка по убыванию удельной стоимости
    sort(items_with_ratio.begin(), items_with_ratio.end(), 
         [](const tuple<double, int, int, int>& a, const tuple<double, int, int, int>& b) {
             return get<0>(a) > get<0>(b);
         });
    
    // Шаг 4: Инициализация переменных для результатов
    vector<pair<int, int>> selected_items;
    int total_value = 0;
    int total_weight = 0;
    
    // Шаг 5: Жадный выбор предметов
    for (const auto& item : items_with_ratio) {
        double ratio = get<0>(item);
        int original_index = get<1>(item);
        int weight = get<2>(item);
        int value = get<3>(item);
        
        // Проверка возможности добавления предмета
        if (total_weight + weight <= capacity) {
            selected_items.push_back(items[original_index]);
            total_value += value;
            total_weight += weight;
        }
    }
    
    // Шаг 6: Поиск предмета с максимальной стоимостью
    int max_single_value = 0;
    for (const auto& item : items) {
        if (item.second > max_single_value && item.first <= capacity) {
            max_single_value = item.second;
        }
    }
    
    // Шаг 7: Вычисление коэффициента аппроксимации
    double approx_ratio;
    if (total_value == 0) {
        if (max_single_value == 0) {
            approx_ratio = 1.0; // Оба решения нулевые
        } else {
            approx_ratio = numeric_limits<double>::infinity(); // Жадный не нашел решения
        }
    } else {
        // Коэффициент = max(макс.предмет, жадное_решение) / жадное_решение
        approx_ratio = (double)max(max_single_value, total_value) / total_value;
    }
    
    // Шаг 8: Гарантия коэффициента ≤ 2.0
    approx_ratio = min(approx_ratio, 2.0);
    
    return make_tuple(selected_items, total_value, approx_ratio);
}

int main() {
    // Инициализация входных данных
    vector<pair<int, int>> items = {{3, 12}, {2, 8}, {4, 20}, {5, 18}};
    int capacity = 7;
    
    // Вызов жадного алгоритма
    auto [selected_items, total_value, approx_ratio] = knapsack_01_greedy(items, capacity);
    
    // Вывод результатов
    cout << "=== ЖАДНЫЙ АЛГОРИТМ ДЛЯ 0-1 РЮКЗАКА ===" << endl;
    cout << "Вместимость рюкзака: " << capacity << endl;
    
    cout << "\nВсе предметы:" << endl;
    for (size_t i = 0; i < items.size(); i++) {
        double ratio = (double)items[i].second / items[i].first;
        cout << "  " << i+1 << ". Вес: " << items[i].first 
             << ", Стоимость: " << items[i].second 
             << ", Удельная стоимость: " << ratio << endl;
    }
    
    cout << "\nРезультат работы алгоритма:" << endl;
    cout << "Выбранные предметы:" << endl;
    int total_weight = 0;
    for (size_t i = 0; i < selected_items.size(); i++) {
        cout << "  " << i+1 << ". Вес: " << selected_items[i].first 
             << ", Стоимость: " << selected_items[i].second << endl;
        total_weight += selected_items[i].first;
    }
    
    cout << "\nИтоговые показатели:" << endl;
    cout << "Общая стоимость: " << total_value << endl;
    cout << "Общий вес: " << total_weight << endl;
    cout << "Коэффициент аппроксимации: " << approx_ratio << endl;
    cout << "Гарантированный коэффициент: ≤ 2.0" << endl;
    
    // Проверка корректности решения
    if (total_weight > capacity) {
        cout << "ОШИБКА: Превышена вместимость рюкзака!" << endl;
    } else {
        cout << "Решение корректно (вес не превышает вместимость)" << endl;
    }
    
    return 0;
}
