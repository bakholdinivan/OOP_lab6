#pragma once
#include "Point.h"
#include "Array.h"
#include <memory>        // Для unique_ptr, shared_ptr
#include <algorithm>     // Для std::swap
#include <cmath>         // Для std::abs, atan2



// Базовый абстрактный класс для всех геометрических фигур
// Шаблонный класс: T - тип координат (должен быть скалярным)

template<Scalar T>
class Figure {
protected:
\
    // Хранение вершин фигурф

    // unique_ptr - умный указатель с эксклюзивным владением
    // В любой момент времени только ОДИН unique_ptr владеет объектом
    // При уничтожении unique_ptr автоматически удаляется объект
    // Нельзя копировать unique_ptr, только перемещать (std::move)
    Array<std::unique_ptr<Point<T>>> points;
    

    // Сортируем против часовой

    // Необходима для корректного вычисления площади по формуле Гаусса
    // Формула Гаусса (shoelace formula) требует обхода вершин в порядке
    void sortPoints() {
        if (points.size() < 3) return;  // Меньше 3 точек - не многоугольник
        
        // Находим геометрический центр фигуры (центр масс)
        T cx = T{}, cy = T{};  // Инициализация нулями
        for (size_t i = 0; i < points.size(); ++i) {
            cx += points[i]->x;
            cy += points[i]->y;
        }
        cx /= static_cast<T>(points.size());  // Среднее арифметическое
        cy /= static_cast<T>(points.size());
        
        // Вспомогательная структура: точка + её угол относительно центра
        struct PtAng {
            std::unique_ptr<Point<T>> pt;  // Умный указатель на точку
            T angle;                       // Угол в радианах
        };
        
        // Создаём временный массив для сортировки
        std::unique_ptr<PtAng[]> temp(new PtAng[points.size()]);
        
        // Вычисляем угол для каждой точки относительно центра
        for (size_t i = 0; i < points.size(); ++i) {
            temp[i].pt = std::move(points[i]);  // Перемещаем точку во временный массив
            
            // atan2(y, x) - арктангенс от y/x, возвращает угол в диапазоне [-π, π]
            // Вычисляем угол от центра до точки
            temp[i].angle = std::atan2(temp[i].pt->y - cy, temp[i].pt->x - cx);
        }
        
            // Сортируем
        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                if (temp[i].angle > temp[j].angle) {
                    std::swap(temp[i], temp[j]);  // Меняем местами
                }
            }
        }
        
        // Перемещаем отсортированные точки обратно в points
        for (size_t i = 0; i < points.size(); ++i) {
            points[i] = std::move(temp[i].pt);
        }
    }
    
public:

    // Деструктор(обязательно виртуальный)

    // virtual обязателен для базовых классов с полиморфизмом
    // Обеспечивает правильное удаление объектов через указатель на базовый класс

    virtual ~Figure() = default;
    
    // Конструктор по умолчанию
    Figure() = default;
    
   
    // Конструктор копирования
 
    // Выполняет глубокое копирование всех точек
    // Каждая точка копируется в новый unique_ptr
    Figure(const Figure& other) {
        points.reserve(other.points.size());  // Резервируем память
        
        for (size_t i = 0; i < other.points.size(); ++i) {
            // make_unique - создаёт unique_ptr с новым объектом
            // *other.points[i] - разыменовываем unique_ptr для копирования Point
            points.push(std::make_unique<Point<T>>(*other.points[i]));
        }
    }
    

    // Оператор присваивания копированием

    Figure& operator=(const Figure& other) {
        if (this != &other) {  // Защита от самоприсваивания: fig = fig;
            points.clear();    // Удаляем старые точки
            points.reserve(other.points.size());
            
            for (size_t i = 0; i < other.points.size(); ++i) {
                points.push(std::make_unique<Point<T>>(*other.points[i]));
            }
        }
        return *this;  // Возвращаем ссылку для цепочки присваиваний: a = b = c;
    }
    

    // Конструктор перемещения

    // = default - компилятор генерирует автоматически
    // Перемещает ресурсы (points) от временного объекта
    // noexcept - гарантирует отсутствие исключений
    Figure(Figure&& other) noexcept = default;
    

    // Оператор присваивания перемещениемм

    Figure& operator=(Figure&& other) noexcept = default;
    

 

    // = 0 означает "чисто виртуальный метод"
    // Наследники ОБЯЗАНЫ реализовать эти методы
    // Делает класс Figure абстрактным (нельзя создать объект)
    
    virtual T area() const = 0;                      // Вычисление площади
    virtual Point<T> center() const = 0;             // Вычисление центра
    virtual void print(std::ostream& os) const = 0;  // Вывод в поток
    virtual const char* getType() const = 0;         // Получение типа фигуры
    
    
    // Перегрузка вывода в поток
  
    // friend - функция не член класса, но имеет доступ к private/protected
    // Позволяет писать: std::cout << figure;
    friend std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
        fig.print(os);  // Вызов виртуального метода 
        return os;
    }
    

    // Оператор приведения к double

    // explicit - запрещает неявное преобразование
    // Позволяет писать: double area = static_cast<double>(figure);
    // Возвращает площадь фигуры
    explicit operator double() const {
        return static_cast<double>(area());
    }
    
  
    // Оператор сравнения фигур

    // virtual - наследники могут переопределить логику сравнения
    // Сравнивает фигуры по множествам вершин (порядок не важен)
    virtual bool operator==(const Figure<T>& other) const {
        if (points.size() != other.points.size()) {
            return false;  // Разное количество вершин
        }
        
        // Проверяем, что каждая точка первой фигуры есть во второй
        for (size_t i = 0; i < points.size(); ++i) {
            bool found = false;
            for (size_t j = 0; j < other.points.size(); ++j) {
                if (*points[i] == *other.points[j]) {  // Разыменовываем unique_ptr
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        
        return true;
    }
    
    // Получение количества вершин
    size_t vertexCount() const { return points.size(); }
    
    // Получение вершины по индексу (разыменовываем unique_ptr)
    const Point<T>& getVertex(size_t idx) const { return *points[idx]; }
    
protected:
   
    // ВЫЧИСЛЕНИЕ ПЛОЩАДИ ПО ФОРМУЛЕ ГАУССА (SHOELACE FORMULA)

    // Формула: S = (1/2) * |Σ(x_i * y_{i+1} - x_{i+1} * y_i)|
    // Требует обхода вершин в правильном порядке (против часовой стрелки)
    T calculateArea() const {
        if (points.size() < 3) return T{};  // Меньше 3 вершин - не многоугольник
        
        T sum = T{};
        for (size_t i = 0; i < points.size(); ++i) {
            size_t j = (i + 1) % points.size();  // Следующая вершина (циклично)
            
            // Произведение крест-накрест
            sum += points[i]->x * points[j]->y;
            sum -= points[j]->x * points[i]->y;
        }
        
        return std::abs(sum) / static_cast<T>(2);  // Модуль и делим на 2
    }
    
  
    // ВЫЧИСЛЕНИЕ ГЕОМЕТРИЧЕСКОГО ЦЕНТРА (ЦЕНТРА МАСС)

    // Центр = среднее арифметическое координат всех вершин
    Point<T> calculateCenter() const {
        T cx = T{}, cy = T{};
        
        for (size_t i = 0; i < points.size(); ++i) {
            cx += points[i]->x;
            cy += points[i]->y;
        }
        
        size_t n = points.size();
        return Point<T>(cx / static_cast<T>(n), cy / static_cast<T>(n));
    }
};