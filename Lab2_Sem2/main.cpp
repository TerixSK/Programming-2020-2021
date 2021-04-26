#include<iostream>
#include<vector>
using namespace std;
class ElementPolynomial
{
private:
    float factor = 0;
    int power = 0;
public:
    ElementPolynomial() {}
    ElementPolynomial(int factor, int power)
    {
        this->factor = factor;
        this->power = power;
    }
    ~ElementPolynomial() {}

    void operator= (ElementPolynomial elPol)
    {
        factor = elPol.getFactor();
        power = elPol.getPower();
    }

    float getFactor()
    {
        return factor;
    }
    int getPower()
    {
        return power;
    }
    void setFactor(float _factor)
    {
        factor = _factor;
    }
    void setPower(int _power)
    {
        power = _power;
    }

    void Print()
    {
        cout << factor << "x^" << power;
    }
};

class Polynomial
{
private:
    vector<ElementPolynomial> arr;
    int k;

    // ||||||||| Прочие методы ||||||||||
    void sumPower(vector<ElementPolynomial> &a)
    {
        if (a.size() > 0)
            for (int i = 0; i < a.size(); i++)
                for (int j = (i + 1); j < a.size(); j++)
                    if (a[i].getPower() == a[j].getPower())
                    {
                        a[i].setFactor(a[i].getFactor() + a[j].getFactor());
                        a.erase(a.begin() + j);
                        j--;
                    }
    }
    void sumFactor(vector<ElementPolynomial> &a)
    {
        if (a.size() > 0)
        {
            ElementPolynomial temp;
            for (int i = 0; i < a.size(); i++)
            {
                if (a[i].getPower() == 0)
                {
                    temp.setFactor(temp.getFactor() + a[i].getFactor());
                    a.erase(a.begin() + i);
                    i--;
                }
            }
            a.push_back(temp);
        }
    }
    void sortByPowers(vector<ElementPolynomial> &a) {
        int k = a.size();
        if (k > 2) {
            for (int i = 0; i < a.size() - 1; i++) {
                for (int j = 1; j < a.size(); j++) {
                    if (a[j - 1].getPower() < a[j].getPower())
                        swap(a[j - 1], a[j]);
                }
            }
        } else if (k == 2) {
            if (a[0].getPower() < a[1].getPower())
                swap(a[0], a[1]);
        }
    }
    static void createVector(const vector<ElementPolynomial> &a, const vector<ElementPolynomial> &b, vector<ElementPolynomial> &c)
    {
        c = a;
        if (b.size() != -1)
            for (int i = 0; i < b.size(); i++)
                c.push_back(b[i]);
    }
public:
    // |||||||||| Конструкоры и деструктор ||||||||||
    Polynomial() {}
    Polynomial(vector<ElementPolynomial> &a)
    {
        this->k = a.size();
        this->setP(a);
    }
    ~Polynomial() {}
    // ----------------------------------------------

    // |||||||||| Гетторы и Сетторы ||||||||||
    ElementPolynomial getI(int i)
    {
        return arr[i];
    }
    int getK()
    {
        return k;
    }
    void setP(vector<ElementPolynomial> &a) {
        arr = a;
        sumPower(this->arr); // суммирование множителей элементов с одинаковой степенью
        sumFactor(this->arr); // нахождение констант в полиноме и их суммирование
        sortByPowers(this->arr); // сортировка по убыванию степеней
    }
    // -----------------------------

    // |||||||||| Прочие методы ||||||||||
    void Print()
    {
        cout << "Polynomial\n";
        for (auto i: arr)
        {
            i.Print();
            cout << "\n";
        }
    }
    // -----------------------------------

    // |||||||||| Операторы ||||||||||

    // Блок 1. Оператор =
    Polynomial &operator=(const Polynomial &p)
    {
        arr.resize(p.arr.size());
        for (int i = 0; i < p.arr.size(); i++)
            arr[i] = p.arr[i];
        this->setP(this->arr);

        return *this;
    }

    // Блок 2. Операторы == и !=
    bool operator == (Polynomial p)
    {
        if (k != p.getK())
            return false;
        for (int i = 0; i < p.getK(); i++)
            if (arr[i].getPower() != p.getI(i).getPower() || arr[i].getFactor() != p.getI(i).getFactor())
                return false;
        return true;
    }
    bool operator != (Polynomial p)
    {
        if (k != p.getK())
            return true;
        for (int i = 0; i < p.getK(); i++)
            if (arr[i].getPower() != p.getI(i).getPower() || arr[i].getFactor() != p.getI(i).getFactor())
                return true;
        return false;
    }

    // Блок 3. Операторы + - += -=
    friend Polynomial operator + (const Polynomial &p1, const Polynomial &p2)
    {
        vector<ElementPolynomial> sum;
        createVector(p1.arr, p2.arr, sum);
        return Polynomial(sum);
    }
    friend Polynomial operator - (const Polynomial &p1, const Polynomial &p2)
    {
        vector<ElementPolynomial> dif;
        vector<ElementPolynomial> invertP2 = p2.arr;
        int factor;
        for (int i = 0; i < invertP2.size(); i++)
        {
            factor = -1 * invertP2[i].getFactor();
            invertP2[i].setFactor(factor);
        }
        createVector(p1.arr, invertP2, dif);
        return Polynomial(dif);
    }
    Polynomial operator += (Polynomial &p)
    {
        *this = *this + p;
        return p;
    }
    Polynomial operator -= (Polynomial &p)
    {
        *this = *this - p;
        return p;
    }

    // Блок 4. Операторы * / *= /=
    friend Polynomial operator * (Polynomial &p1, Polynomial &p2)
    {
        vector<ElementPolynomial> temp;
        for (int i = 0; i < p1.arr.size(); i++)
            for (int j = 0; j < p2.arr.size(); j++)
                temp.push_back(ElementPolynomial(p1.arr[i].getFactor() * p2.arr[j].getFactor(), p1.arr[i].getPower() + p2.arr[j].getPower()));
        p1.setP(temp);
        return p1;
    }
    friend Polynomial operator / (Polynomial &p1, Polynomial &p2)
    {
        vector<ElementPolynomial> temp;
        int k = 0, maxP = 0;
        for (int i = 0; p2.arr.size(); i++) {
            if (p2.arr[i].getPower() == 0)
                k = -1 * p2.arr[i].getFactor();
            if (p2.arr[i].getPower() > maxP)
                maxP = p1.arr[i].getPower();
        }
        if (k != 0)
        {
            temp.push_back(ElementPolynomial(p1.arr[0].getFactor(), p1.arr[0].getPower() - 1));
            int с = p1.arr[0].getFactor();
            for (int i = 1; i < p1.arr.size(); i++)
            {
                с = с * k + p1.arr[i].getFactor();
                temp.push_back(ElementPolynomial(с, p1.arr[i].getPower() - 1));
            }
            temp[p1.arr.size() - 1].setPower(-1 * p2.arr[0].getPower());
            temp.push_back(ElementPolynomial(temp[p1.arr.size() - 1].getFactor() / p2.arr[p2.arr.size() - 1].getFactor(), 0));
            p1 = temp;
        }
        else if (k == 0)
        {
            if (p2.arr[0].getPower() != 0)
                for (int i = 0; i < p1.arr.size(); i++)
                {
                    p1.arr[i].setFactor(p1.arr[i].getFactor() / p2.arr[0].getFactor());
                    p1.arr[i].setPower(p1.arr[i].getPower() - p2.arr[0].getPower());
                }
            else
            {
                cout << "\n" << "Произошёл прикол" << "\n";
                p1.arr.resize(0);
            }
        }
        p1.setP(p1.arr);
        return p1;
    }
    Polynomial operator /= (Polynomial &p)
    {
        *this = *this / p;
        return p;
    }

    // Блок 5. Операторы >> <<
    friend istream &operator >> (istream &cin_, Polynomial &a)
    {
        cout << "Введине индекс, который вы хотите изменить: ";
        int index;
        while (index >= a.arr.size());
        {
            cin >> index;
            if (index >= a.arr.size())
                cout << "Данный индекс не найден. Введите другой: " << "\n";
        }
        int factor, power;
        if (cin_ >> factor && cin_ >> power) {
            a.arr[index] = ElementPolynomial(factor, power);
            a.setP(a.arr);
        } else {
            cout << "Ошибка >>" << "\n";
            exit(1);
        }
        return cin_;
    }
    friend ostream &operator << (ostream &cout_, Polynomial &a)
    {
        cout << "Введине индекс, который вы хотите вывести: ";
        int index;
        while (index >= a.arr.size());
        {
            cin >> index;
            if (index >= a.arr.size())
                cout << "Данный индекс не найден. Введите другой: " << "\n";
        }
        cout << "\n" << "Элемент многочлена с индексом: " << index << "\n";
        cout_ << "Степень: " << a.arr[index].getPower() << " Коэффициент: " << a.arr[index].getPower() << "\n";
        return cout_;
    }

    // Блок 6. Оператор []
    ElementPolynomial operator [] (int i)
    {
        return this->arr[i];
    }
};

int main()
{
    setlocale(LC_ALL, " Russian");
    ElementPolynomial p11(1, 1), p12(2, 2), p13(3, 3);

    ElementPolynomial p21(3, 1), p22(2, 2), p23(1, 3);
    ElementPolynomial p31(0, 1), p32(0, 2), p33(0, 3);
    vector<ElementPolynomial> polynomialArr1(3);
    vector<ElementPolynomial> polynomialArr2(3);
    vector<ElementPolynomial> polynomialArr3(3);
    polynomialArr1 = {p11, p12, p13};
    polynomialArr2 = {p21, p22, p23};
    polynomialArr3 = {p31, p32, p33};
    Polynomial pol1(polynomialArr1);
    Polynomial pol2(polynomialArr2);
    Polynomial pol3(polynomialArr3);
    cout << pol1[-1].getPower();
    //pol1.Print();
    //pol2.Print();


    pol1 = pol2; // Оператор =
    cout << "Оператор ==\n";
    pol1 == pol2 ? cout << "pol1 == pol2\n" : cout << "pol1 != pol2\n";
    cout << "Оператор !=\n";
    pol1 != pol2 ? cout << "pol1 != pol2\n" : cout << "pol1 == pol2\n";

    cout << "Оператор +\n";
    pol3 = pol1 + pol2;
    pol3.Print();
    cout << "Оператор -\n";
    pol3 = pol1 - pol2;
    pol3.Print();
    cout << "Оператор +=\n";
    pol3 += pol1;
    pol3.Print();
    cout << "Оператор -=\n";
    pol3 -= pol1;
    pol3.Print();

    cout << "Оператор *\n";
    pol3 = pol1 * pol1;
    pol3.Print();


    return 0;
}
