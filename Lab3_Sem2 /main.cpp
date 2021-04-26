#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <cmath>

using namespace std;
using namespace pugi;
using std::stringstream;

class Transport_station
{
private:
    int num;
    string type_of_vehicle;
    string name_stopping;
    string the_official_name;
    vector<string> location;
    vector<string> routes;
    pair<double, double> coordinates;
public:
    // Конструктор
    Transport_station(int num, string type_of_vehicle, string name_stopping, string the_official_name,
                      vector<string> location, vector<string> routes, pair<double, double> coordinates)
    {
        this->num = num;
        this->type_of_vehicle = type_of_vehicle;
        this->name_stopping = name_stopping;
        this->the_official_name = the_official_name;
        this->location = location;
        this->routes = routes;
        this->coordinates = coordinates;
    }
    // Гетеры
    int get_num() const
    {
        return num;
    }
    string get_type_of_vehicle() const
    {
        return type_of_vehicle;
    }
    string get_name_stopping() const
    {
        return name_stopping;
    }
    string get_the_official_name() const
    {
        return the_official_name;
    }
    vector<string> get_location() const
    {
        return location;
    }
    vector<string> get_routes () const
    {
        return routes;
    }
    double get_coordinates_x() const
    {
        return coordinates.first;
    }
    double get_coordinates_y() const
    {
        return coordinates.second;
    }
    string route_i(int i) const
    {
        return routes[i];
    }
    int route_size() const
    {
        return routes.size();
    }
};

class Routes
{
public:
    string route;
    vector<Transport_station> Trolleybus;
    vector<Transport_station> Bus;
    vector<Transport_station> Tram;
};

// Функция подсчёта растояний между двумя координатами
double distCoord(const Transport_station &a, const Transport_station &b)
{
    double lat1, lat2, long1, long2, delta_lat, gelta_long, ans;
    lat1 = a.get_coordinates_x() * M_PI / 180;
    long1 = a.get_coordinates_y() * M_PI / 180;
    lat2 = b.get_coordinates_x() * M_PI / 180;
    long2 = b.get_coordinates_y() * M_PI / 180;
    delta_lat = lat2 - lat1;
    gelta_long = long2 - long1;
    ans = pow(sin(delta_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(gelta_long / 2), 2);
    ans = 2 * asin(sqrt(ans)) * 6371;
    return ans;
}

// Функция для считывания данных без запятой
void separator(const string &str, string &str1, string &str2)
{
    string flag = ","; // указываем раздилитель
    auto start = 0; // начало
    auto finish = str.find(flag); // и конец строки
    while (finish != -1)
    {
        str1 = str.substr(start, finish - start); // записываем первую чать строки
        start = finish + flag.length(); // переводим указатели старт и финиш на след. пункт
        finish = str.find(flag, start);
    }
    str2 = str.substr(start, finish); // записываем вторую часть строки
}

// Функция для обрезания строки
string correction_location(string &str)
{ // errors хранит всё то, что можно обрезать
    vector<string> errors{"ул.", " ул.", " УЛ.", " ш.", " Ш.", " шоссе", " ШОССЕ", " пер.", " ПЕР.", " переулок", " ПЕРЕУЛОК", " улица", "улица", " УЛИЦА", " бул", " БУЛ", " бульвар", " БУЛЬВАР", " пр", " ПР", " проспект", " ПРОСПЕКТ"};
    for (auto i: errors)
    {
        if (str.find(i) != -1)
        {
            int first = str.find(i);
            int second = first + i.size();
            str.erase(first, second);
        }
    }
    if (str[str.size() - 1] == ' ')
        str.erase(str.end() - 1);
    return str;
}

// Парсинг
void parsing_file(vector<Transport_station> &a, map<string, Routes> &Map_routes, set<string> &Name_routes, map<string, int> &Locations)
{
    xml_document doc; // создаем обьект
    doc.load_file("data.xml"); // используем файл
    xml_node data = doc.child("dataset"); // находи первого ребёнка
    // идём по детям dataset считывая нужную информацию
    for (xml_node i = data.child("transport_station"); i; i = i.next_sibling("transport_station"))
    {
        // координаты
        string str1, str2;
        separator(i.child_value("coordinates"), str1, str2); // используем сепоратор
        float coord1, coord2;
        coord1 = stof(str1); // преобразуем строку в float
        coord2 = stof(str2);
        pair<double, double> coordinates;
        coordinates.first = coord1;
        coordinates.second = coord2;
        // маршрут
        str1 = "";
        str2 = "";
        string str_routes = i.child_value("routes"), segment; // segment - пустая строка
        vector<string> routes;
        stringstream tempR(str_routes);  // создаём строковый поток
        if (count(str_routes.begin(), str_routes.end(), ',')) // заполняем вектор routes
            while (getline(tempR, segment, ',')) // корректируя данные для дальнейшей работы
                routes.push_back(segment);
        else
            while (getline(tempR, segment, '.'))
                routes.push_back(segment);
        // локация
        str1 = "";
        str2 = "";
        string str_location = i.child_value("location");
        vector<string> locations;
        stringstream tempL(str_location); // создаём строковый поток
        if (count(str_location.begin(), str_location.end(), ',') && !str_location.empty())
        { // корректируем и разделяем локации для дальнейшей работы
            while (getline(tempL, segment, ','))
            {
                if (segment[0] == ' ')
                    segment.erase(segment.begin());
                locations.push_back(correction_location(segment));
                Locations[correction_location(segment)] += 1;
            }
        }
        else if (!str_location.empty())
        {
            locations.push_back(correction_location(str_location));
            Locations[correction_location(str_location)] += 1;
        }
        // помещаем данные в экземпляр класса
        int num = stoi(i.child_value("number"));
        string type_of_vehicle = i.child_value("type_of_vehicle");
        string name_stopping = i.child_value("name_stopping");
        string the_official_name = i.child_value("the_official_name");
        Transport_station Tran_st(num, type_of_vehicle, name_stopping, the_official_name, locations, routes, coordinates);
        a.emplace_back(Tran_st);
        if (strcmp(i.child_value("type_of_vehicle"), "Трамвай") == false)
        {
            for (auto j: routes)
            {
                Map_routes[j].Tram.emplace_back(Transport_station(num, type_of_vehicle, name_stopping, the_official_name, locations, routes, coordinates));
                Map_routes[j].route = j;
                Name_routes.insert(j);
            }
        }
        if (strcmp(i.child_value("type_of_vehicle"), "Автобус") == false)
        {
            for (auto j: routes)
            {
                Map_routes[j].Bus.emplace_back(Transport_station(num, type_of_vehicle, name_stopping, the_official_name, locations, routes, coordinates));
                Map_routes[j].route = j;
                Name_routes.insert(j);
            }
        }
        if (strcmp(i.child_value("type_of_vehicle"), "Троллейбус") == false)
        {
            for (auto j: routes)
            {
                Map_routes[j].Trolleybus.emplace_back(Transport_station(num, type_of_vehicle, name_stopping, the_official_name, locations, routes, coordinates));
                Map_routes[j].route = j;
                Name_routes.insert(j);
            }
        }
    }
}

int main()
{
    vector<Transport_station> TransportStation;
    map<string, Routes> routes;
    set<string> name_routes;
    map<string, int> locations;
    parsing_file(TransportStation, routes, name_routes, locations);
    // создаём хеш-таблицу, которая будет хранить маршруты и кол-во остановок
    map<string, int> tram_routes, bus_routes, trolleybus_routes;
    for (auto const i: TransportStation)
    { // заполняем кол-во станция для каждого вида транспорта
        if (i.get_type_of_vehicle() == "Трамвай")
            for (int j = 0; j < i.route_size(); j++)
                tram_routes[i.route_i(j)] += 1;
        else if (i.get_type_of_vehicle() == "Автобус")
            for (int j = 0; j < i.route_size(); j++)
                bus_routes[i.route_i(j)] += 1;
        else if (i.get_type_of_vehicle() == "Троллейбус")
            for (int j = 0; j < i.route_size(); j++)
                trolleybus_routes[i.route_i(j)] += 1;
    }

    // Пункт 1. Маршрут с наибольшим количеством остановок по отдельными видам транспорта.
    string max_tram_routes, max_bus_routes, max_trolleybus_routes; // номера маршрутов
    int k_tram_routes = 0, k_bus_routes = 0, k_trolleybus_routes = 0; // кол-во остановок
    // подсчёт наибольшего кол-ва остановок и номер этого маршрута
    for (auto const i: tram_routes)
        if (i.second > k_tram_routes) // ищем саксимальное кол-во остановок для каждого вида транспорта
        {
            k_tram_routes = i.second;
            max_tram_routes = i.first;
        }
    for (auto const i: bus_routes)
        if (i.second > k_bus_routes)
        {
            k_bus_routes = i.second;
            max_bus_routes = i.first;
        }
    for (auto const i: trolleybus_routes)
        if (i.second > k_trolleybus_routes)
        {
            k_trolleybus_routes = i.second;
            max_trolleybus_routes = i.first;
        }
    cout << endl;
    cout << "1. Маршрут с наибольшим количеством остановок по отдельными видам транспорта." << endl;
    cout << "Маршрут №" << max_tram_routes << " имеет максимальное кол-во остановок среди трамваев, равное: " << k_tram_routes << " остановок." << endl;
    cout << "Маршрут №" << max_bus_routes << " имеет максимальное кол-во остановок среди автобусов, равное: " << k_bus_routes << " остановок." << endl;
    cout << "Маршрут №" << max_trolleybus_routes << " имеет максимальное кол-во остановок среди троллейбусов, равное: " << k_trolleybus_routes << " остановок." << endl;
    cout << endl;

    // Пункт 2. Наиболее длинный маршрут (основывая на координатах) по отдельным видам транспорта.
    map<string, double> size_tram_routes, size_bus_routes, size_trolleybus_routes;
    for (auto const i: name_routes)
    {
        if (routes[i].Tram.size() > 1)
            for (int j = 0; j < routes[i].Tram.size() - 1; j++)
                size_tram_routes[routes[i].route] += distCoord(routes[i].Tram[j], routes[i].Tram[j + 1]);
        if (routes[i].Bus.size() > 1)
            for (int j = 0; j < routes[i].Bus.size() - 1; j++)
                size_bus_routes[routes[i].route] += distCoord(routes[i].Bus[j], routes[i].Bus[j + 1]);
        if (routes[i].Trolleybus.size() > 1)
            for (int j = 0; j < routes[i].Trolleybus.size() - 1; j++)
                size_trolleybus_routes[routes[i].route] += distCoord(routes[i].Trolleybus[j], routes[i].Trolleybus[j + 1]);
    }
    string max_tram_routes1, max_bus_routes1, max_trolleybus_routes1;
    double k_tram_routes1 = 0, k_bus_routes1 = 0, k_trolleybus_routes1 = 0;
    for (auto const i : size_tram_routes)
        if (i.second > k_tram_routes1)
        {
            k_tram_routes1 = i.second;
            max_tram_routes1 = i.first;
        }
    for (auto const i : size_bus_routes)
        if (i.second > k_bus_routes1)
        {
            k_bus_routes1 = i.second;
            max_bus_routes1 = i.first;
        }
    for (auto const i : size_trolleybus_routes)
        if (i.second > k_trolleybus_routes1)
        {
            k_trolleybus_routes1 = i.second;
            max_trolleybus_routes1 = i.first;
        }
    cout << endl;
    cout << "2. Наиболее длинный маршрут (основывая на координатах) по отдельным видам транспорта." << endl;
    cout << "Максимальный маршрут трамвая: " << max_tram_routes1 << endl;
    cout << "Максимальный маршрут автобуса: " << max_bus_routes1 << endl;
    cout << "Максимальный маршрут троллейбуса: " << max_trolleybus_routes1 << endl;
    cout << endl;

    // Пункт 3. Улица с наибольшим числом остановок.
    int max_street = 0; // ищем максимальное кол-во остановок и улицу, на котрой они рассположенны
    string str_max_street;
    for (auto const i: locations)
        if (i.second)
        {
            max_street = i.second;
            str_max_street = i.first;
        }
    cout << endl;
    cout << "3. Улица с наибольшим числом остановок.    " << endl;
    cout << "Улица " << "'" <<str_max_street << "'" << " имеет наибольшее кол-во остановок, равное: " << max_street << " остановок." << endl;
    cout << endl;
    return 0;
}