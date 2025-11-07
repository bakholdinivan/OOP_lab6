#pragma once
#include "Figure.h"


// КЛАСС ТРАПЕЦИЯ

// Наследование от Figure<T> 
// Трапеция - четырёхугольник, у которого ОДНА пара сторон параллельна
// Характеристики:
// - 4 вершины
// - Одна пара противоположных сторон параллельна (основания)
// - Другая пара сторон не параллельна (боковые стороны)

template<Scalar T>
class Trapezoid : public Figure<T> {
public:

    // Конструктор по дефолту

    // Создаёт трапецию с горизонтальными основаниями
    // Нижнее основание: от (0,0) до (4,0) - длина 4
    // Верхнее основание: от (1,2) до (3,2) - длина 2
    // Высота трапеции: 2
    Trapezoid() {
        // Резервируем память для 4 точек
        this->points.reserve(4);
        
        // Задаём вершины трапеции по часовой стрелке, начиная с левого нижнего угла
        // T{} - value initialization (для числовых типов = 0)
        this->points.push(std::make_unique<Point<T>>(T{}, T{}));                    // (0, 0)
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(4), T{}));      // (4, 0)
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(3), static_cast<T>(2)));  // (3, 2)
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(1), static_cast<T>(2)));  // (1, 2)
        
        // Упорядочиваем точки против часовой стрелки относительно центра
        // Это критически важно для формулы Гаусса (shoelace formula)
        this->sortPoints();
    }
    

    // Конструктор из массива точек

    // Создаёт трапецию из пользовательских координат
    // pts - C-style массив точек (const Point<T>*)
    // n - количество элементов в массиве
    Trapezoid(const Point<T>* pts, size_t n) {
        // Валидация входных данных
        // std::invalid_argument - стандартное исключение для некорректных аргументов
        if (n != 4) {
            throw std::invalid_argument("Трапеция должна иметь 4 вершины");
        }
        
        this->points.reserve(4);
        
        // Копируем точки из входного массива
        // Каждая точка оборачивается в unique_ptr для автоматического управления памятью
        for (size_t i = 0; i < n; ++i) {
            // Конструктор Point вызывается с копированием из pts[i]
            this->points.push(std::make_unique<Point<T>>(pts[i]));
        }
        
        // Сортировка обеспечивает правильный порядок обхода вершин
        this->sortPoints();
    }
    
 
    // Переопределяем виритульные методы

    // override - ключевое слово C++11 для явного указания переопределения
    // Компилятор проверит, что метод действительно виртуальный в базовом классе
    
    // Вычисление площади трапеции

    // Мы используем универсальную формулу Гаусса, работающую для любого многоугольника
    T area() const override { 
        return this->calculateArea(); 
    }
    
    // Вычисление геометрического центра (центроида)
    // Для трапеции центр находится на средней линии
    // Вычисляется как среднее арифметическое координат всех вершин
    Point<T> center() const override { 
        return this->calculateCenter(); 
    }

    // Вывод информации


    void print(std::ostream& os) const override {
        os << "Trapezoid: ";  // Название типа фигуры
        
        // Перебираем все вершины и выводим их координаты
        for (size_t i = 0; i < this->points.size(); ++i) {
            // this->points[i] - это unique_ptr<Point<T>>
            // *this->points[i] - разыменование, получаем Point<T>&
            // Затем вызывается operator<< для Point<T>
            os << *this->points[i];
            
           
            if (i < this->points.size() - 1) os << " ";
        }
    }
    
    // Возвращает имя типа фигуры как C-строку
    // Используется для идентификации типа в runtime (RTTI - Run-Time Type Information)
    const char* getType() const override { 
        return "Trapezoid"; 
    }
};