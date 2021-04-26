#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

template<class T>
class СircularBuffer
{
private:
    int capacity = 0;
    int size = 0;
    T *arr = new T[size];
    T *first, *last, *cur_first, *cur_last; // first и last указывают на первый последний и неизменимы
public:
    // Итератор
    class Iterator : public iterator<random_access_iterator_tag, T> {
    private:
        T *currentValue;
    public:
        explicit Iterator(T *currentValue) {
            this->currentValue = currentValue;
        }

        [[nodiscard]] T *getCurent() const {
            return currentValue;
        }

        //OPERATORS
        Iterator operator+(int value) {
            currentValue += value;
            return *this;
        }

        Iterator operator-(int value) {
            currentValue -= value;
            return *this;
        }

        T &operator*() const {
            return *currentValue;
        }

        T *operator->() const {
            return currentValue;
        }

        Iterator &operator++() {
            ++currentValue;
            return *this;
        }

        Iterator operator--() {
            currentValue--;
            return *this;
        }

        Iterator &operator=(T *other) {
            currentValue = other;
            return *this;
        }

        bool operator==(const Iterator &other) {
            return this->currentValue = other.currentValue;
        }

        bool operator!=(const Iterator &other) {
            return this->currentValue != other.currentValue;
        }

        bool operator>(const Iterator &other) {
            return this->currentValue > other.currentValue;
        }

        bool operator>=(const Iterator &other) {
            return this->currentValue >= other.currentValue;
        }

        bool operator<(const Iterator &other) {
            return this->currentValue < other.currentValue;
        }

        bool operator<=(const Iterator &other) {
            return this->currentValue <= other.currentValue;
        }
    };

    // Конструктор по умолчанию, конструктор и деструктор
    explicit СircularBuffer(int size_)
    {
        this->capacity = size_ + 0.2 * size_;
        size = size_;
        this->first = &arr[0];
        this->last = &arr[size - 1];
        this->cur_first = this->cur_last = &arr[0];
    }
    СircularBuffer(const СircularBuffer& buf)
    {
        this->capacity = buf.capacity;
        this->size = buf.size;
        this->arr = buf.arr;
    }
    ~СircularBuffer()
    {
        delete arr;
    }

    [[nodiscard]] Iterator begin() // возвращает первый элемент
    {
        return Iterator(arr);
    }
    [[nodiscard]] Iterator end() // возвращает последний элемент
    {
        return Iterator(arr + size - 1);
    }
    int capacity_buffer() const // возвращает капасити
    {
        return capacity;
    }
    int size_buffer() const // возвращает размер
    {
        return size;
    }
    void push_back(const T& value) // вставка элемента в конец
    {
        *cur_last = value;
        last == cur_last ? cur_last = first : cur_last++;
    }
    void push_front(const T& value) // вставка элемента в начало
    {
        *cur_first = value;
        first == cur_first ? cur_first = last : cur_first--;
    }
    void pop_back() // удаление элемента с конца
    {
        *cur_last = 0;
        cur_last == first ? cur_last = last : cur_last++;
    }
    void pop_front() // удаление элемента с начала
    {
        *cur_first = 0;
        cur_first == last ? cur_first = first : cur_first++;
    }
    T operator [] (int i) const // возвращает i-ый член
    {
        return arr[i % capacity];
    }
    void change_capacity(int value)
    {
        if (capacity < value)
            capacity = value;
        else
        {
            if (size > value)
            {
                capacity = value;
            }
            else
            {
                capacity = value;
            }
        }
    }
    void Print()
    {
        for (auto i = &arr[0]; i != &arr[size]; i++)
            cout << *i << ' ';
        cout << endl;
    }
};

template<class T>
class CPoint
{
private:
    T x, y;
public:
    CPoint() {}
    CPoint(T x, T y)
    {
        this->x = x;
        this->y = y;
    }
    T getX() const
    {
        return this->x;
    }
    T getY() const
    {
        return this->y;
    }
    void Print()
    {
        cout << "(" << x << ";" << y << ") ";
    }
};

int main()
{
    cout << endl;
    cout << "СircularBuffer int (5) | push 1 2 3 4 5 | pop_front x2 | push 10 20 30" << endl;
    СircularBuffer<int> buf_int(4);
    buf_int.push_front(1);
    buf_int.push_front(2);
    buf_int.push_front(3);
    buf_int.push_front(4);
    buf_int.Print();
    buf_int.pop_front();
    buf_int.pop_front();
    buf_int.Print();
    buf_int.push_front(10);
    buf_int.push_front(20);
    buf_int.push_front(30);
    buf_int.Print();
    cout << endl;

    cout << "СircularBuffer float | push 1.1 2.1 3.1 4.1 " << endl;
    СircularBuffer<float> buf_float(4);
    buf_float.push_back(1.1);
    buf_float.push_back(2.1);
    buf_float.push_back(3.1);
    buf_float.push_back(4.1);
    buf_float.Print();
    cout << endl;

    cout << "СircularBuffer CPoint | push 1.1 2.2 3.3 4.4 " << endl;
    СircularBuffer<CPoint<float>> buf_CPoint(10);
    CPoint<float> a(1.1, 1.1), b(2.2, 2.2), c(3.3, 3.3);
    buf_CPoint.push_back(a);
    buf_CPoint.push_back(b);
    buf_CPoint.push_back(c);
    buf_CPoint.push_back(c);
    buf_CPoint.push_back(c);
    for (int i = 0; i < buf_CPoint.size_buffer(); i++)
        buf_CPoint[i].Print();
    cout << endl;
    cout << endl;

    auto print = [](const int &n) { std::cout << n << " "; };
    for_each(buf_int.begin(), buf_int.end() + 1, print);
    return 0;
}
