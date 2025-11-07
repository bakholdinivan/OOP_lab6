#pragma once
#include <memory>
#include <stdexcept>
#include <utility>
#include <type_traits>

// Универсальный шаблонный динамический массив.
// Может хранить любой тип: int, unique_ptr, shared_ptr, объекты фигур.
template<typename T>
class Array {
private:
    std::shared_ptr<T[]> data;  // Умный указатель на массив элементов.
    size_t cnt;                 // Текущее количество элементов.
    size_t cap;                 // Вместимость (выделенная память).
    
    // Увеличивает размер массива \
    // Использует std::move для переноса элементов (важно для уникальных указателей).
    void resize() {
        size_t newCap = (cap == 0) ? 4 : (cap * 2);
        std::shared_ptr<T[]> newData(new T[newCap]);
        for (size_t i = 0; i < cnt; ++i) {
            newData[i] = std::move(data[i]);
        }
        data = newData;
        cap = newCap;
    }
    
public:
    Array() : data(nullptr), cnt(0), cap(0) {}
    ~Array() = default;
    
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    Array(Array&&) noexcept = default;
    Array& operator=(Array&&) noexcept = default;
    
    void reserve(size_t n) {
        if (n <= cap) return;
        std::shared_ptr<T[]> newData(new T[n]);
        for (size_t i = 0; i < cnt; ++i) {
            newData[i] = std::move(data[i]);
        }
        data = newData;
        cap = n;
    }
    
    // Перегрузка для rvalue (перемещение) — нужна для move-only типов, например unique_ptr.
    void push(T&& value) {
        if (cnt >= cap) {
            resize();
        }
        data[cnt++] = std::move(value);
    }

    // Перегрузка для lvalue (копирование) — чтобы компилировались вызовы типа arr.push(i), где i — переменная.
    // Опционально можно ограничить только копируемыми типами:
    // requires(std::is_copy_assignable_v<T>)
    void push(const T& value) {
        if (cnt >= cap) {
            resize();
        }
        data[cnt++] = value; // Копирование для копируемых типов (int, double, и т.д.)
    }
    
    void remove(size_t idx) {
        if (idx >= cnt) {
            throw std::out_of_range("Индекс за границами массива");
        }
        for (size_t i = idx; i < cnt - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        cnt--;
    }
    
    T& get(size_t idx) {
        if (idx >= cnt) {
            throw std::out_of_range("Индекс за границами массива");
        }
        return data[idx];
    }
    const T& get(size_t idx) const {
        if (idx >= cnt) {
            throw std::out_of_range("Индекс за границами массива");
        }
        return data[idx];
    }
    
    T& operator[](size_t idx) {
        return get(idx);
    }
    const T& operator[](size_t idx) const {
        return get(idx);
    }
    
    size_t size() const {
        return cnt;
    }
    
    void clear() {
        cnt = 0;
    }
    
    // Вычисляет общую площадь всех фигур — работает если T это указатель на фигуру или shared_ptr.
    template<typename U = T>
    auto totalArea() const -> decltype((*std::declval<U>()).area()) {
        using AreaType = decltype((*std::declval<U>()).area());
        AreaType total{};
        for (size_t i = 0; i < cnt; ++i) {
            total += data[i]->area();
        }
        return total;
    }
    
    void printAll(std::ostream& os) const {
        for (size_t i = 0; i < cnt; ++i) {
            os << i << ". ";
            if constexpr (std::is_pointer_v<T>) {
                os << *data[i];
                auto c = data[i]->center();
                os << " | Центр: " << c;
                os << " | Площадь: " << data[i]->area();
            } else if constexpr (requires { data[i]->area(); }) {
                os << *data[i];
                auto c = data[i]->center();
                os << " | Центр: " << c;
                os << " | Площадь: " << data[i]->area();
            } else {
                os << data[i];
            }
            os << "\n";
        }
    }
    
    // Итераторы для range-based for
    class Iterator {
        T* ptr;
    public:
        explicit Iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        T* operator->() { return ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };
    Iterator begin() { return Iterator(data.get()); }
    Iterator end() { return Iterator(data.get() + cnt); }
    
    class ConstIterator {
        const T* ptr;
    public:
        explicit ConstIterator(const T* p) : ptr(p) {}
        const T& operator*() const { return *ptr; }
        const T* operator->() const { return ptr; }
        ConstIterator& operator++() { ++ptr; return *this; }
        bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }
    };
    ConstIterator begin() const { return ConstIterator(data.get()); }
    ConstIterator end() const { return ConstIterator(data.get() + cnt); }
};