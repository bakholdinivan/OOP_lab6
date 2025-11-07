#pragma once
#include <concepts>      // Библиотека для концептов C++20
#include <iostream>
#include <cmath>


// КОНЦЕПТ (CONCEPT) - ограничение на тип шаблона

// Scalar проверяет, что тип T является скалярным (int, float, double, char и т.д.)
// std::is_scalar_v - это метафункция из стандартной библиотеки <type_traits>

template<typename T>
concept Scalar = std::is_scalar_v<T>;


// Шаблонная структура точки

// T - тип координат, должен удовлетворять концепту Scalar
// Если попытаться создать Point<std::string>, компилятор выдаст понятную ошибку
template<Scalar T>
struct Point {
    T x;  // Координата по оси X
    T y;  // Координата по оси Y
    
    // Конструктор по умолчанию - инициализирует координаты нулями
    // T{} - это value initialization (для числовых типов даёт 0)
    Point() : x(T{}), y(T{}) {}
    
    // Конструктор с параметрами - задаёт конкретные координаты
    Point(T x, T y) : x(x), y(y) {}
    

    // Перегружаем вывод в поток

    // friend - функция получает доступ к приватным членам класса
    // Позволяет писать: std::cout << point;
    friend std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;  // Возвращаем поток для цепочки операций: cout << p1 << p2;
    }
    

    // Перегружаем теперь ввод

    // Позволяет писать: std::cin >> point;
    // Ожидается ввод двух чисел через пробел: x y
    friend std::istream& operator>>(std::istream& is, Point<T>& p) {
        is >> p.x >> p.y;
        return is;
    }
    
    // Оператор сравнения точек

    bool operator==(const Point<T>& other) const {
        // constexpr if - условная компиляция (проверка на этапе компиляции)
        // Компилируется только одна ветка в зависимости от типа T
        if constexpr (std::is_floating_point_v<T>) {
            // Для вещественных чисел (float, double) используем сравнение с погрешностью
            // Из-за ошибок округления нельзя сравнивать float/double через ==
            const T epsilon = static_cast<T>(1e-6);  // Погрешность
            return std::abs(x - other.x) < epsilon && 
                   std::abs(y - other.y) < epsilon;
        } else {
            // Для целых чисел (int, long и т.д.) используем точное сравнение
            return x == other.x && y == other.y;
        }
    }
};