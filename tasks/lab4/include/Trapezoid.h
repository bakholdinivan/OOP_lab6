#pragma once
#include "Figure.h"


// КЛАСС ТРАПЕЦИЯ

// Наследование от Figure<T> 
// Трапеция - четырёхугольник, у которого ОДНА пара сторон параллельна
// Характеристики:
// - 4 вершины
// - Одна пара противоположных сторон параллельна (основания)
// - Другая пара сторон не параллельна (боковые стороны)
// - По заданию: должна быть вписана в круг (⟺ равнобедренная)

template<Scalar T>
class Trapezoid : public Figure<T> {
private:
    // Проверка, что трапеция равнобедренная (⟺ вписана в круг)
    // Математический факт: равнобедренная трапеция имеет равные диагонали
    bool isIsoscelesTrapezoid() const {
        if (this->points.size() != 4) return false;
        
        // Вычисляем длины диагоналей
        T diag1 = this->distance(this->points[0], this->points[2]);
        T diag2 = this->distance(this->points[1], this->points[3]);
        
        // Проверяем равенство диагоналей (с погрешностью для вещественных типов)
        if constexpr (std::is_floating_point_v<T>) {
            const T epsilon = static_cast<T>(1e-6);
            return std::abs(diag1 - diag2) <= epsilon;
        } else {
            return diag1 == diag2;
        }
    }
    
public:

    // Конструктор по дефолту

    // Создаёт равнобедренную трапецию с горизонтальными основаниями
    // Нижнее основание: от (0,0) до (4,0) - длина 4
    // Верхнее основание: от (1,2) до (3,2) - длина 2
    // Высота трапеции: 2
    // Боковые стороны равны: обе длиной √5 ≈ 2.236
    Trapezoid() {
        // Резервируем память для 4 точек
        this->points.reserve(4);
        
        // Задаём вершины равнобедренной трапеции
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
        
        
        // Математический факт: четырёхугольник вписан в круг ⟺ 
        // ⟺ сумма противоположных углов = 180°
        // Для трапеции это эквивалентно равнобедренности (равные диагонали)
        if (!isIsoscelesTrapezoid()) {
            throw std::invalid_argument("Трапеция должна быть равнобедренной (вписанной в круг)");
        }
    }
    
 
    // Переопределяем виртуальные методы

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