#include <gtest/gtest.h>
#include "Rectangle.h"
#include "Trapezoid.h"
#include "Rhombus.h"
#include "Array.h"
#include <memory>

using namespace std;


// ТЕСТЫ CONCEPT


TEST(ConceptTest, ScalarTypesCompile) {
    Point<int> p1;
    Point<double> p2;
    Point<float> p3;
    Point<long> p4;
    SUCCEED();
}



// ТЕСТЫ POINT


TEST(PointTest, DefaultConstructor) {
    Point<int> p;
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<int> p(10, 20);
    EXPECT_EQ(p.x, 10);
    EXPECT_EQ(p.y, 20);
}

TEST(PointTest, IntComparison) {
    Point<int> p1(10, 20);
    Point<int> p2(10, 20);
    Point<int> p3(15, 25);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(PointTest, DoubleComparison) {
    Point<double> p1(1.0, 2.0);
    Point<double> p2(1.0, 2.0);
    EXPECT_TRUE(p1 == p2);
}

TEST(PointTest, FloatingPointEpsilon) {
    Point<double> p1(1.0000001, 2.0);
    Point<double> p2(1.0, 2.0);
    EXPECT_TRUE(p1 == p2);  // Разница меньше epsilon
}


// ТЕСТЫ ARRAY (БАЗОВАЯ ФУНКЦИОНАЛЬНОСТЬ)


TEST(ArrayTest, EmptyArray) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, PushIntegers) {
    Array<int> arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, RemoveElement) {
    Array<int> arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);
    arr.remove(1);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30);
}

TEST(ArrayTest, AutoResize) {
    Array<int> arr;
    for (int i = 0; i < 20; ++i) {
        arr.push(i);
    }
    EXPECT_EQ(arr.size(), 20);
    for (size_t i = 0; i < 20; ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i));
    }
}

TEST(ArrayTest, OutOfRangeException) {
    Array<int> arr;
    arr.push(10);
    EXPECT_THROW(arr[5], std::out_of_range);
    EXPECT_THROW(arr.remove(5), std::out_of_range);
}

TEST(ArrayTest, ClearArray) {
    Array<int> arr;
    arr.push(10);
    arr.push(20);
    arr.clear();
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, Reserve) {
    Array<int> arr;
    arr.reserve(100);
    for (int i = 0; i < 50; ++i) {
        arr.push(i);
    }
    EXPECT_EQ(arr.size(), 50);
}

// ТЕСТЫ RECTANGLE


TEST(RectangleTest, DefaultConstructorInt) {
    Rectangle<int> rect;
    EXPECT_EQ(rect.area(), 2);
    EXPECT_EQ(rect.vertexCount(), 4);
    EXPECT_STREQ(rect.getType(), "Rectangle");
}

TEST(RectangleTest, DefaultConstructorDouble) {
    Rectangle<double> rect;
    EXPECT_DOUBLE_EQ(rect.area(), 2.0);
}

TEST(RectangleTest, CustomVertices) {
    Point<int> points[4] = {
        Point<int>(0, 0),
        Point<int>(3, 0),
        Point<int>(3, 2),
        Point<int>(0, 2)
    };
    Rectangle<int> rect(points, 4);
    EXPECT_EQ(rect.area(), 6);
}

TEST(RectangleTest, Center) {
    Rectangle<double> rect;
    auto center = rect.center();
    EXPECT_DOUBLE_EQ(center.x, 1.0);
    EXPECT_DOUBLE_EQ(center.y, 0.5);
}

TEST(RectangleTest, DoubleOperator) {
    Rectangle<int> rect;
    double area = static_cast<double>(rect);
    EXPECT_DOUBLE_EQ(area, 2.0);
}

TEST(RectangleTest, InvalidVertexCount) {
    Point<int> points[3] = {
        Point<int>(0, 0),
        Point<int>(1, 0),
        Point<int>(1, 1)
    };
    EXPECT_THROW(Rectangle<int>(points, 3), std::invalid_argument);
}


// ТЕСТЫ TRAPEZOID


TEST(TrapezoidTest, DefaultConstructor) {
    Trapezoid<int> trap;
    EXPECT_EQ(trap.area(), 6);
    EXPECT_EQ(trap.vertexCount(), 4);
}

TEST(TrapezoidTest, CustomVertices) {
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(3, 2),
        Point<double>(1, 2)
    };
    Trapezoid<double> trap(points, 4);
    EXPECT_NEAR(trap.area(), 6.0, 0.01);
}

TEST(TrapezoidTest, GetType) {
    Trapezoid<int> trap;
    EXPECT_STREQ(trap.getType(), "Trapezoid");
}

// ТЕСТЫ RHOMBUS


TEST(RhombusTest, DefaultConstructor) {
    Rhombus<int> rhomb;
    EXPECT_EQ(rhomb.area(), 4);
    EXPECT_EQ(rhomb.vertexCount(), 4);
}

TEST(RhombusTest, CustomVertices) {
    Point<double> points[4] = {
        Point<double>(3, 0),
        Point<double>(0, 2),
        Point<double>(-3, 0),
        Point<double>(0, -2)
    };
    Rhombus<double> rhomb(points, 4);
    EXPECT_NEAR(rhomb.area(), 12.0, 0.01);
}

TEST(RhombusTest, GetType) {
    Rhombus<int> rhomb;
    EXPECT_STREQ(rhomb.getType(), "Rhombus");
}


// ТЕСТЫ КОПИРОВАНИЯ


TEST(CopyTest, RectangleCopyConstructor) {
    Rectangle<int> rect1;
    Rectangle<int> rect2(rect1);
    EXPECT_EQ(rect1.area(), rect2.area());
    EXPECT_TRUE(rect1 == rect2);
}

TEST(CopyTest, RectangleCopyAssignment) {
    Rectangle<int> rect1;
    Rectangle<int> rect2;
    Point<int> points[4] = {
        Point<int>(0, 0),
        Point<int>(5, 0),
        Point<int>(5, 3),
        Point<int>(0, 3)
    };
    rect2 = Rectangle<int>(points, 4);
    EXPECT_NE(rect1.area(), rect2.area());
}

TEST(CopyTest, TrapezoidCopy) {
    Trapezoid<double> trap1;
    Trapezoid<double> trap2(trap1);
    EXPECT_DOUBLE_EQ(trap1.area(), trap2.area());
}

TEST(CopyTest, RhombusCopy) {
    Rhombus<float> rhomb1;
    Rhombus<float> rhomb2;
    rhomb2 = rhomb1;
    EXPECT_FLOAT_EQ(rhomb1.area(), rhomb2.area());
}


// ТЕСТЫ СРАВНЕНИЯ


TEST(ComparisonTest, EqualRectangles) {
    Rectangle<int> rect1;
    Rectangle<int> rect2;
    EXPECT_TRUE(rect1 == rect2);
}

TEST(ComparisonTest, DifferentRectangles) {
    Rectangle<int> rect1;
    Point<int> points[4] = {
        Point<int>(0, 0),
        Point<int>(5, 0),
        Point<int>(5, 3),
        Point<int>(0, 3)
    };
    Rectangle<int> rect2(points, 4);
    EXPECT_FALSE(rect1 == rect2);
}

TEST(ComparisonTest, DifferentTypes) {
    Rectangle<int> rect;
    Trapezoid<int> trap;
    // Разные типы могут быть равны если вершины совпадают
    EXPECT_FALSE(rect == trap);
}


// ТЕСТЫ ARRAY С УКАЗАТЕЛЯМИ НА FIGURE


TEST(ArrayFigureTest, SharedPtrPolymorphism) {
    Array<shared_ptr<Figure<int>>> figures;
    
    figures.push(make_shared<Rectangle<int>>());
    figures.push(make_shared<Trapezoid<int>>());
    figures.push(make_shared<Rhombus<int>>());
    
    EXPECT_EQ(figures.size(), 3);
    EXPECT_EQ(figures[0]->area(), 2);
    EXPECT_EQ(figures[1]->area(), 6);
    EXPECT_EQ(figures[2]->area(), 4);
    
    int total = figures.totalArea();
    EXPECT_EQ(total, 12);
    
}

TEST(ArrayFigureTest, SharedPtr) {
    Array<shared_ptr<Figure<double>>> figures;
    
    figures.push(make_shared<Rectangle<double>>());
    figures.push(make_shared<Trapezoid<double>>());
    figures.push(make_shared<Rhombus<double>>());
    
    EXPECT_EQ(figures.size(), 3);
    EXPECT_NEAR(figures.totalArea(), 12.0, 0.01);
}

TEST(ArrayFigureTest, MixedTypes) {
    Array<shared_ptr<Figure<int>>> figures;
    
    figures.push(make_shared<Rectangle<int>>());
    figures.push(make_shared<Trapezoid<int>>());
    
    EXPECT_STREQ(figures[0]->getType(), "Rectangle");
    EXPECT_STREQ(figures[1]->getType(), "Trapezoid");
}

// ТЕСТЫ ARRAY С ОБЪЕКТАМИ


TEST(ArrayObjectTest, RectangleObjects) {
    Array<Rectangle<int>> rectangles;
    
    rectangles.push(Rectangle<int>());
    
    Point<int> points[4] = {
        Point<int>(0, 0),
        Point<int>(3, 0),
        Point<int>(3, 2),
        Point<int>(0, 2)
    };
    rectangles.push(Rectangle<int>(points, 4));
    
    EXPECT_EQ(rectangles.size(), 2);
    EXPECT_EQ(rectangles[0].area(), 2);
    EXPECT_EQ(rectangles[1].area(), 6);
}

TEST(ArrayObjectTest, TrapezoidObjects) {
    Array<Trapezoid<double>> trapezoids;
    
    trapezoids.push(Trapezoid<double>());
    trapezoids.push(Trapezoid<double>());
    
    EXPECT_EQ(trapezoids.size(), 2);
}


// ТЕСТЫ MOVE-СЕМАНТИКИ


TEST(MoveTest, ArrayResize) {
    Array<Rectangle<int>> rectangles;
    
    for (int i = 0; i < 10; ++i) {
        rectangles.push(Rectangle<int>());
    }
    
    EXPECT_EQ(rectangles.size(), 10);
    for (size_t i = 0; i < rectangles.size(); ++i) {
        EXPECT_EQ(rectangles[i].area(), 2);
    }
}

TEST(MoveTest, UniquePtrInArray) {
    Array<unique_ptr<Rectangle<int>>> rectangles;
    
    rectangles.push(make_unique<Rectangle<int>>());
    rectangles.push(make_unique<Rectangle<int>>());
    
    EXPECT_EQ(rectangles.size(), 2);
    EXPECT_EQ(rectangles[0]->area(), 2);
}


// ТЕСТЫ ПОЛИМОРФИЗМА


TEST(PolymorphismTest, BasePointer) {
    shared_ptr<Figure<int>> fig = make_shared<Rectangle<int>>();
    
    EXPECT_EQ(fig->area(), 2);
    EXPECT_STREQ(fig->getType(), "Rectangle");
    
}

TEST(PolymorphismTest, VirtualMethods) {
    Array<shared_ptr<Figure<double>>> figures;
    
    figures.push(make_shared<Rectangle<double>>());
    figures.push(make_shared<Trapezoid<double>>());
    figures.push(make_shared<Rhombus<double>>());
    
    EXPECT_STREQ(figures[0]->getType(), "Rectangle");
    EXPECT_STREQ(figures[1]->getType(), "Trapezoid");
    EXPECT_STREQ(figures[2]->getType(), "Rhombus");
}


// ТЕСТЫ РАЗНЫХ СКАЛЯРНЫХ ТИПОВ


TEST(ScalarTest, IntFloatDouble) {
    Rectangle<int> rect_int;
    Rectangle<float> rect_float;
    Rectangle<double> rect_double;
    
    EXPECT_EQ(rect_int.area(), 2);
    EXPECT_FLOAT_EQ(rect_float.area(), 2.0f);
    EXPECT_DOUBLE_EQ(rect_double.area(), 2.0);
}

TEST(ScalarTest, LongType) {
    Rectangle<long> rect_long;
    EXPECT_EQ(rect_long.area(), 2L);
}

TEST(ScalarTest, CharType) {
    Point<char> p(10, 20);
    EXPECT_EQ(p.x, 10);
    EXPECT_EQ(p.y, 20);
}


// ТЕСТЫ ИТЕРАТОРОВ


TEST(IteratorTest, RangeBasedFor) {
    Array<int> arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);
    
    int sum = 0;
    for (const auto& val : arr) {
        sum += val;
    }
    EXPECT_EQ(sum, 60);
}

TEST(IteratorTest, ManualIteration) {
    Array<int> arr;
    arr.push(1);
    arr.push(2);
    arr.push(3);
    
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
}


// СТРЕСС-ТЕСТЫ


TEST(StressTest, LargeArray) {
    Array<int> arr;
    for (int i = 0; i < 1000; ++i) {
        arr.push(i);
    }
    EXPECT_EQ(arr.size(), 1000);
}

TEST(StressTest, ManyFigures) {
    Array<shared_ptr<Figure<double>>> figures;
    
    for (int i = 0; i < 100; ++i) {
        figures.push(make_shared<Rectangle<double>>());
    }
    
    EXPECT_EQ(figures.size(), 100);
    EXPECT_NEAR(figures.totalArea(), 200.0, 0.01);
}

// ===== ПРОВЕРКА ГЕОМЕТРИЧЕСКИХ СВОЙСТВ =====

TEST(ValidationTest, RhombusInvalidSides) {
    // Попытка создать "ромб" с неравными сторонами
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(5, 0),  // Длинная сторона
        Point<double>(6, 1),
        Point<double>(1, 1)
    };
    
    EXPECT_THROW(Rhombus<double>(points, 4), std::invalid_argument);
}

TEST(ValidationTest, RhombusValidSides) {
    // Правильный ромб - все стороны равны
    Point<double> points[4] = {
        Point<double>(2, 0),
        Point<double>(0, 1),
        Point<double>(-2, 0),
        Point<double>(0, -1)
    };
    
    EXPECT_NO_THROW(Rhombus<double>(points, 4));
}

TEST(ValidationTest, RectangleInvalidShape) {
    // Попытка создать "прямоугольник" из неправильных точек
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(5, 0),
        Point<double>(6, 3),  // Несимметричная точка
        Point<double>(0, 3)
    };
    
    EXPECT_THROW(Rectangle<double>(points, 4), std::invalid_argument);
}

TEST(ValidationTest, RectangleValidShape) {
    // Правильный прямоугольник
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(4, 3),
        Point<double>(0, 3)
    };
    
    EXPECT_NO_THROW(Rectangle<double>(points, 4));
}

TEST(ValidationTest, TrapezoidNotInscribed) {
    // Трапеция, которая НЕ вписана в круг
    // Произвольная трапеция с явно неравными диагоналями
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(10, 0),
        Point<double>(7, 3),   // Изменено с (8,5)
        Point<double>(1, 3)    // Изменено с (2,5)
    };
    
    EXPECT_THROW(Trapezoid<double>(points, 4), std::invalid_argument);
}

TEST(ValidationTest, TrapezoidInscribed) {
    // Вписанная трапеция (равнобедренная с особыми пропорциями)
    Point<double> points[4] = {
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(3, 2),
        Point<double>(1, 2)
    };
    
    // Эта трапеция может быть вписана в окружность
    EXPECT_NO_THROW(Trapezoid<double>(points, 4));
}