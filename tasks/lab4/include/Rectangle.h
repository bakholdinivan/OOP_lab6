#pragma once
#include "Figure.h"


// Класс прямоугольник

// Наследуется от Figure<T> 
// Прямоугольник - это четырёхугольник с четырьмя прямыми углами (90°)
// Характеристики:
// - 4 вершины
// - Противоположные стороны равны и параллельны
// - Все углы 90°
// - Диагонали равны и делятся пополам точкой пересечения
template<Scalar T>
class Rectangle : public Figure<T> {
private:
    // Проверка, что 4 точки образуют прямоугольник
    // (противоположные стороны равны И диагонали равны)
    bool isRectangle() const {
        if (this->points.size() != 4) return false;
        
        // Вычисляем длины сторон и диагоналей
        T side1 = this->distance(this->points[0], this->points[1]);
        T side2 = this->distance(this->points[1], this->points[2]);
        T side3 = this->distance(this->points[2], this->points[3]);
        T side4 = this->distance(this->points[3], this->points[0]);
        
        T diag1 = this->distance(this->points[0], this->points[2]);
        T diag2 = this->distance(this->points[1], this->points[3]);
        
        // Проверка с погрешностью для вещественных типов
        if constexpr (std::is_floating_point_v<T>) {
            const T epsilon = static_cast<T>(1e-6);
            // Противоположные стороны равны И диагонали равны
            return std::abs(side1 - side3) <= epsilon &&
                   std::abs(side2 - side4) <= epsilon &&
                   std::abs(diag1 - diag2) <= epsilon;
        } else {
            return side1 == side3 && side2 == side4 && diag1 == diag2;
        }
    }
    
public:
   
    // Конструктор дефолтный

    // Создаёт прямоугольник 2x1 в начале координат
    // Вершины: (0,0), (2,0), (2,1), (0,1)
    Rectangle() {
        // Резервируем память для 4 точек 
        this->points.reserve(4);
        
        // Создаём вершины прямоугольника
        // make_unique - создаёт unique_ptr, владеющий новым объектом Point
        // T{} - инициализация нулём (для int будет 0, для double будет 0.0)
        this->points.push(std::make_unique<Point<T>>(T{}, T{}));
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(2), T{}));
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(2), static_cast<T>(1)));
        this->points.push(std::make_unique<Point<T>>(T{}, static_cast<T>(1)));
        
        // Упорядочиваем точки против часовой стрелки
        // Это необходимо для корректного вычисления площади
        this->sortPoints();
    }
    
   
    // Конструктор из массива точек

    // Создаёт прямоугольник из заданных пользователем вершин
    // pts - указатель на массив точек
    // n - количество точек (должно быть 4)
    Rectangle(const Point<T>* pts, size_t n) {
        // Валидация: прямоугольник должен иметь ровно 4 вершины
        if (n != 4) {
            throw std::invalid_argument("Прямоугольник должен иметь 4 вершины");
        }
        
        this->points.reserve(4);
        
        // Копируем точки из массива в наш динамический массив
        for (size_t i = 0; i < n; ++i) {
            // pts[i] - обращение к i-той точке в массиве
            this->points.push(std::make_unique<Point<T>>(pts[i]));
        }
        
        // Упорядочиваем вершины для корректных вычислений
        this->sortPoints();
        
        // ПРОВЕРКА: точки должны образовывать прямоугольник (вписан в круг)
        if (!isRectangle()) {
            throw std::invalid_argument("Точки не образуют прямоугольник");
        }
    }
    

    // Переопределяем виртуальные методы базового класса

    // override - ключевое слово C++11, явно указывает что метод переопределяет
    // виртуальный метод базового класса
    
    // Вычисление площади прямоугольника
    // Использует формулу Гаусса из базового класса
    T area() const override { 
        return this->calculateArea(); 
    }
    
    // Вычисление геометрического центра
    // Для прямоугольника центр = среднее арифметическое всех вершин
    Point<T> center() const override { 
        return this->calculateCenter(); 
    }
    
  
    // Выводим информацию о прямоугольнике

    // Вызывается при использовании оператора << 

    void print(std::ostream& os) const override {
        os << "Rectangle: ";
        
        // Выводим все вершины
        for (size_t i = 0; i < this->points.size(); ++i) {
            os << *this->points[i];  // Разыменовываем unique_ptr для доступа к Point
            if (i < this->points.size() - 1) os << " ";  // Пробел между точками
        }
    }
    
    // Возвращает строковое название типа фигуры
    const char* getType() const override { 
        return "Rectangle"; 
    }
};