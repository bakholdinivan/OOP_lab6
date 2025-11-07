#include <iostream>
#include "Rectangle.h"
#include "Trapezoid.h"
#include "Rhombus.h"
#include "Array.h"

using namespace std;

void printMenu() {
    cout << "\n========== МЕНЮ ==========" << endl;
    cout << "1. Добавить прямоугольник" << endl;
    cout << "2. Добавить трапецию" << endl;
    cout << "3. Добавить ромб" << endl;
    cout << "4. Показать все фигуры" << endl;
    cout << "5. Показать общую площадь" << endl;
    cout << "6. Удалить фигуру по индексу" << endl;
    cout << "7. Демонстрация работы с разными типами" << endl;
    cout << "0. Выход" << endl;
    cout << "==========================" << endl;
    cout << "Выберите действие: ";
}

// Демонстрация: массив указателей на базовый класс (полиморфизм)
void demoBaseType() {
    cout << "\n=== Array<shared_ptr<Figure<int>>> ===" << endl;
    
    
    Array<shared_ptr<Figure<int>>> figs;
    
    // Добавляем разные типы фигур через базовый указатель
    figs.push(make_shared<Rectangle<int>>());
    figs.push(make_shared<Trapezoid<int>>());
    figs.push(make_shared<Rhombus<int>>());
    
    cout << "Добавлено 3 фигуры\n" << endl;
    figs.printAll(cout);
    
    cout << "\nОбщая площадь: " << figs.totalArea() << endl;
    
   
}

// Демонстрация: массив объектов конкретного типа
void demoConcreteType() {
    cout << "\n=== Array<Rectangle<double>> ===" << endl;
    
    // Массив объектов (не указателей!)
    Array<Rectangle<double>> rects;
    
    // Добавляем прямоугольники
    rects.push(Rectangle<double>());
    
    Point<double> pts[4] = {
        Point<double>(0, 0),
        Point<double>(3, 0),
        Point<double>(3, 2),
        Point<double>(0, 2)
    };
    rects.push(Rectangle<double>(pts, 4));
    
    cout << "Добавлено 2 прямоугольника\n" << endl;
    rects.printAll(cout);
}

// Демонстрация: разные скалярные типы координат
void demoScalarTypes() {
    cout << "\n=== Разные типы координат ===" << endl;
    
    // int координаты
    Rectangle<int> r1;
    cout << "Rectangle<int>: " << r1 << " | Area: " << r1.area() << endl;
    
    // float координаты
    Rectangle<float> r2;
    cout << "Rectangle<float>: " << r2 << " | Area: " << r2.area() << endl;
    
    // double координаты
    Rectangle<double> r3;
    cout << "Rectangle<double>: " << r3 << " | Area: " << r3.area() << endl;
}

int main() {
    // Массив shared_ptr на Figure<double>
    // shared_ptr автоматически управляет памятью (не нужен delete)
    Array<shared_ptr<Figure<double>>> figs;
    
    int choice;
    
    while (true) {
        printMenu();
        cin >> choice;
        
        // Обработка ошибок ввода
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка ввода!" << endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                cout << "\nВведите координаты 4 вершин (x y):" << endl;
                cout << "Пример: 0 0 4 0 4 2 0 2" << endl;
                cout << "> ";
                
                Point<double> pts[4];
                for (int i = 0; i < 4; ++i) {
                    cin >> pts[i];
                }
                
                // make_shared создаёт shared_ptr и объект за один вызов
                auto rect = make_shared<Rectangle<double>>(pts, 4);
                figs.push(rect);
                
                cout << "Добавлен прямоугольник! Площадь: " << rect->area() << endl;
                break;
            }
            
            case 2: {
                cout << "\nВведите координаты 4 вершин (x y):" << endl;
                cout << "Пример: 0 0 4 0 3 2 1 2" << endl;
                cout << "> ";
                
                Point<double> pts[4];
                for (int i = 0; i < 4; ++i) {
                    cin >> pts[i];
                }
                
                auto trap = make_shared<Trapezoid<double>>(pts, 4);
                figs.push(trap);
                
                cout << "Добавлена трапеция! Площадь: " << trap->area() << endl;
                break;
            }
            
            case 3: {
                cout << "\nВведите координаты 4 вершин (x y):" << endl;
                cout << "Пример: 2 0 0 1 -2 0 0 -1" << endl;
                cout << "> ";
                
                Point<double> pts[4];
                for (int i = 0; i < 4; ++i) {
                    cin >> pts[i];
                }
                
                auto rhomb = make_shared<Rhombus<double>>(pts, 4);
                figs.push(rhomb);
                
                cout << "Добавлен ромб! Площадь: " << rhomb->area() << endl;
                break;
            }
            
            case 4: {
                cout << "\n=== Все фигуры ===" << endl;
                if (figs.size() == 0) {
                    cout << "Массив пуст" << endl;
                } else {
                    cout << "Всего: " << figs.size() << "\n" << endl;
                    figs.printAll(cout);
                }
                break;
            }
            
            case 5: {
                cout << "\nОбщая площадь: " << figs.totalArea() << endl;
                cout << "Количество фигур: " << figs.size() << endl;
                break;
            }
            
            case 6: {
                if (figs.size() == 0) {
                    cout << "\nМассив пуст" << endl;
                    break;
                }
                
                cout << "\nВведите индекс (0-" << figs.size() - 1 << "): ";
                size_t idx;
                cin >> idx;
                
                try {
                    cout << "Удаляем: " << *figs[idx] << endl;
                    figs.remove(idx);
                    cout << "Удалено! Осталось: " << figs.size() << endl;
                } catch (const exception& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            
            case 7: {
                demoBaseType();
                demoConcreteType();
                demoScalarTypes();
                break;
            }
            
            case 0:
                cout << "\nВыход" << endl;
                return 0;
            
            default:
                cout << "Неверный выбор" << endl;
        }
    }
    
    return 0;
}