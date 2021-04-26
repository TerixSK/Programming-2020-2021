#include<iostream>
#include<fstream>
#include <utility>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;
ofstream fout("output.txt");
#define RightCenter RightPlane[1][1]->getRightPlane_piece()
#define LeftCenter LeftPlane[1][1]->getLeftPlane_piece()
#define UpCenter UpPlane[1][1]->getUpPlane_piece()
#define DownCenter DownPlane[1][1]->getDownPlane_piece()
#define FrontCenter FrontPlane[1][1]->getFrontPlane_piece()
#define BackCenter BackPlane[1][1]->getBackPlane_piece()

// Кубик, из которого состоит Кубик Рубик
class CubePiece
{
private:
    // Грани кубика. Видимые грани имеют цвет, а невидимые - чёрные
    string up_plane_piece = "black";
    string down_plane_piece = "black";
    string left_plane_piece = "black";
    string right_plane_piece = "black";
    string front_plane_piece = "black";
    string back_plane_piece = "black";
public:
    // Конструкторы
    CubePiece() = default;
    CubePiece(string up, string down, string left, string right, string front, string back)
    {
        this->up_plane_piece = move(up);
        this->down_plane_piece = move(down);
        this->left_plane_piece = move(left);
        this->right_plane_piece = move(right);
        this->front_plane_piece = move(front);
        this->back_plane_piece = move(back);
    }
    // Операторы
    CubePiece operator = (const CubePiece& cube_piece)
    {
        this->up_plane_piece = cube_piece.up_plane_piece;
        this->down_plane_piece = cube_piece.down_plane_piece;
        this->left_plane_piece = cube_piece.left_plane_piece;
        this->right_plane_piece = cube_piece.right_plane_piece;
        this->front_plane_piece = cube_piece.front_plane_piece;
        this->back_plane_piece = cube_piece.back_plane_piece;

        return *this;
    }

    // Гетеры
    [[nodiscard]] string getUpPlane_piece() const
    {
        return this->up_plane_piece;
    }
    [[nodiscard]] string getDownPlane_piece() const
    {
        return this->down_plane_piece;
    }
    [[nodiscard]] string getLeftPlane_piece() const
    {
        return this->left_plane_piece;
    }
    [[nodiscard]] string getRightPlane_piece() const
    {
        return this->right_plane_piece;
    }
    [[nodiscard]] string getFrontPlane_piece() const
    {
        return this->front_plane_piece;
    }
    [[nodiscard]] string getBackPlane_piece() const
    {
        return this->back_plane_piece;
    }

    // Сеттеры
    void setUpPlane_piece(string color)
    {
        this->up_plane_piece = move(color);
    }
    void setDownPlane_piece(string color)
    {
        this->down_plane_piece = move(color);
    }
    void setLeftPlane_piece(string color)
    {
        this->left_plane_piece = move(color);
    }
    void setRightPlane_piece(string color)
    {
        this->right_plane_piece = move(color);
    }
    void setFrontPlane_piece(string color)
    {
        this->front_plane_piece = move(color);
    }
    void setBackPlane_piece(string color)
    {
        this->back_plane_piece = move(color);
    }
};

// Кубик Рубик
class CubeRubik
{
private:
    // Мы храним сам куб в виде трёхмерного вектора ( Координаты (1, 1, 1) имеет центральный кубик,
    // координаты (0, 0, 0) имеет левый нижний задний кубик, координаты (2, 2, 2) имеет правый верхний передний кубик)
    vector<vector<vector<CubePiece>>> arr;
    // Грани кубика. В скобочках цвет при собранном положении Кубика Рубика
    vector<vector<CubePiece*>> UpPlane; // (белый)
    vector<vector<CubePiece*>> DownPlane; // (жёлтый)
    vector<vector<CubePiece*>> LeftPlane; // (оранжевый)
    vector<vector<CubePiece*>> RightPlane; // (красный)
    vector<vector<CubePiece*>> FrontPlane; // (зелёный)
    vector<vector<CubePiece*>> BackPlane; // (синий)

    unsigned int FirstStepCounter = 0, SecondStepCounter = 0, ThirdStepCounter = 0,
            FourthStepCounter = 0, FifthStepCounter = 0, SixthStepCounter = 0, SeventhStepCounter = 0;

    // Распределение кубиков из трёхмерного вектора по граням
    void PushInPlaneVector()
    {
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[i][2][j]);
            UpPlane.push_back(temp);
        }
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[i][0][j]);
            DownPlane.push_back(temp);
        }
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[2][i][j]);
            LeftPlane.push_back(temp);
        }
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[0][i][j]);
            RightPlane.push_back(temp);
        }
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[i][j][2]);
            FrontPlane.push_back(temp);
        }
        for (int i = 0; i < 3; i++)
        {
            vector<CubePiece*> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(&arr[i][j][0]);
            BackPlane.push_back(temp);
        }
    }

    // Вспомогательная функция для вывода развёртки
    static void PrintWord(const string &color)
    {
        int width_field = 6;
        string v(width_field, ' '), v1;

        int l_word = color.size(), l_field = v.size() + 1, step = (l_field - l_word) / 2;

        for (int i = 0; i < l_word; i++)
            v1.push_back(color[i]);

        copy(v1.begin(), v1.end(), v.begin() + step);
        for (auto x : v)
            cout << x;

        cout << " ";
    }
    static void PrintWordInFile(const string &color)
    {
        int width_field = 6;
        string v(width_field, ' '), v1;

        int l_word = color.size(), l_field = v.size() + 1, step = (l_field - l_word) / 2;

        for (int i = 0; i < l_word; i++)
            v1.push_back(color[i]);

        copy(v1.begin(), v1.end(), v.begin() + step);
        for (auto x : v)
            fout << x;

        fout << " ";
    }

public:

    // Конструктор
    CubeRubik()
    {
        this->arr.resize(3, vector<vector<CubePiece>>(3, vector<CubePiece>(3)));
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    arr[i][j][k] = CubePiece();
        PushInPlaneVector();
        CreateCubeRubik();
        PrintCubeRubik();
    }

    // Функция для расскраскки граней изночальными цветами
    void CreateCubeRubik()
    {
        arr[1][1][1] = CubePiece();
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                UpPlane[i][j]->setUpPlane_piece("white");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                DownPlane[i][j]->setDownPlane_piece("yellow");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                LeftPlane[i][j]->setLeftPlane_piece("orange");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                RightPlane[i][j]->setRightPlane_piece("red");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                FrontPlane[i][j]->setFrontPlane_piece("green");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                BackPlane[i][j]->setBackPlane_piece("blue");
    }

    // Функция обновления кубика рубика
    void UpdateCubeRubik()
    {
        this->arr.resize(3, vector<vector<CubePiece>>(3, vector<CubePiece>(3)));
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    arr[i][j][k] = CubePiece();
        PushInPlaneVector();
        CreateCubeRubik();
        PrintCubeRubik();
    }

    //Ввод развёртки из файла
    void ReadFromFileCubeRubik(const string& file)
    {
        ifstream fin(file);
        string color;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                UpPlane[i][j]->setUpPlane_piece(color);
            }
        }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                LeftPlane[i][j]->setLeftPlane_piece(color);
            }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                FrontPlane[i][j]->setFrontPlane_piece(color);
            }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                RightPlane[i][j]->setRightPlane_piece(color);
            }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                BackPlane[i][j]->setBackPlane_piece(color);
            }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                fin >> color;
                DownPlane[i][j]->setDownPlane_piece(color);
            }
    }

    // Геттеры граней
    [[nodiscard]] vector<vector<vector<CubePiece>>> getVector() const
    {
        return this->arr;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getUpPlane() const
    {
        return this->UpPlane;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getDownPlane() const
    {
        return this->DownPlane;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getLeftPlane() const
    {
        return this->LeftPlane;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getRightPlane() const
    {
        return this->RightPlane;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getFrontPlane() const
    {
        return this->FrontPlane;
    }
    [[nodiscard]] vector<vector<CubePiece*>> getBackPlane() const
    {
        return this->BackPlane;
    }

    // Повороты
    void RotateUpPlane(char where)
    {
        if (where == '+')
        {
            string right_color_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_color_0_1 = RightPlane[0][1]->getRightPlane_piece();
            string right_color_0_2 = RightPlane[0][2]->getRightPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_0_1 = FrontPlane[0][1]->getFrontPlane_piece();
            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_0_1 = LeftPlane[0][1]->getLeftPlane_piece();
            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_0_1 = BackPlane[0][1]->getBackPlane_piece();
            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();

            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_0_1 = UpPlane[0][1]->getUpPlane_piece();
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();
            string up_color_1_0 = UpPlane[1][0]->getUpPlane_piece();
            string up_color_1_2 = UpPlane[1][2]->getUpPlane_piece();
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();
            string up_color_2_1 = UpPlane[2][1]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            UpPlane[0][0]->setUpPlane_piece(up_color_2_0);
            UpPlane[0][1]->setUpPlane_piece(up_color_1_0);
            UpPlane[0][2]->setUpPlane_piece(up_color_0_0);
            UpPlane[1][0]->setUpPlane_piece(up_color_2_1);
            UpPlane[1][2]->setUpPlane_piece(up_color_0_1);
            UpPlane[2][0]->setUpPlane_piece(up_color_2_2);
            UpPlane[2][1]->setUpPlane_piece(up_color_1_2);
            UpPlane[2][2]->setUpPlane_piece(up_color_0_2);

            RightPlane[0][0]->setRightPlane_piece(back_color_0_0);
            RightPlane[0][1]->setRightPlane_piece(back_color_0_1);
            RightPlane[0][2]->setRightPlane_piece(back_color_0_2);

            FrontPlane[0][0]->setFrontPlane_piece(right_color_0_0);
            FrontPlane[0][1]->setFrontPlane_piece(right_color_0_1);
            FrontPlane[0][2]->setFrontPlane_piece(right_color_0_2);

            LeftPlane[0][0]->setLeftPlane_piece(front_color_0_0);
            LeftPlane[0][1]->setLeftPlane_piece(front_color_0_1);
            LeftPlane[0][2]->setLeftPlane_piece(front_color_0_2);

            BackPlane[0][0]->setBackPlane_piece(left_color_0_0);
            BackPlane[0][1]->setBackPlane_piece(left_color_0_1);
            BackPlane[0][2]->setBackPlane_piece(left_color_0_2);
        }
        else if (where == '-')
        {
            string right_color_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_color_0_1 = RightPlane[0][1]->getRightPlane_piece();
            string right_color_0_2 = RightPlane[0][2]->getRightPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_0_1 = FrontPlane[0][1]->getFrontPlane_piece();
            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_0_1 = LeftPlane[0][1]->getLeftPlane_piece();
            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_0_1 = BackPlane[0][1]->getBackPlane_piece();
            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();

            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_0_1 = UpPlane[0][1]->getUpPlane_piece();
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();
            string up_color_1_0 = UpPlane[1][0]->getUpPlane_piece();
            string up_color_1_2 = UpPlane[1][2]->getUpPlane_piece();
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();
            string up_color_2_1 = UpPlane[2][1]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            UpPlane[0][0]->setUpPlane_piece(up_color_0_2);
            UpPlane[0][1]->setUpPlane_piece(up_color_1_2);
            UpPlane[0][2]->setUpPlane_piece(up_color_2_2);
            UpPlane[1][0]->setUpPlane_piece(up_color_0_1);
            UpPlane[1][2]->setUpPlane_piece(up_color_2_1);
            UpPlane[2][0]->setUpPlane_piece(up_color_0_0);
            UpPlane[2][1]->setUpPlane_piece(up_color_1_0);
            UpPlane[2][2]->setUpPlane_piece(up_color_2_0);

            RightPlane[0][0]->setRightPlane_piece(front_color_0_0);
            RightPlane[0][1]->setRightPlane_piece(front_color_0_1);
            RightPlane[0][2]->setRightPlane_piece(front_color_0_2);

            FrontPlane[0][0]->setFrontPlane_piece(left_color_0_0);
            FrontPlane[0][1]->setFrontPlane_piece(left_color_0_1);
            FrontPlane[0][2]->setFrontPlane_piece(left_color_0_2);

            LeftPlane[0][0]->setLeftPlane_piece(back_color_0_0);
            LeftPlane[0][1]->setLeftPlane_piece(back_color_0_1);
            LeftPlane[0][2]->setLeftPlane_piece(back_color_0_2);

            BackPlane[0][0]->setBackPlane_piece(right_color_0_0);
            BackPlane[0][1]->setBackPlane_piece(right_color_0_1);
            BackPlane[0][2]->setBackPlane_piece(right_color_0_2);
        }
    }
    void RotateDownPlane(char where)
    {
        if (where == '+')
        {
            string right_color_2_0 = RightPlane[2][0]->getRightPlane_piece();
            string right_color_2_1 = RightPlane[2][1]->getRightPlane_piece();
            string right_color_2_2 = RightPlane[2][2]->getRightPlane_piece();

            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();
            string front_color_2_1 = FrontPlane[2][1]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();
            string left_color_2_1 = LeftPlane[2][1]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();
            string back_color_2_1 = BackPlane[2][1]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_0_1 = DownPlane[0][1]->getDownPlane_piece();
            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();
            string down_color_1_0 = DownPlane[1][0]->getDownPlane_piece();
            string down_color_1_2 = DownPlane[1][2]->getDownPlane_piece();
            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();
            string down_color_2_1 = DownPlane[2][1]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            DownPlane[0][0]->setDownPlane_piece(down_color_2_0);
            DownPlane[0][1]->setDownPlane_piece(down_color_1_0);
            DownPlane[0][2]->setDownPlane_piece(down_color_0_0);
            DownPlane[1][0]->setDownPlane_piece(down_color_2_1);
            DownPlane[1][2]->setDownPlane_piece(down_color_0_1);
            DownPlane[2][0]->setDownPlane_piece(down_color_2_2);
            DownPlane[2][1]->setDownPlane_piece(down_color_1_2);
            DownPlane[2][2]->setDownPlane_piece(down_color_0_2);

            RightPlane[2][0]->setRightPlane_piece(front_color_2_0);
            RightPlane[2][1]->setRightPlane_piece(front_color_2_1);
            RightPlane[2][2]->setRightPlane_piece(front_color_2_2);

            FrontPlane[2][0]->setFrontPlane_piece(left_color_2_0);
            FrontPlane[2][1]->setFrontPlane_piece(left_color_2_1);
            FrontPlane[2][2]->setFrontPlane_piece(left_color_2_2);

            LeftPlane[2][0]->setLeftPlane_piece(back_color_2_0);
            LeftPlane[2][1]->setLeftPlane_piece(back_color_2_1);
            LeftPlane[2][2]->setLeftPlane_piece(back_color_2_2);

            BackPlane[2][0]->setBackPlane_piece(right_color_2_0);
            BackPlane[2][1]->setBackPlane_piece(right_color_2_1);
            BackPlane[2][2]->setBackPlane_piece(right_color_2_2);
        }
        else if (where == '-')
        {
            string right_color_2_0 = RightPlane[2][0]->getRightPlane_piece();
            string right_color_2_1 = RightPlane[2][1]->getRightPlane_piece();
            string right_color_2_2 = RightPlane[2][2]->getRightPlane_piece();

            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();
            string front_color_2_1 = FrontPlane[2][1]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();
            string left_color_2_1 = LeftPlane[2][1]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();
            string back_color_2_1 = BackPlane[2][1]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_0_1 = DownPlane[0][1]->getDownPlane_piece();
            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();
            string down_color_1_0 = DownPlane[1][0]->getDownPlane_piece();
            string down_color_1_2 = DownPlane[1][2]->getDownPlane_piece();
            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();
            string down_color_2_1 = DownPlane[2][1]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            DownPlane[0][0]->setDownPlane_piece(down_color_0_2);
            DownPlane[0][1]->setDownPlane_piece(down_color_1_2);
            DownPlane[0][2]->setDownPlane_piece(down_color_2_2);
            DownPlane[1][0]->setDownPlane_piece(down_color_0_1);
            DownPlane[1][2]->setDownPlane_piece(down_color_2_1);
            DownPlane[2][0]->setDownPlane_piece(down_color_0_0);
            DownPlane[2][1]->setDownPlane_piece(down_color_1_0);
            DownPlane[2][2]->setDownPlane_piece(down_color_2_0);

            RightPlane[2][0]->setRightPlane_piece(back_color_2_0);
            RightPlane[2][1]->setRightPlane_piece(back_color_2_1);
            RightPlane[2][2]->setRightPlane_piece(back_color_2_2);

            FrontPlane[2][0]->setFrontPlane_piece(right_color_2_0);
            FrontPlane[2][1]->setFrontPlane_piece(right_color_2_1);
            FrontPlane[2][2]->setFrontPlane_piece(right_color_2_2);

            LeftPlane[2][0]->setLeftPlane_piece(front_color_2_0);
            LeftPlane[2][1]->setLeftPlane_piece(front_color_2_1);
            LeftPlane[2][2]->setLeftPlane_piece(front_color_2_2);

            BackPlane[2][0]->setBackPlane_piece(left_color_2_0);
            BackPlane[2][1]->setBackPlane_piece(left_color_2_1);
            BackPlane[2][2]->setBackPlane_piece(left_color_2_2);
        }
    }
    void RotateLeftPlane(char where)
    {
        if (where == '+')
        {
            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_1_0 = UpPlane[1][0]->getUpPlane_piece();
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_1_0 = FrontPlane[1][0]->getFrontPlane_piece();
            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_1_0 = DownPlane[1][0]->getDownPlane_piece();
            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();

            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();
            string back_color_1_2 = BackPlane[1][2]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_0_1 = LeftPlane[0][1]->getLeftPlane_piece();
            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();
            string left_color_1_0 = LeftPlane[1][0]->getLeftPlane_piece();
            string left_color_1_2 = LeftPlane[1][2]->getLeftPlane_piece();
            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();
            string left_color_2_1 = LeftPlane[2][1]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            LeftPlane[0][0]->setLeftPlane_piece(left_color_2_0);
            LeftPlane[0][1]->setLeftPlane_piece(left_color_1_0);
            LeftPlane[0][2]->setLeftPlane_piece(left_color_0_0);
            LeftPlane[1][0]->setLeftPlane_piece(left_color_2_1);
            LeftPlane[1][2]->setLeftPlane_piece(left_color_0_1);
            LeftPlane[2][0]->setLeftPlane_piece(left_color_2_2);
            LeftPlane[2][1]->setLeftPlane_piece(left_color_1_2);
            LeftPlane[2][2]->setLeftPlane_piece(left_color_0_2);

            UpPlane[0][0]->setUpPlane_piece(back_color_2_2);
            UpPlane[1][0]->setUpPlane_piece(back_color_1_2);
            UpPlane[2][0]->setUpPlane_piece(back_color_0_2);

            FrontPlane[0][0]->setFrontPlane_piece(up_color_0_0);
            FrontPlane[1][0]->setFrontPlane_piece(up_color_1_0);
            FrontPlane[2][0]->setFrontPlane_piece(up_color_2_0);

            DownPlane[0][0]->setDownPlane_piece(front_color_0_0);
            DownPlane[1][0]->setDownPlane_piece(front_color_1_0);
            DownPlane[2][0]->setDownPlane_piece(front_color_2_0);

            BackPlane[0][2]->setBackPlane_piece(down_color_2_0);
            BackPlane[1][2]->setBackPlane_piece(down_color_1_0);
            BackPlane[2][2]->setBackPlane_piece(down_color_0_0);
        }
        else if (where == '-')
        {
            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_1_0 = UpPlane[1][0]->getUpPlane_piece();
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_1_0 = FrontPlane[1][0]->getFrontPlane_piece();
            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_1_0 = DownPlane[1][0]->getDownPlane_piece();
            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();

            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();
            string back_color_1_2 = BackPlane[1][2]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_0_1 = LeftPlane[0][1]->getLeftPlane_piece();
            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();
            string left_color_1_0 = LeftPlane[1][0]->getLeftPlane_piece();
            string left_color_1_2 = LeftPlane[1][2]->getLeftPlane_piece();
            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();
            string left_color_2_1 = LeftPlane[2][1]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            LeftPlane[0][0]->setLeftPlane_piece(left_color_0_2);
            LeftPlane[0][1]->setLeftPlane_piece(left_color_1_2);
            LeftPlane[0][2]->setLeftPlane_piece(left_color_2_2);
            LeftPlane[1][0]->setLeftPlane_piece(left_color_0_1);
            LeftPlane[1][2]->setLeftPlane_piece(left_color_2_1);
            LeftPlane[2][0]->setLeftPlane_piece(left_color_0_0);
            LeftPlane[2][1]->setLeftPlane_piece(left_color_1_0);
            LeftPlane[2][2]->setLeftPlane_piece(left_color_2_0);

            UpPlane[0][0]->setUpPlane_piece(front_color_0_0);
            UpPlane[1][0]->setUpPlane_piece(front_color_1_0);
            UpPlane[2][0]->setUpPlane_piece(front_color_2_0);

            FrontPlane[0][0]->setFrontPlane_piece(down_color_0_0);
            FrontPlane[1][0]->setFrontPlane_piece(down_color_1_0);
            FrontPlane[2][0]->setFrontPlane_piece(down_color_2_0);

            DownPlane[0][0]->setDownPlane_piece(back_color_2_2);
            DownPlane[1][0]->setDownPlane_piece(back_color_1_2);
            DownPlane[2][0]->setDownPlane_piece(back_color_0_2);

            BackPlane[0][2]->setBackPlane_piece(up_color_2_0);
            BackPlane[1][2]->setBackPlane_piece(up_color_1_0);
            BackPlane[2][2]->setBackPlane_piece(up_color_0_0);
        }
    }
    void RotateRightPlane(char where)
    {
        if (where == '+')
        {
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();
            string up_color_1_2 = UpPlane[1][2]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();
            string front_color_1_2 = FrontPlane[1][2]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();
            string down_color_1_2 = DownPlane[1][2]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_1_0 = BackPlane[1][0]->getBackPlane_piece();
            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();

            string right_color_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_color_0_1 = RightPlane[0][1]->getRightPlane_piece();
            string right_color_0_2 = RightPlane[0][2]->getRightPlane_piece();
            string right_color_1_0 = RightPlane[1][0]->getRightPlane_piece();
            string right_color_1_2 = RightPlane[1][2]->getRightPlane_piece();
            string right_color_2_0 = RightPlane[2][0]->getRightPlane_piece();
            string right_color_2_1 = RightPlane[2][1]->getRightPlane_piece();
            string right_color_2_2 = RightPlane[2][2]->getRightPlane_piece();

            RightPlane[0][0]->setRightPlane_piece(right_color_2_0);
            RightPlane[0][1]->setRightPlane_piece(right_color_1_0);
            RightPlane[0][2]->setRightPlane_piece(right_color_0_0);
            RightPlane[1][0]->setRightPlane_piece(right_color_2_1);
            RightPlane[1][2]->setRightPlane_piece(right_color_0_1);
            RightPlane[2][0]->setRightPlane_piece(right_color_2_2);
            RightPlane[2][1]->setRightPlane_piece(right_color_1_2);
            RightPlane[2][2]->setRightPlane_piece(right_color_0_2);

            UpPlane[0][2]->setUpPlane_piece(front_color_0_2);
            UpPlane[1][2]->setUpPlane_piece(front_color_1_2);
            UpPlane[2][2]->setUpPlane_piece(front_color_2_2);

            FrontPlane[0][2]->setFrontPlane_piece(down_color_0_2);
            FrontPlane[1][2]->setFrontPlane_piece(down_color_1_2);
            FrontPlane[2][2]->setFrontPlane_piece(down_color_2_2);

            DownPlane[0][2]->setDownPlane_piece(back_color_2_0);
            DownPlane[1][2]->setDownPlane_piece(back_color_1_0);
            DownPlane[2][2]->setDownPlane_piece(back_color_0_0);

            BackPlane[0][0]->setBackPlane_piece(up_color_2_2);
            BackPlane[1][0]->setBackPlane_piece(up_color_1_2);
            BackPlane[2][0]->setBackPlane_piece(up_color_0_2);
        }
        else if (where == '-')
        {
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();
            string up_color_1_2 = UpPlane[1][2]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();
            string front_color_1_2 = FrontPlane[1][2]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();
            string down_color_1_2 = DownPlane[1][2]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_1_0 = BackPlane[1][0]->getBackPlane_piece();
            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();

            string right_color_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_color_0_1 = RightPlane[0][1]->getRightPlane_piece();
            string right_color_0_2 = RightPlane[0][2]->getRightPlane_piece();
            string right_color_1_0 = RightPlane[1][0]->getRightPlane_piece();
            string right_color_1_2 = RightPlane[1][2]->getRightPlane_piece();
            string right_color_2_0 = RightPlane[2][0]->getRightPlane_piece();
            string right_color_2_1 = RightPlane[2][1]->getRightPlane_piece();
            string right_color_2_2 = RightPlane[2][2]->getRightPlane_piece();

            RightPlane[0][0]->setRightPlane_piece(right_color_0_2);
            RightPlane[0][1]->setRightPlane_piece(right_color_1_2);
            RightPlane[0][2]->setRightPlane_piece(right_color_2_2);
            RightPlane[1][0]->setRightPlane_piece(right_color_0_1);
            RightPlane[1][2]->setRightPlane_piece(right_color_2_1);
            RightPlane[2][0]->setRightPlane_piece(right_color_0_0);
            RightPlane[2][1]->setRightPlane_piece(right_color_1_0);
            RightPlane[2][2]->setRightPlane_piece(right_color_2_0);

            UpPlane[0][2]->setUpPlane_piece(back_color_2_0);
            UpPlane[1][2]->setUpPlane_piece(back_color_1_0);
            UpPlane[2][2]->setUpPlane_piece(back_color_0_0);

            FrontPlane[0][2]->setFrontPlane_piece(up_color_0_2);
            FrontPlane[1][2]->setFrontPlane_piece(up_color_1_2);
            FrontPlane[2][2]->setFrontPlane_piece(up_color_2_2);

            DownPlane[0][2]->setDownPlane_piece(front_color_0_2);
            DownPlane[1][2]->setDownPlane_piece(front_color_1_2);
            DownPlane[2][2]->setDownPlane_piece(front_color_2_2);

            BackPlane[0][0]->setBackPlane_piece(down_color_2_2);
            BackPlane[1][0]->setBackPlane_piece(down_color_1_2);
            BackPlane[2][0]->setBackPlane_piece(down_color_0_2);
        }
    }
    void RotateFrontPlane(char where)
    {
        if (where == '+')
        {
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();
            string up_color_2_1 = UpPlane[2][1]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            string right_up_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_up_1_0 = RightPlane[1][0]->getRightPlane_piece();
            string right_up_2_0 = RightPlane[2][0]->getRightPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_0_1 = DownPlane[0][1]->getDownPlane_piece();
            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();

            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();
            string left_color_1_2 = LeftPlane[1][2]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_0_1 = FrontPlane[0][1]->getFrontPlane_piece();
            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();
            string front_color_1_0 = FrontPlane[1][0]->getFrontPlane_piece();
            string front_color_1_2 = FrontPlane[1][2]->getFrontPlane_piece();
            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();
            string front_color_2_1 = FrontPlane[2][1]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            FrontPlane[0][0]->setFrontPlane_piece(front_color_2_0);
            FrontPlane[0][1]->setFrontPlane_piece(front_color_1_0);
            FrontPlane[0][2]->setFrontPlane_piece(front_color_0_0);
            FrontPlane[1][0]->setFrontPlane_piece(front_color_2_1);
            FrontPlane[1][2]->setFrontPlane_piece(front_color_0_1);
            FrontPlane[2][0]->setFrontPlane_piece(front_color_2_2);
            FrontPlane[2][1]->setFrontPlane_piece(front_color_1_2);
            FrontPlane[2][2]->setFrontPlane_piece(front_color_0_2);

            UpPlane[2][0]->setUpPlane_piece(left_color_2_2);
            UpPlane[2][1]->setUpPlane_piece(left_color_1_2);
            UpPlane[2][2]->setUpPlane_piece(left_color_0_2);

            RightPlane[0][0]->setRightPlane_piece(up_color_2_0);
            RightPlane[1][0]->setRightPlane_piece(up_color_2_1);
            RightPlane[2][0]->setRightPlane_piece(up_color_2_2);

            DownPlane[0][0]->setDownPlane_piece(right_up_2_0);
            DownPlane[0][1]->setDownPlane_piece(right_up_1_0);
            DownPlane[0][2]->setDownPlane_piece(right_up_0_0);

            LeftPlane[0][2]->setLeftPlane_piece(down_color_0_0);
            LeftPlane[1][2]->setLeftPlane_piece(down_color_0_1);
            LeftPlane[2][2]->setLeftPlane_piece(down_color_0_2);
        }
        else if (where == '-')
        {
            string up_color_2_0 = UpPlane[2][0]->getUpPlane_piece();
            string up_color_2_1 = UpPlane[2][1]->getUpPlane_piece();
            string up_color_2_2 = UpPlane[2][2]->getUpPlane_piece();

            string right_up_0_0 = RightPlane[0][0]->getRightPlane_piece();
            string right_up_1_0 = RightPlane[1][0]->getRightPlane_piece();
            string right_up_2_0 = RightPlane[2][0]->getRightPlane_piece();

            string down_color_0_0 = DownPlane[0][0]->getDownPlane_piece();
            string down_color_0_1 = DownPlane[0][1]->getDownPlane_piece();
            string down_color_0_2 = DownPlane[0][2]->getDownPlane_piece();

            string left_color_0_2 = LeftPlane[0][2]->getLeftPlane_piece();
            string left_color_1_2 = LeftPlane[1][2]->getLeftPlane_piece();
            string left_color_2_2 = LeftPlane[2][2]->getLeftPlane_piece();

            string front_color_0_0 = FrontPlane[0][0]->getFrontPlane_piece();
            string front_color_0_1 = FrontPlane[0][1]->getFrontPlane_piece();
            string front_color_0_2 = FrontPlane[0][2]->getFrontPlane_piece();
            string front_color_1_0 = FrontPlane[1][0]->getFrontPlane_piece();
            string front_color_1_2 = FrontPlane[1][2]->getFrontPlane_piece();
            string front_color_2_0 = FrontPlane[2][0]->getFrontPlane_piece();
            string front_color_2_1 = FrontPlane[2][1]->getFrontPlane_piece();
            string front_color_2_2 = FrontPlane[2][2]->getFrontPlane_piece();

            FrontPlane[0][0]->setFrontPlane_piece(front_color_0_2);
            FrontPlane[0][1]->setFrontPlane_piece(front_color_1_2);
            FrontPlane[0][2]->setFrontPlane_piece(front_color_2_2);
            FrontPlane[1][0]->setFrontPlane_piece(front_color_0_1);
            FrontPlane[1][2]->setFrontPlane_piece(front_color_2_1);
            FrontPlane[2][0]->setFrontPlane_piece(front_color_0_0);
            FrontPlane[2][1]->setFrontPlane_piece(front_color_1_0);
            FrontPlane[2][2]->setFrontPlane_piece(front_color_2_0);

            UpPlane[2][0]->setUpPlane_piece(right_up_0_0);
            UpPlane[2][1]->setUpPlane_piece(right_up_1_0);
            UpPlane[2][2]->setUpPlane_piece(right_up_2_0);

            RightPlane[0][0]->setRightPlane_piece(down_color_0_2);
            RightPlane[1][0]->setRightPlane_piece(down_color_0_1);
            RightPlane[2][0]->setRightPlane_piece(down_color_0_0);

            DownPlane[0][0]->setDownPlane_piece(left_color_0_2);
            DownPlane[0][1]->setDownPlane_piece(left_color_1_2);
            DownPlane[0][2]->setDownPlane_piece(left_color_2_2);

            LeftPlane[0][2]->setLeftPlane_piece(up_color_2_2);
            LeftPlane[1][2]->setLeftPlane_piece(up_color_2_1);
            LeftPlane[2][2]->setLeftPlane_piece(up_color_2_0);
        }
    }
    void RotateBackPlane(char where)
    {
        if (where == '+')
        {
            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_0_1 = UpPlane[0][1]->getUpPlane_piece();
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();

            string right_up_0_2 = RightPlane[0][2]->getRightPlane_piece();
            string right_up_1_2 = RightPlane[1][2]->getRightPlane_piece();
            string right_up_2_2 = RightPlane[2][2]->getRightPlane_piece();

            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();
            string down_color_2_1 = DownPlane[2][1]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_1_0 = LeftPlane[1][0]->getLeftPlane_piece();
            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_0_1 = BackPlane[0][1]->getBackPlane_piece();
            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();
            string back_color_1_0 = BackPlane[1][0]->getBackPlane_piece();
            string back_color_1_2 = BackPlane[1][2]->getBackPlane_piece();
            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();
            string back_color_2_1 = BackPlane[2][1]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            BackPlane[0][0]->setBackPlane_piece(back_color_2_0);
            BackPlane[0][1]->setBackPlane_piece(back_color_1_0);
            BackPlane[0][2]->setBackPlane_piece(back_color_0_0);
            BackPlane[1][0]->setBackPlane_piece(back_color_2_1);
            BackPlane[1][2]->setBackPlane_piece(back_color_0_1);
            BackPlane[2][0]->setBackPlane_piece(back_color_2_2);
            BackPlane[2][1]->setBackPlane_piece(back_color_1_2);
            BackPlane[2][2]->setBackPlane_piece(back_color_0_2);

            UpPlane[0][0]->setUpPlane_piece(right_up_0_2);
            UpPlane[0][1]->setUpPlane_piece(right_up_1_2);
            UpPlane[0][2]->setUpPlane_piece(right_up_2_2);

            RightPlane[0][2]->setRightPlane_piece(down_color_2_2);
            RightPlane[1][2]->setRightPlane_piece(down_color_2_1);
            RightPlane[2][2]->setRightPlane_piece(down_color_2_0);

            DownPlane[2][0]->setDownPlane_piece(left_color_0_0);
            DownPlane[2][1]->setDownPlane_piece(left_color_1_0);
            DownPlane[2][2]->setDownPlane_piece(left_color_2_0);

            LeftPlane[0][0]->setLeftPlane_piece(up_color_0_2);
            LeftPlane[1][0]->setLeftPlane_piece(up_color_0_1);
            LeftPlane[2][0]->setLeftPlane_piece(up_color_0_0);
        }
        else if (where == '-')
        {
            string up_color_0_0 = UpPlane[0][0]->getUpPlane_piece();
            string up_color_0_1 = UpPlane[0][1]->getUpPlane_piece();
            string up_color_0_2 = UpPlane[0][2]->getUpPlane_piece();

            string right_up_0_2 = RightPlane[0][2]->getRightPlane_piece();
            string right_up_1_2 = RightPlane[1][2]->getRightPlane_piece();
            string right_up_2_2 = RightPlane[2][2]->getRightPlane_piece();

            string down_color_2_0 = DownPlane[2][0]->getDownPlane_piece();
            string down_color_2_1 = DownPlane[2][1]->getDownPlane_piece();
            string down_color_2_2 = DownPlane[2][2]->getDownPlane_piece();

            string left_color_0_0 = LeftPlane[0][0]->getLeftPlane_piece();
            string left_color_1_0 = LeftPlane[1][0]->getLeftPlane_piece();
            string left_color_2_0 = LeftPlane[2][0]->getLeftPlane_piece();

            string back_color_0_0 = BackPlane[0][0]->getBackPlane_piece();
            string back_color_0_1 = BackPlane[0][1]->getBackPlane_piece();
            string back_color_0_2 = BackPlane[0][2]->getBackPlane_piece();
            string back_color_1_0 = BackPlane[1][0]->getBackPlane_piece();
            string back_color_1_2 = BackPlane[1][2]->getBackPlane_piece();
            string back_color_2_0 = BackPlane[2][0]->getBackPlane_piece();
            string back_color_2_1 = BackPlane[2][1]->getBackPlane_piece();
            string back_color_2_2 = BackPlane[2][2]->getBackPlane_piece();

            BackPlane[0][0]->setBackPlane_piece(back_color_0_2);
            BackPlane[0][1]->setBackPlane_piece(back_color_1_2);
            BackPlane[0][2]->setBackPlane_piece(back_color_2_2);
            BackPlane[1][0]->setBackPlane_piece(back_color_0_1);
            BackPlane[1][2]->setBackPlane_piece(back_color_2_1);
            BackPlane[2][0]->setBackPlane_piece(back_color_0_0);
            BackPlane[2][1]->setBackPlane_piece(back_color_1_0);
            BackPlane[2][2]->setBackPlane_piece(back_color_2_0);

            UpPlane[0][0]->setUpPlane_piece(left_color_2_0);
            UpPlane[0][1]->setUpPlane_piece(left_color_1_0);
            UpPlane[0][2]->setUpPlane_piece(left_color_0_0);

            RightPlane[0][2]->setRightPlane_piece(up_color_0_0);
            RightPlane[1][2]->setRightPlane_piece(up_color_0_1);
            RightPlane[2][2]->setRightPlane_piece(up_color_0_2);

            DownPlane[2][0]->setDownPlane_piece(right_up_2_2);
            DownPlane[2][1]->setDownPlane_piece(right_up_1_2);
            DownPlane[2][2]->setDownPlane_piece(right_up_0_2);

            LeftPlane[0][0]->setLeftPlane_piece(down_color_2_0);
            LeftPlane[1][0]->setLeftPlane_piece(down_color_2_1);
            LeftPlane[2][0]->setLeftPlane_piece(down_color_2_2);
        }
    }
    // Последовательный поворот
    void RotateMachineGun(string list) {
        int size = list.size();
        for (int i = 0; i < size; i += 2)
        {
            if (list[i] == 'U')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateUpPlane('+');
                }
                else
                {
                    RotateUpPlane('-');
                }
            }

            if (list[i] == 'D')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateDownPlane('+');
                }
                else
                {
                    RotateDownPlane('-');
                }
            }

            if (list[i] == 'L')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateLeftPlane('+');
                }
                else
                {
                    RotateLeftPlane('-');
                }
            }

            if (list[i] == 'R')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateRightPlane('+');
                }
                else
                {
                    RotateRightPlane('-');
                }
            }

            if (list[i] == 'F')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateFrontPlane('+');
                }
                else
                {
                    RotateFrontPlane('-');
                }
            }

            if (list[i] == 'B')
            {
                if (list[i + 1] == '+' || list[i + 1] == ' ')
                {
                    RotateBackPlane('+');
                }
                else
                {
                    RotateBackPlane('-');
                }
            }
        }
    }


    // Разборка кубика
    void Shuffle()
    {
        srand(time(NULL));
        int count_turns = 15 + rand() % 30;
        for (int i = 0; i < count_turns; i++)
        {
            int temp = 1 + rand() % 12;
            switch (temp)
            {
                case (1):
                {
                    RotateUpPlane('+');
                    break;
                }
                case (2):
                {
                    RotateUpPlane('-');
                    break;
                }
                case (3):
                {
                    RotateDownPlane('+');
                    break;
                }
                case (4):
                {
                    RotateDownPlane('-');
                    break;
                }
                case (5):
                {
                    RotateLeftPlane('+');
                    break;
                }
                case (6):
                {
                    RotateLeftPlane('-');
                    break;
                }
                case (7):
                {
                    RotateRightPlane('+');
                    break;
                }
                case (8):
                {
                    RotateRightPlane('-');
                    break;
                }
                case (9):
                {
                    RotateFrontPlane('+');
                    break;
                }
                case (10):
                {
                    RotateFrontPlane('-');
                    break;
                }
                case (11):
                {
                    RotateBackPlane('+');
                    break;
                }
                case (12):
                {
                    RotateBackPlane('-');
                    break;
                }
                default:
                    break;
            }
        }
    }

    // 1й цвет - низ
    // 2й цвет - фронт
    // 3й цвет - право
    // 4й цвет - зад
    // 5й цвет - лево
    // 6й цвет - верх

    //Алгоритм Сборки
    // проверки на собронность куба после определённого шага
    bool ifFirstStepCompleted() {
        bool isDownCompleted = (DownPlane[1][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[1][2]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[0][1]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][1]->getDownPlane_piece() == DownCenter);

        bool isLeftCompleted = LeftCenter == LeftPlane[2][1]->getLeftPlane_piece();
        bool isRightCompleted = RightCenter == RightPlane[2][1]->getRightPlane_piece();
        bool isFrontCompleted = FrontCenter == FrontPlane[2][1]->getFrontPlane_piece();
        bool isBackCompleted = BackCenter == BackPlane[2][1]->getBackPlane_piece();

        bool isCompleted =
                isDownCompleted && isLeftCompleted && isRightCompleted && isFrontCompleted && isBackCompleted;

        return isCompleted;
    }
    bool ifSecondStepCompleted() {

        bool isFirstCompleted = ifFirstStepCompleted();

        bool isDownCompleted = (DownPlane[0][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[0][2]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][2]->getDownPlane_piece() == DownCenter);

        bool isLeftCompleted = LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter;

        bool isRightCompleted = RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                                RightPlane[2][2]->getRightPlane_piece() == RightCenter;

        bool isFrontCompleted = FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter;

        bool isBackCompleted = BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                               BackPlane[2][2]->getBackPlane_piece() == BackCenter;

        bool isCompleted =
                isFirstCompleted && isDownCompleted && isLeftCompleted && isRightCompleted && isFrontCompleted &&
                isBackCompleted;

        return isCompleted;
    }
    bool ifThirdStepCompleted() {

        bool isSecondCompleted = ifSecondStepCompleted();

        bool isLeftCompleted = LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter;

        bool isFrontCompleted = FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter;

        bool isRightCompleted = RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                                RightPlane[1][2]->getRightPlane_piece() == RightCenter;

        bool isBackCompleted = BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                               BackPlane[1][2]->getBackPlane_piece() == BackCenter;

        bool isCompleted =
                isSecondCompleted && isLeftCompleted && isRightCompleted && isFrontCompleted && isBackCompleted;

        return isCompleted;
    }
    bool ifFourthStepCompleted() {
        bool isThirdCompleted = ifThirdStepCompleted();

        bool isUpCompleted =
                UpPlane[0][1]->getUpPlane_piece() == UpCenter && UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][2]->getUpPlane_piece() == UpCenter && UpPlane[2][1]->getUpPlane_piece() == UpCenter;

        bool isCompleted = isThirdCompleted && isUpCompleted;

        return isCompleted;
    }
    bool ifFifthStepCompleted() {
        bool isFourthCompleted = ifFourthStepCompleted();

        bool isLeftCompleted = LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter;

        bool isRightCompleted = RightPlane[0][1]->getRightPlane_piece() == RightCenter;

        bool isFrontCompleted = FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter;

        bool isBackCompleted = BackPlane[0][1]->getBackPlane_piece() == BackCenter;

        bool isCompleted =
                isFourthCompleted && isLeftCompleted && isRightCompleted && isFrontCompleted && isBackCompleted;

        return isCompleted;
    }
    bool ifSixthStepCompleted() {
        bool isFifthCompleted = ifFifthStepCompleted();

        bool isLeftCornersCompleted = LeftPlane[0][0]->getLeftPlane_piece() == LeftCenter ||
                                      LeftPlane[0][0]->getLeftPlane_piece() == BackCenter ||
                                      LeftPlane[0][0]->getLeftPlane_piece() == UpCenter &&
                                      LeftPlane[0][2]->getLeftPlane_piece() == LeftCenter ||
                                      LeftPlane[0][2]->getLeftPlane_piece() == FrontCenter ||
                                      LeftPlane[0][2]->getLeftPlane_piece() == UpCenter;

        bool isBackCornersCompleted = BackPlane[0][0]->getBackPlane_piece() == BackCenter ||
                                      BackPlane[0][0]->getBackPlane_piece() == RightCenter ||
                                      BackPlane[0][0]->getBackPlane_piece() == UpCenter &&
                                      BackPlane[0][2]->getBackPlane_piece() == BackCenter ||
                                      BackPlane[0][2]->getBackPlane_piece() == LeftCenter ||
                                      BackPlane[0][2]->getBackPlane_piece() == UpCenter;

        bool isRightCornersCompleted = RightPlane[0][0]->getRightPlane_piece() == RightCenter ||
                                       RightPlane[0][0]->getRightPlane_piece() == FrontCenter ||
                                       RightPlane[0][0]->getRightPlane_piece() == UpCenter &&
                                       RightPlane[0][2]->getRightPlane_piece() == RightCenter ||
                                       RightPlane[0][2]->getRightPlane_piece() == BackCenter ||
                                       RightPlane[0][2]->getRightPlane_piece() == UpCenter;

        bool isFrontCornersCompleted = FrontPlane[0][0]->getFrontPlane_piece() == FrontCenter ||
                                       FrontPlane[0][0]->getFrontPlane_piece() == LeftCenter ||
                                       FrontPlane[0][0]->getFrontPlane_piece() == UpCenter &&
                                       FrontPlane[0][2]->getFrontPlane_piece() == FrontCenter ||
                                       FrontPlane[0][2]->getFrontPlane_piece() == RightCenter ||
                                       FrontPlane[0][2]->getFrontPlane_piece() == UpCenter;

        bool isUpCornersCompleted =
                UpPlane[0][0]->getUpPlane_piece() != FrontCenter && UpPlane[0][0]->getUpPlane_piece() != RightCenter &&
                UpPlane[0][2]->getUpPlane_piece() != FrontCenter && UpPlane[0][2]->getUpPlane_piece() != LeftCenter &&
                UpPlane[2][0]->getUpPlane_piece() != BackCenter && UpPlane[2][0]->getUpPlane_piece() != RightCenter &&
                UpPlane[2][2]->getUpPlane_piece() != BackCenter && UpPlane[2][2]->getUpPlane_piece() != LeftCenter;

        bool isCompleted =
                isFifthCompleted && isLeftCornersCompleted && isRightCornersCompleted && isFrontCornersCompleted &&
                isBackCornersCompleted && isUpCornersCompleted;

        return isCompleted;
    }
    bool ifCubeCompleted() {
        bool isDownCompleted = (DownPlane[0][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[0][1]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[0][2]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[1][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[1][2]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][0]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][1]->getDownPlane_piece() == DownCenter) &&
                               (DownPlane[2][2]->getDownPlane_piece() == DownCenter);

        bool isLeftCompleted = LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[0][0]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                               LeftPlane[0][2]->getLeftPlane_piece() == LeftCenter;

        bool isFrontCompleted = FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[0][0]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter &&
                                FrontPlane[0][2]->getFrontPlane_piece() == FrontCenter;

        bool isRightCompleted = RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                                RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                                RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                                RightPlane[0][0]->getRightPlane_piece() == RightCenter &&
                                RightPlane[0][1]->getRightPlane_piece() == RightCenter &&
                                RightPlane[0][2]->getRightPlane_piece() == RightCenter;

        bool isBackCompleted = BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                               BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                               BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                               BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                               BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                               BackPlane[0][0]->getBackPlane_piece() == BackCenter &&
                               BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                               BackPlane[0][2]->getBackPlane_piece() == BackCenter;

        bool isUpCompleted = UpPlane[2][0]->getUpPlane_piece() == UpCenter &&
                             UpPlane[2][2]->getUpPlane_piece() == UpCenter &&
                             UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                             UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                             UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                             UpPlane[0][0]->getUpPlane_piece() == UpCenter &&
                             UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                             UpPlane[0][2]->getUpPlane_piece() == UpCenter;

        bool result = isDownCompleted && isBackCompleted && isFrontCompleted && isLeftCompleted && isRightCompleted &&
                      isUpCompleted;

        return result;

    }

    // шаги сборки
    void FirstStep(int count = 0) {
        if (FirstStepCounter++ > 100) {
            cout << "Простите, с кубиком что-то не так :( (1й шаг)" << endl;
            exit(0);
        } else if (!ifFirstStepCompleted()) {
            if (FrontCenter == FrontPlane[0][1]->getFrontPlane_piece() &&
                UpPlane[2][1]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("F+F+");
                cout << "F F ";
                FirstStep(0);
            }
            if (RightCenter == RightPlane[0][1]->getRightPlane_piece() &&
                UpPlane[1][2]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("R+R+");
                cout << "R R ";
                FirstStep(0);
            }
            if (LeftCenter == LeftPlane[0][1]->getLeftPlane_piece() &&
                UpPlane[1][0]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("L+L+");
                cout << "L L ";
                FirstStep(0);
            }
            if (BackCenter == BackPlane[0][1]->getBackPlane_piece() &&
                UpPlane[0][1]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("B+B+");
                cout << "B B ";
                FirstStep(0);
            }
            if (FrontPlane[0][1]->getFrontPlane_piece() == DownCenter &&
                UpPlane[2][1]->getUpPlane_piece() == FrontCenter) {
                RotateMachineGun("U-R-F+R+");
                cout << "U' R' F R ";
                FirstStep(0);
            }
            if (UpPlane[1][2]->getUpPlane_piece() == RightCenter &&
                RightPlane[0][1]->getRightPlane_piece() == DownCenter) {
                RotateMachineGun("U-B-R+B+");
                cout << "U' B' R B ";
                FirstStep(0);
            }
            if (LeftPlane[0][1]->getLeftPlane_piece() == DownCenter &&
                UpPlane[1][0]->getUpPlane_piece() == LeftCenter) {
                RotateMachineGun("U-F-L+F+");
                cout << "U' F' L F ";
                FirstStep(0);
            }
            if (BackPlane[0][1]->getBackPlane_piece() == DownCenter &&
                UpPlane[0][1]->getUpPlane_piece() == BackCenter) {
                RotateMachineGun("U-L-B+L+");
                cout << "U' L' B L ";
                FirstStep(0);
            }
            if (FrontPlane[1][2]->getFrontPlane_piece() == DownCenter) {
                RotateMachineGun("F-U-F+");
                cout << "F' U' F ";
                FirstStep(0);
            }
            if (RightPlane[1][2]->getRightPlane_piece() == DownCenter) {
                RotateMachineGun("R-U-R+");
                cout << "R' U' R ";
                FirstStep(0);
            }
            if (LeftPlane[1][2]->getLeftPlane_piece() == DownCenter) {
                RotateMachineGun("L-U-L+");
                cout << "L' U' L ";
                FirstStep(0);
            }
            if (BackPlane[1][2]->getBackPlane_piece() == DownCenter) {
                RotateMachineGun("B-U-B+");
                cout << "B' U' B ";
                FirstStep(0);
            }
            if (FrontPlane[1][0]->getFrontPlane_piece() == DownCenter) {
                RotateMachineGun("F+U-F-");
                cout << "F U' F' ";
                FirstStep(0);
            }
            if (RightPlane[1][0]->getRightPlane_piece() == DownCenter) {
                RotateMachineGun("R+U-R-");
                cout << "R U' R' ";
                FirstStep(0);
            }
            if (LeftPlane[1][0]->getLeftPlane_piece() == DownCenter) {
                RotateMachineGun("L+U-L-");
                cout << "L U' L' ";
                FirstStep(0);
            }
            if (BackPlane[1][0]->getBackPlane_piece() == DownCenter) {
                RotateMachineGun("B+U-B-");
                cout << "B U' B' ";
                FirstStep(0);
            }
            if (FrontPlane[2][1]->getFrontPlane_piece() == DownCenter) {
                RotateMachineGun("F+F+U-F+F+");
                cout << "F F U' F F ";
                FirstStep(0);
            }
            if (RightPlane[2][1]->getRightPlane_piece() == DownCenter) {
                RotateMachineGun("R+R+U-R+R+");
                cout << "R R U' R R ";
                FirstStep(0);
            }
            if (LeftPlane[2][1]->getLeftPlane_piece() == DownCenter) {
                RotateMachineGun("L+L+U-L+L+");
                cout << "L L U' L L ";
                FirstStep(0);
            }
            if (BackPlane[2][1]->getBackPlane_piece() == DownCenter) {
                RotateMachineGun("B+B+U-B+B+");
                cout << "B B U' B B ";
                FirstStep(0);
            }
            if (DownPlane[0][1]->getDownPlane_piece() == DownCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() != FrontCenter) {
                RotateMachineGun("F-F-U-F-F-");
                cout << "F' F' U' F' F' ";
                FirstStep(0);
            }
            if (DownPlane[1][2]->getDownPlane_piece() == DownCenter &&
                RightPlane[2][1]->getRightPlane_piece() != RightCenter) {
                RotateMachineGun("R-R-U-R-R-");
                cout << "R' R' U' R' R' ";
                FirstStep(0);
            }
            if (DownPlane[1][0]->getDownPlane_piece() == DownCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() != LeftCenter) {
                RotateMachineGun("L-L-U-L-L-");
                cout << "L' L' U' L' L' ";
                FirstStep(0);
            }
            if (DownPlane[2][1]->getDownPlane_piece() == DownCenter &&
                BackPlane[2][1]->getBackPlane_piece() != BackCenter) {
                RotateMachineGun("B-B-U-B-B-");
                cout << "B' B' U' B' B' ";
                FirstStep(0);
            }

            if (!ifFirstStepCompleted() && count < 4) {
                RotateMachineGun("U+");
                cout << "U ";
                FirstStep(++count);
            } else if (!ifFirstStepCompleted() && count == 4) {
                Shuffle();
                FirstStep(0);
            }
        }
    }

    void SecondStep(int count = 0) {
        if (SecondStepCounter++ > 80) {
            cout << "Простите, с кубиком что-то не так :( (2й шаг)" << endl;
            exit(0);
        } else if (!ifSecondStepCompleted()) {
            if (FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[0][2]->getFrontPlane_piece() == DownCenter &&
                UpPlane[2][2]->getUpPlane_piece() == FrontCenter &&
                RightPlane[0][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter) {
                RotateMachineGun("F-U-F+");
                cout << "F' U' F ";
                SecondStep(0);
            }
            if (FrontPlane[0][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                UpPlane[2][0]->getUpPlane_piece() == LeftCenter &&
                LeftPlane[0][2]->getLeftPlane_piece() == DownCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter) {
                RotateMachineGun("L-U-L+");
                cout << "L' U' L ";
                SecondStep(0);
            }
            if (BackPlane[0][0]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                UpPlane[0][2]->getUpPlane_piece() == RightCenter &&
                RightPlane[0][2]->getRightPlane_piece() == DownCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter) {
                RotateMachineGun("R-U-R+");
                cout << "R' U' R ";
                SecondStep(0);
            }
            if (BackPlane[0][2]->getBackPlane_piece() == DownCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                LeftPlane[0][0]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[0][0]->getUpPlane_piece() == BackCenter) {
                RotateMachineGun("B-U-B+");
                cout << "B' U' B ";
                SecondStep(0);
            }
            if (FrontPlane[0][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[0][0]->getRightPlane_piece() == DownCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                UpPlane[2][2]->getUpPlane_piece() == RightCenter) {
                RotateMachineGun("R+U+R-");
                cout << "R U R' ";
                SecondStep(0);
            }
            if (FrontPlane[0][0]->getFrontPlane_piece() == DownCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                UpPlane[2][0]->getUpPlane_piece() == FrontCenter &&
                LeftPlane[0][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter) {
                RotateMachineGun("F+U+F-");
                cout << "F U F' ";
                SecondStep(0);
            }
            if (BackPlane[0][0]->getBackPlane_piece() == DownCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                UpPlane[0][2]->getUpPlane_piece() == BackCenter &&
                RightPlane[0][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter) {
                RotateMachineGun("B+U+B-");
                cout << "B U B' ";
                SecondStep(0);
            }
            if (BackPlane[0][2]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                LeftPlane[0][0]->getLeftPlane_piece() == DownCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[0][0]->getUpPlane_piece() == LeftCenter) {
                RotateMachineGun("L+U+L-");
                cout << "L U L' ";
                SecondStep(0);
            }
            if (FrontPlane[0][2]->getFrontPlane_piece() == RightCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[0][0]->getRightPlane_piece() == FrontCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                UpPlane[2][2]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("R+U-R-U+U+R+U+R-");
                cout << "R U' R' U U R U R' ";
                SecondStep(0);
            }
            if (FrontPlane[0][0]->getFrontPlane_piece() == LeftCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                UpPlane[2][0]->getUpPlane_piece() == DownCenter &&
                RightPlane[0][2]->getRightPlane_piece() == FrontCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter) {
                RotateMachineGun("F+U-F-U+U+F+U+F-");
                cout << "F U' F' U U F U F' ";
                SecondStep(0);
            }
            if (BackPlane[0][0]->getBackPlane_piece() == RightCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                UpPlane[0][2]->getUpPlane_piece() == DownCenter &&
                RightPlane[0][2]->getRightPlane_piece() == BackCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter) {
                RotateMachineGun("B+U-B-U+U+B+U+B-");
                cout << "B U' B' U U B U B' ";
                SecondStep(0);
            }
            if (BackPlane[0][2]->getBackPlane_piece() == LeftCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                LeftPlane[0][0]->getLeftPlane_piece() == BackCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[0][0]->getUpPlane_piece() == DownCenter) {
                RotateMachineGun("L+U-L-U+U+L+U+L-");
                cout << "L U' L' U U L U L' ";
                SecondStep(0);
            }
            if (FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                (FrontPlane[2][2]->getFrontPlane_piece() == DownCenter ||
                 RightPlane[2][0]->getRightPlane_piece() == DownCenter)) {
                RotateMachineGun("F-U-F+");
                cout << "F' U' F ";
                SecondStep(0);
            }
            if (FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                (LeftPlane[2][2]->getLeftPlane_piece() == DownCenter ||
                 FrontPlane[2][0]->getFrontPlane_piece() == DownCenter)) {
                RotateMachineGun("L-U-L+");
                cout << "L' U' L ";
                SecondStep(0);
            }
            if (BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                (RightPlane[2][2]->getRightPlane_piece() == DownCenter ||
                 BackPlane[2][0]->getBackPlane_piece() == DownCenter)) {
                RotateMachineGun("R-U-R+");
                cout << "R' U' R ";
                SecondStep(0);
            }
            if (BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                (BackPlane[2][2]->getBackPlane_piece() == DownCenter ||
                 LeftPlane[2][0]->getLeftPlane_piece() == DownCenter)) {
                RotateMachineGun("B-U-B+");
                cout << "B' U' B ";
                SecondStep(0);
            }
            if (LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                DownPlane[0][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[0][1]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[2][1]->getDownPlane_piece() == DownCenter &&
                (FrontPlane[2][0]->getFrontPlane_piece() != FrontCenter ||
                 LeftPlane[2][2]->getLeftPlane_piece() != LeftCenter)) {
                RotateMachineGun("F+U-F-");
                cout << "F U' F' ";
                SecondStep(0); // попробовать поставить XOR
            }
            if (RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                DownPlane[0][1]->getDownPlane_piece() == DownCenter &&
                DownPlane[0][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[2][1]->getDownPlane_piece() == DownCenter &&
                (FrontPlane[2][2]->getFrontPlane_piece() != FrontCenter ||
                 RightPlane[2][0]->getRightPlane_piece() != RightCenter)) {
                RotateMachineGun("R+U-R-");
                cout << "R U' R' ";
                SecondStep(0); // попробовать поставить XOR
            }
            if (BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                DownPlane[2][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[2][1]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[0][1]->getDownPlane_piece() == DownCenter &&
                (BackPlane[2][0]->getBackPlane_piece() != BackCenter ||
                 RightPlane[2][2]->getRightPlane_piece() != RightCenter)) {
                RotateMachineGun("B+U-B-");
                cout << "B U' B' ";
                SecondStep(0); // попробовать поставить XOR
            }
            if (BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                DownPlane[2][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[2][1]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][2]->getDownPlane_piece() == DownCenter &&
                DownPlane[1][0]->getDownPlane_piece() == DownCenter &&
                DownPlane[0][1]->getDownPlane_piece() == DownCenter &&
                (BackPlane[2][2]->getBackPlane_piece() != BackCenter ||
                 LeftPlane[2][0]->getLeftPlane_piece() != LeftCenter)) {
                RotateMachineGun("L+U-L-");
                cout << "L U' L' ";
                SecondStep(0);
            }
            if (!ifSecondStepCompleted() && count < 4) {
                RotateMachineGun("U+");
                cout << "U ";
                SecondStep(++count);
            } else if (!ifSecondStepCompleted() && count == 4) {
                Shuffle();
                FirstStep();
                SecondStep(0);
            }
        }
    }

    void ThirdStep(int count = 0) {
        if (ThirdStepCounter++ > 80) {
            cout << "Простите, с кубиком что-то не так :( (3й шаг)" << endl;
            exit(0);
        } else if (!ifThirdStepCompleted()) {
            if (FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter &&
                LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[2][1]->getUpPlane_piece() == LeftCenter) {
                RotateMachineGun("U-L-U+L+U+F+U-F-");
                cout << "U’ L’ U L U F U’ F’";
                ThirdStep(0);
            }
            if (FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[0][1]->getRightPlane_piece() == RightCenter &&
                UpPlane[1][2]->getUpPlane_piece() == FrontCenter) {
                RotateMachineGun("U-F-U+F+U+R+U-R-");
                cout << "U' F' U F U R U' R' ";
                ThirdStep(0);
            }
            if (BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                UpPlane[0][1]->getUpPlane_piece() == RightCenter) {
                RotateMachineGun("U-R-U+R+U+B+U-B-");
                cout << "U' R' U R U B U' B' ";
                ThirdStep(0);
            }
            if (BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[1][0]->getUpPlane_piece() == BackCenter) {
                RotateMachineGun("U-B-U+B+U+L+U-L-");
                cout << "U' B' U B U L U' L' ";
                ThirdStep(0);
            }
            if (RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                UpPlane[2][1]->getUpPlane_piece() == RightCenter) {
                RotateMachineGun("U+R+U-R-U-F-U+F+");
                cout << "U R U' R' U' F' U F ";
                ThirdStep(0);
            }
            if (FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[1][0]->getUpPlane_piece() == FrontCenter) {
                RotateMachineGun("U+F+U-F-U-L-U+L+");
                cout << "U R U' R' U' F' U F ";
                ThirdStep(0);
            }
            if (LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                UpPlane[0][1]->getUpPlane_piece() == LeftCenter) {
                RotateMachineGun("U+L+U-L-U-B-U+B+");
                cout << "U L U' L' U' B' U B ";
                ThirdStep(0);
            }
            if (BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                RightPlane[0][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                UpPlane[1][2]->getUpPlane_piece() == BackCenter) {
                RotateMachineGun("U+B+U-B-U-R-U+R+");
                cout << "U B U' B' U' R' U R ";
                ThirdStep(0);
            }
            if (!ifThirdStepCompleted() && count < 4) {
                RotateMachineGun("U+");
                cout << "U ";
                ThirdStep(++count);
            } else if (count == 4) {
                if (FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                    FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                    FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                    RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                    RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                    RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                    (FrontPlane[1][2]->getFrontPlane_piece() != FrontCenter ||
                     RightPlane[1][0]->getRightPlane_piece() != RightCenter)) {
                    RotateMachineGun("U+R+U-R-U-F-U+F+");
                    cout << "U R U' R' U' F' U F ";
                    ThirdStep(0);
                } else if (
                        FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                        FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                        FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                        LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                        LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                        LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                        (FrontPlane[1][0]->getFrontPlane_piece() != FrontCenter ||
                         LeftPlane[1][2]->getLeftPlane_piece() != LeftCenter)) {
                    RotateMachineGun("U+F+U-F-U-L-U+L+");
                    cout << "U F U' F' U' L' U L ";
                    ThirdStep(0);
                } else if (
                        BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                        BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                        BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                        LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                        LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                        LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                        (BackPlane[1][2]->getBackPlane_piece() != BackCenter ||
                         LeftPlane[1][0]->getLeftPlane_piece() != LeftCenter)) {
                    RotateMachineGun("U+L+U-L-U-B-U+B+");
                    cout << "U L U' L' U' B' U B ";
                    ThirdStep(0);
                } else if (
                        BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                        BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                        BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                        RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                        RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                        RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                        (BackPlane[1][0]->getBackPlane_piece() != BackCenter ||
                         RightPlane[1][2]->getRightPlane_piece() != RightCenter)) {
                    RotateMachineGun("U+B+U-B-U-R-U+R+");
                    cout << "U B U' B' U' R' U R ";
                    ThirdStep(0);
                }
            } else if (!ifThirdStepCompleted() && count == 4) {
                Shuffle();
                SecondStep();
                ThirdStep();
            }
        }
    }

    void FourthStep() {
        if (FourthStepCounter++ > 64) {
            cout << "Простите, с кубиком что-то не так :( (4й шаг)" << endl;
            exit(0);
        } else if (!ifFourthStepCompleted()) {
            if (FrontPlane[0][1]->getFrontPlane_piece() == UpCenter &&
                FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[0][1]->getRightPlane_piece() == UpCenter &&
                RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][0]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("F+U+R+U-R-F-");
                cout << "F U R U' R' F' ";
            } else if (FrontPlane[0][1]->getFrontPlane_piece() == UpCenter &&
                       FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == UpCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("L+U+F+U-F-L-");
                cout << "L U F U' F' L' ";
            } else if (BackPlane[0][1]->getBackPlane_piece() == UpCenter &&
                       BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       RightPlane[0][1]->getRightPlane_piece() == UpCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("R+U+B+U-B-R-");
                cout << "R U B U' B' R' ";
            } else if (BackPlane[0][1]->getBackPlane_piece() == UpCenter &&
                       BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == UpCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("B+U+L+U-L-B-");
                cout << "B U L U' L' B' ";
            } else if (FrontPlane[0][1]->getFrontPlane_piece() == UpCenter &&
                       FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       BackPlane[0][1]->getBackPlane_piece() == UpCenter) {
                RotateMachineGun("F+R+U+R-U-F-");
                cout << "F R U R' U' F' ";
            } else if (FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       RightPlane[0][1]->getRightPlane_piece() == UpCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == UpCenter) {
                RotateMachineGun("R+B+U+B-U-R-");
                cout << "R B U B' U' R' ";
            } else if (FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       FrontPlane[0][1]->getFrontPlane_piece() == UpCenter &&
                       RightPlane[0][1]->getRightPlane_piece() == UpCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == UpCenter &&
                       BackPlane[0][1]->getBackPlane_piece() == UpCenter) {
                RotateMachineGun("F+U+R+U-R-F-R+B+U+B-U-R-");
                cout << "F U R U' R' F' R B U B' U' R' ";
            } else if (!ifFourthStepCompleted()) {
                Shuffle();
                ThirdStep();
                FourthStep();
            }
        }
    }

    void FifthStep(int count = 0) {
        if (FifthStepCounter++ > 64) {
            cout << "Простите, с кубиком что-то не так :( (5й шаг)" << endl;
            exit(0);
        } else if (!ifFifthStepCompleted()) {
            if (FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                RightPlane[0][1]->getRightPlane_piece() == FrontCenter &&
                UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+L-U-U-L+U+L-U+L+");
                cout << "U L' U'U'L U L'U L ";
                FifthStep();
            } else if (FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == FrontCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+B-U-U-B+U+B-U+B+");
                cout << "U B' U' U' B U B' U B ";
                FifthStep();
            } else if (FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       BackPlane[0][1]->getBackPlane_piece() == FrontCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+R-U-U-R+U+R-U+R+U+B-U-U-B+U+B-U+B+U+R-U-U-R+U+R-U+R+");
                cout << "U R' U' U' R U R' U R U B' U' U' B U B' U B U R' U' U' R U R' U R ";
                FifthStep();
            } else if (BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[0][1]->getBackPlane_piece() == RightCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+F-U-U-F+U+F-U+F+");
                cout << "U F' U' U' F U F' U F ";
                FifthStep();
            } else if (BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == RightCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+B-U-U-B+U+B-U+B+U+L-U-U-L+U+L-U+L+U+B-U-U-B+U+B-U+B+");
                cout << "U B'U'U'B U B'U B U L'U'U'L U L'U L U B'U'U'B U B'U B ";
                FifthStep();
            } else if (BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == BackCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter) {
                RotateMachineGun("U+R-U-U-R+U+R-U+R+");
                cout << "U R' U' U' R U R' U R ";
                FifthStep();
            }
        }
    }

    void SixthStep(int count = 0) {
        if (SixthStepCounter++ > 64) {
            cout << "Простите, с кубиком что-то не так :( (6й шаг)" << endl;
            exit(0);
        } else if (!ifSixthStepCompleted()) {
            if (FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                (LeftPlane[0][2]->getLeftPlane_piece() == FrontCenter ||
                 LeftPlane[0][2]->getLeftPlane_piece() == RightCenter ||
                 LeftPlane[0][2]->getLeftPlane_piece() == UpCenter) &&
                (FrontPlane[0][0]->getFrontPlane_piece() == RightCenter ||
                 FrontPlane[0][0]->getFrontPlane_piece() == UpCenter ||
                 FrontPlane[0][0]->getFrontPlane_piece() == FrontCenter) &&
                (UpPlane[2][0]->getUpPlane_piece() == UpCenter ||
                 UpPlane[2][0]->getUpPlane_piece() == RightCenter ||
                 UpPlane[2][0]->getUpPlane_piece() == FrontCenter)) {

                RotateMachineGun("F'L'F R'F'L F R ");
                cout << "F' L' F R' F' L F R ";
                SixthStep();
            } else if (BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       RightPlane[0][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[1][2]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][0]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][1]->getRightPlane_piece() == RightCenter &&
                       RightPlane[2][2]->getRightPlane_piece() == RightCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       (RightPlane[0][2]->getRightPlane_piece() == FrontCenter ||
                        RightPlane[0][2]->getRightPlane_piece() == RightCenter ||
                        RightPlane[0][2]->getRightPlane_piece() == UpCenter) &&
                       (BackPlane[0][0]->getBackPlane_piece() == RightCenter ||
                        BackPlane[0][0]->getBackPlane_piece() == UpCenter ||
                        BackPlane[0][0]->getBackPlane_piece() == FrontCenter) &&
                       (UpPlane[0][2]->getUpPlane_piece() == UpCenter ||
                        UpPlane[0][2]->getUpPlane_piece() == RightCenter ||
                        UpPlane[0][2]->getUpPlane_piece() == FrontCenter)) {
                RotateMachineGun("R'F'L'F R F'L F ");
                cout << "R' F' L' F R F' L F ";
                SixthStep();
            } else if (BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       (BackPlane[0][2]->getBackPlane_piece() == FrontCenter ||
                        BackPlane[0][2]->getBackPlane_piece() == UpCenter ||
                        BackPlane[0][2]->getBackPlane_piece() == RightCenter) &&
                       (LeftPlane[0][0]->getLeftPlane_piece() == RightCenter ||
                        LeftPlane[0][0]->getLeftPlane_piece() == UpCenter ||
                        LeftPlane[0][0]->getLeftPlane_piece() == FrontCenter) &&
                       (UpPlane[0][0]->getUpPlane_piece() == UpCenter ||
                        UpPlane[0][0]->getUpPlane_piece() == RightCenter ||
                        UpPlane[0][0]->getUpPlane_piece() == FrontCenter)) {
                RotateMachineGun("F'L'B'L F L'B L ");
                cout << "F' L' B' L F L' B L ";
                SixthStep();
            } else if (FrontPlane[0][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[1][2]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][0]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][1]->getFrontPlane_piece() == FrontCenter &&
                       FrontPlane[2][2]->getFrontPlane_piece() == FrontCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       (LeftPlane[0][2]->getLeftPlane_piece() == RightCenter ||
                        LeftPlane[0][2]->getLeftPlane_piece() == BackCenter ||
                        LeftPlane[0][2]->getLeftPlane_piece() == UpCenter) &&
                       (FrontPlane[0][0]->getFrontPlane_piece() == BackCenter ||
                        FrontPlane[0][0]->getFrontPlane_piece() == RightCenter ||
                        FrontPlane[0][0]->getFrontPlane_piece() == UpCenter) &&
                       (UpPlane[2][0]->getUpPlane_piece() == UpCenter ||
                        UpPlane[2][0]->getUpPlane_piece() == RightCenter ||
                        UpPlane[2][0]->getUpPlane_piece() == BackCenter)) {
                RotateMachineGun("B'R'B L'B'R B L ");
                cout << "B' R' B L' B' R B L ";
                SixthStep();
            } else if (BackPlane[0][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[1][2]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][0]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][1]->getBackPlane_piece() == BackCenter &&
                       BackPlane[2][2]->getBackPlane_piece() == BackCenter &&
                       LeftPlane[0][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[1][2]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][0]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][1]->getLeftPlane_piece() == LeftCenter &&
                       LeftPlane[2][2]->getLeftPlane_piece() == LeftCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       (BackPlane[0][2]->getBackPlane_piece() == RightCenter ||
                        BackPlane[0][2]->getBackPlane_piece() == BackCenter ||
                        BackPlane[0][2]->getBackPlane_piece() == UpCenter) &&
                       (LeftPlane[0][0]->getLeftPlane_piece() == BackCenter ||
                        LeftPlane[0][0]->getLeftPlane_piece() == UpCenter ||
                        LeftPlane[0][0]->getLeftPlane_piece() == RightCenter) &&
                       (UpPlane[0][0]->getUpPlane_piece() == UpCenter ||
                        UpPlane[0][0]->getUpPlane_piece() == BackCenter ||
                        UpPlane[0][0]->getUpPlane_piece() == RightCenter)) {
                RotateMachineGun("L'B'R'B L B'R B ");
                cout << "L' B' R' B L R' R B ";
                SixthStep();
            } else if (!ifSixthStepCompleted() && count < 12) {
                SixthStep(++count);
            } else if (!ifSixthStepCompleted() && count == 12) {
                Shuffle();
                FifthStep();
                SixthStep();
            }

        }
    }

    void SeventhStep(int count = 0, bool flag = false) {
        if (SeventhStepCounter++ > 48) {
            cout << "Простите, с кубиком что-то не так :( (7й шаг)" << endl;
            exit(0);
        } else if (!ifCubeCompleted()) {
            if (UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                RightPlane[0][0]->getRightPlane_piece() == UpCenter) {
                RotateMachineGun("F-R+F+R-F-R+F+R-U+");
                cout << "F' R F R' F' R F R' ";
                if (flag) {
                    RotateMachineGun("U+");
                    cout << "U ";
                } else flag = true;

            } else if (UpPlane[0][1]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][0]->getUpPlane_piece() == UpCenter &&
                       UpPlane[1][2]->getUpPlane_piece() == UpCenter &&
                       UpPlane[2][1]->getUpPlane_piece() == UpCenter &&
                       FrontPlane[0][2]->getFrontPlane_piece() == UpCenter) {
                RotateMachineGun("R+F-R-F+R+F-R-F+U+");
                cout << "R F' R' F R F' R' F";
                if (flag) {
                    RotateMachineGun("U+");
                    cout << "U ";
                } else flag = true;
            }

            if (!ifCubeCompleted()) {
                RotateMachineGun("U+");
                cout << "U ";
                SeventhStep(++count, flag);
            }
        }
    }

    void AlgorithmBild()
    {
        cout << "Последовательность поворотов:" << endl;
        FirstStep();
        cout << endl;
        SecondStep();
        cout << endl;
        ThirdStep();
        cout << endl;
        FourthStep();
        cout << endl;
        FifthStep();
        cout << endl;
        SixthStep();
        cout << endl;
        SeventhStep();
        cout << endl;
        cout << "------------------------------------Кубик Рубик Собран------------------------------------" << endl;
    }

    // 1й цвет - низ
    // 2й цвет - фронт
    // 3й цвет - право
    // 4й цвет - зад
    // 5й цвет - лево
    // 6й цвет - верх

    // Узоры
    void Chess()
    {
        RotateRightPlane('+');
        RotateRightPlane('+');

        RotateLeftPlane('+');
        RotateLeftPlane('+');

        RotateFrontPlane('+');
        RotateFrontPlane('+');

        RotateBackPlane('+');
        RotateBackPlane('+');

        RotateUpPlane('+');
        RotateUpPlane('+');

        RotateDownPlane('+');
        RotateDownPlane('+');
    }
    void CubeInCube()
    {
        RotateUpPlane('+');
        RotateUpPlane('+');

        RotateFrontPlane('+');
        RotateFrontPlane('+');

        RotateRightPlane('+');
        RotateRightPlane('+');

        RotateUpPlane('-');

        RotateLeftPlane('+');
        RotateLeftPlane('+');

        RotateDownPlane('+');

        RotateBackPlane('+');

        RotateRightPlane('-');

        RotateBackPlane('+');

        RotateRightPlane('-');

        RotateBackPlane('+');

        RotateRightPlane('-');

        RotateDownPlane('-');

        RotateLeftPlane('+');
        RotateLeftPlane('+');

        RotateUpPlane('-');
    }
    void Crosses()
    {
        RotateLeftPlane('+');
        RotateLeftPlane('+');

        RotateRightPlane('-');

        RotateBackPlane('+');
        RotateBackPlane('+');

        RotateFrontPlane('+');
        RotateFrontPlane('+');

        RotateDownPlane('+');
        RotateDownPlane('+');

        RotateBackPlane('+');
        RotateBackPlane('+');

        RotateFrontPlane('+');
        RotateFrontPlane('+');

        RotateLeftPlane('+');
        RotateLeftPlane('+');

        RotateRightPlane('+');
        RotateRightPlane('+');

        RotateUpPlane('+');
        RotateUpPlane('+');

        RotateRightPlane('-');
    }

    // Вывод
    // вывод всех цветов кусочка Кубика Рубика
    static void PrintCubePiece(vector<vector<CubePiece*>> plane, int i, int j)
    {
        cout << plane[i][j]->getUpPlane_piece() << "    ";
        cout << plane[i][j]->getDownPlane_piece() << "    ";
        cout << plane[i][j]->getLeftPlane_piece() << "    ";
        cout << plane[i][j]->getRightPlane_piece() << "    ";
        cout << plane[i][j]->getFrontPlane_piece() << "    ";
        cout << plane[i][j]->getBackPlane_piece() << endl;
    }
    // вывод всех цветов кусочков одной грани Кубика Рубика
    [[maybe_unused]] static void PrintAllCubePiece(const vector<vector<CubePiece*>>& plane)
    {
        cout << "       Up      Down     Left    Right    Front    Back" << endl;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << "[" << i << ", " << j << "] ";
                PrintCubePiece(plane, i, j);
            }
        }
    }
    // Вывод развёртки в консоль
    void PrintCubeRubik()
    {
        for (int i = 0; i < 3; i++)
        {
            cout << "\t\t\t\t\t\t | ";
            for (int j = 0; j < 3; j++)
            {
                PrintWord(UpPlane[i][j]->getUpPlane_piece());
            }
            cout << " | " << endl;
        }

        cout <<  endl << " | ";
        for (int j = 0; j < 3; j++)
            PrintWord(LeftPlane[0][j]->getLeftPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(FrontPlane[0][j]->getFrontPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(RightPlane[0][j]->getRightPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(BackPlane[0][j]->getBackPlane_piece());
        cout << " | " << endl << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(LeftPlane[1][j]->getLeftPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(FrontPlane[1][j]->getFrontPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(RightPlane[1][j]->getRightPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(BackPlane[1][j]->getBackPlane_piece());
        cout << " | " << endl << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(LeftPlane[2][j]->getLeftPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(FrontPlane[2][j]->getFrontPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(RightPlane[2][j]->getRightPlane_piece());
        cout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWord(BackPlane[2][j]->getBackPlane_piece());
        cout << " | " << endl << endl;

        for (int i = 0; i < 3; i++)
        {
            cout << "\t\t\t\t\t\t | ";
            for (int j = 0; j < 3; j++)
                PrintWord(DownPlane[i][j]->getDownPlane_piece());
            cout << " | " << endl;
        }
        cout << endl;
        cout << endl;
    }
    // Вывод развёртки в файл
    void PrintInFileCubeRubik()
    {
        for (int i = 0; i < 3; i++)
        {
            fout << "\t\t\t\t\t\t | ";
            for (int j = 0; j < 3; j++)
            {
                PrintWordInFile(UpPlane[i][j]->getUpPlane_piece());
            }
            fout << " | " << endl;
        }

        fout <<  endl << " | ";
        for (int j = 0; j < 3; j++)
            PrintWordInFile(LeftPlane[0][j]->getLeftPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(FrontPlane[0][j]->getFrontPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(RightPlane[0][j]->getRightPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(BackPlane[0][j]->getBackPlane_piece());
        fout << " | " << endl << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(LeftPlane[1][j]->getLeftPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(FrontPlane[1][j]->getFrontPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(RightPlane[1][j]->getRightPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(BackPlane[1][j]->getBackPlane_piece());
        fout << " | " << endl << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(LeftPlane[2][j]->getLeftPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(FrontPlane[2][j]->getFrontPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(RightPlane[2][j]->getRightPlane_piece());
        fout << " | ";

        for (int j = 0; j < 3; j++)
            PrintWordInFile(BackPlane[2][j]->getBackPlane_piece());
        fout << " | " << endl << endl;

        for (int i = 0; i < 3; i++)
        {
            fout << "\t\t\t\t\t\t | ";
            for (int j = 0; j < 3; j++)
                PrintWordInFile(DownPlane[i][j]->getDownPlane_piece());
            fout << " | " << endl;
        }
        fout << endl;
        fout << endl;
    }
};

void Menu()
{
    cout << "------------------------Меню------------------------" << endl;
    cout << "Для использования команды введите её номер из списка" << endl;
    cout << "0) Вызвать меню" << endl;
    cout << "1) Считать Кубик Рубик из файла" << endl;
    cout << "2) Обновить Кубик Рубик" << endl;
    cout << "3) Вывести развёртку Кубика Рубика в консоль" << endl;
    cout << "4) Вывести развёртку Кубика Рубика в файл" << endl;
    cout << "5) Поворот граней Кубика Рубика" << endl;
    cout << "6) Перемешать Кубик Рубик" << endl;
    cout << "7) Собрать Кубик Рубик" << endl;
    cout << "8) Использовать прикол" << endl;
    cout << "9) Выйти из программы" << endl;
    cout << "----------------------------------------------------" << endl;
}

void RotateMenu()
{
    cout << "----------------------Повороты----------------------" << endl;
    cout << "Для использования команды введите её номер из списка" << endl;
    cout << "0) Вернуться в меню" << endl;
    cout << "1) Поворот верхней грани по часовой стрелки" << endl;
    cout << "2) Поворот верхней грани против часовой стрелки" << endl;
    cout << "3) Поворот нижней грани по часовой стрелки" << endl;
    cout << "4) Поворот нижней грани против часовой стрелки" << endl;
    cout << "5) Поворот левой грани по часовой стрелки" << endl;
    cout << "6) Поворот левой грани против часовой стрелки" << endl;
    cout << "7) Поворот правой грани по часовой стрелки" << endl;
    cout << "8) Поворот правой грани против часовой стрелки" << endl;
    cout << "9) Поворот передней грани по часовой стрелки" << endl;
    cout << "10) Поворот передней грани против часовой стрелки" << endl;
    cout << "11) Поворот задней грани по часовой стрелки" << endl;
    cout << "12) Поворот задней грани против часовой стрелки" << endl;
    cout << "13) Вывести развёртку Кубика Рубика в консоль" << endl;
    cout << "14) Вывести развёртку Кубика Рубика в файл" << endl;
    cout << "----------------------------------------------------" << endl;
}

void PatternMenu()
{
    cout << "------------------------Узоры-----------------------" << endl;
    cout << "Для использования команды введите её номер из списка" << endl;
    cout << "0) Вернуться в меню" << endl;
    cout << "1) Шахматы" << endl;
    cout << "2) Кубик в кубе" << endl;
    cout << "3) Кресты" << endl;
    cout << "4) Обновить Кубик Рубик" << endl;
    cout << "5) Вывести развёртку Кубика Рубика в консоль" << endl;
    cout << "6) Вывести развёртку Кубика Рубика в файл" << endl;
    cout << "----------------------------------------------------" << endl;
}

int main()
{
    cout << "Введите любой символ, создать кубик и начать работать" << endl;
    string empty_str;
    cin >> empty_str;
    CubeRubik cube;
    Menu();
    while (true)
    {
        int com;
        cin >> com;
        if (com == 0)
        {
            Menu();
        }
        else if (com == 1)
        {
            cout << "Введите название файла:" << endl;
            string file;
            cin >> file;
            cube.ReadFromFileCubeRubik(file);
        }
        else if (com == 2)
        {
            cube.UpdateCubeRubik();
            cout << "------------------------------------Кубик Рубик был пересоздан------------------------------------" << endl;
        }
        else if (com == 3)
        {
            cube.PrintCubeRubik();
        }
        else if (com == 4)
        {
            cout << "Развёртка выведена в файл: output.txt" << endl;
            cube.PrintInFileCubeRubik();
        }
        else if (com == 5)
        {
            RotateMenu();
            bool flag = true;
            while (flag)
            {
                int com1;
                cin >> com1;
                if (com1 == 0)
                {
                    flag = false;
                    Menu();
                }
                else if (com1 == 1)
                {
                    cube.RotateUpPlane('+');
                }
                else if (com1 == 2)
                {
                    cube.RotateUpPlane('-');
                }
                else if (com1 == 3)
                {
                    cube.RotateDownPlane('+');
                }
                else if (com1 == 4)
                {
                    cube.RotateDownPlane('-');
                }
                else if (com1 == 5)
                {
                    cube.RotateLeftPlane('+');
                }
                else if (com1 == 6)
                {
                    cube.RotateLeftPlane('-');
                }
                else if (com1 == 7)
                {
                    cube.RotateRightPlane('+');
                }
                else if (com1 == 8)
                {
                    cube.RotateRightPlane('-');
                }
                else if (com1 == 9)
                {
                    cube.RotateFrontPlane('+');
                }
                else if (com1 == 10)
                {
                    cube.RotateFrontPlane('-');
                }
                else if (com1 == 11)
                {
                    cube.RotateBackPlane('+');
                }
                else if (com1 == 12)
                {
                    cube.RotateBackPlane('-');
                }
                else if (com1 == 13)
                {
                    cube.PrintCubeRubik();
                }
                else if (com1 == 14)
                {
                    cout << "Развёртка выведена в файл: output.txt" << endl;
                    cube.PrintInFileCubeRubik();
                }
                else
                {
                    cout << "Не ну ты что вообще дурак, нормально команду введи!" << endl;
                }
            }
        }
        else if (com == 6)
        {
            cube.Shuffle();
            cout << "------------------------------------Кубик Рубик был перемешан------------------------------------" << endl;
        }
        else if (com == 7)
        {
            cube.AlgorithmBild();
        }
        else if (com == 8)
        {
            PatternMenu();
            bool flag = true;
            while (flag)
            {
                int com2;
                cin >> com2;
                if (com2 == 0)
                {
                    flag = false;
                    Menu();
                }
                if (com2 == 1)
                {
                    cube.Chess();
                }
                else if (com2 == 2)
                {
                    cube.CubeInCube();
                }
                else if (com2 == 3)
                {
                    cube.Crosses();
                }
                else if (com2 == 4)
                {
                    cube.UpdateCubeRubik();
                    cout << "------------------------------------Кубик Рубик был пересоздан------------------------------------" << endl;
                }
                else if (com2 == 5)
                {
                    cube.PrintCubeRubik();
                }
                else if (com2 == 6)
                {
                    cout << "Развёртка выведена в файл: output.txt" << endl;
                    cube.PrintInFileCubeRubik();
                }
                else
                {
                    cout << "Не ну ты что вообще дурак, нормально команду введи!" << endl;
                }
            }
        }
        else if (com == 9)
        {
            return 0;
        }
        else
        {
            cout << "Не ну ты что вообще дурак, нормально команду введи!" << endl;
        }
    }
}