#pragma once
#include "Figure.h"


// КЛАСС РОМБ

// Наследуется от Figure<T> 
// Ромб - это четырёхугольник, все стороны которого равны
// Характеристики:
// - 4 вершины
// - Все 4 стороны имеют одинаковую длину
// - Противоположные углы равны
// - Диагонали перпендикулярны и делят друг друга пополам
// - Является параллелограммом
// - Частный случай: квадрат (ромб с прямыми углами)
template<Scalar T>
class Rhombus : public Figure<T> {
private:
    // Проверка, что 4 точки образуют ромб (все стороны равны)
    bool isRhombus() const {
        if (this->points.size() != 4) return false;
        
        // Вычисляем длины всех 4 сторон
        T side1 = this->distance(this->points[0], this->points[1]);
        T side2 = this->distance(this->points[1], this->points[2]);
        T side3 = this->distance(this->points[2], this->points[3]);
        T side4 = this->distance(this->points[3], this->points[0]);
        
        // Проверяем, что все стороны равны (с погрешностью для float/double)
        if constexpr (std::is_floating_point_v<T>) {
            const T epsilon = static_cast<T>(1e-6);
            return std::abs(side1 - side2) <= epsilon &&
                   std::abs(side2 - side3) <= epsilon &&
                   std::abs(side3 - side4) <= epsilon;
        } else {
            return side1 == side2 && side2 == side3 && side3 == side4;
        }
    }
    
public:
    
    // Конструктор дефолтный
   
    // Создаёт ромб с диагоналями вдоль осей координат
    // Вершины: (2,0), (0,1), (-2,0), (0,-1)
    // Диагонали: горизонтальная длиной 4, вертикальная длиной 2
    Rhombus() {
        // Резервируем память для 4 вершин
        this->points.reserve(4);
        
        // Создаём вершины ромба, расположенные симметрично относительно начала координат
        // static_cast<T> - явное преобразование типа (int -> T)
        // Необходимо, т.к. литерал 2 имеет тип int
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(2), T{}));
        this->points.push(std::make_unique<Point<T>>(T{}, static_cast<T>(1)));
        this->points.push(std::make_unique<Point<T>>(static_cast<T>(-2), T{}));
        this->points.push(std::make_unique<Point<T>>(T{}, static_cast<T>(-1)));
        
        // Сортируем вершины против часовой стрелки
        // Важно для формулы Гаусса при вычислении площади
        this->sortPoints();
    }
    

    // Конструктор из набора точек

    // Позволяет создать ромб с произвольными координатами вершин
    // pts - массив точек (должен содержать 4 элемента)
    // n - размер массива
    Rhombus(const Point<T>* pts, size_t n) {
        // Проверка количества вершин
        if (n != 4) {
            throw std::invalid_argument("Ромб должен иметь 4 вершины");
        }
        
        this->points.reserve(4);
        
        // Копируем точки из входного массива в наш массив unique_ptr
        for (size_t i = 0; i < n; ++i) {
            // Создаём новый Point копированием из pts[i]
            this->points.push(std::make_unique<Point<T>>(pts[i]));
        }
        
        // Упорядочиваем вершины
        this->sortPoints();
        
        // ПРОВЕРКА: все стороны должны быть равны (фигура вращения)
        if (!isRhombus()) {
            throw std::invalid_argument("Точки не образуют ромб (стороны не равны)");
        }
    }
    
  
    // Переопределяем виртуальные методы
    
    // Вычисление площади ромба
    // Формула: S = (d1 * d2) / 2, где d1, d2 - длины диагоналей
    // Но мы используем универсальную формулу Гаусса из базового класса
    T area() const override { 
        return this->calculateArea(); 
    }
    
    // Вычисление центра ромба
    // Для ромба центр = точка пересечения диагоналей = среднее вершин
    Point<T> center() const override { 
        return this->calculateCenter(); 
    }
    

    // Выводим информацию

    // Этот метод вызывается полиморфно через указатель на базовый класс

    void print(std::ostream& os) const override {
        os << "Rhombus: ";  // Тип фигуры
        
        // Выводим координаты всех вершин
        for (size_t i = 0; i < this->points.size(); ++i) {
            // *this->points[i] - разыменовываем unique_ptr, получаем Point&
            // Затем вызывается Point::operator<<
            os << *this->points[i];
            
            // Добавляем пробел между точками (кроме последней)
            if (i < this->points.size() - 1) os << " ";
        }
    }
    
    // Возвращает строковое представление типа
    // Используется для отладки и информационных сообщений
    const char* getType() const override { 
        return "Rhombus"; 
    }
};