#include <iostream>
#include <fstream>
#include <vector>

struct DataPoint {
    double x;
    double y;
};

int main() {
    std::ifstream file("../train.dat");
    
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл train.dat" << std::endl;
        return 1;
    }
    
    std::vector<DataPoint> data;
    double x, y;
    
    while (file >> x >> y) {
        data.push_back({x, y});
    }
    
    file.close();
    
    std::cout << "Данные из файла train.dat (функция y = x^2):" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "   x\t\t   y" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    
    for (const auto& point : data) {
        std::cout << "   " << point.x << "\t\t   " << point.y << std::endl;
    }
    
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Всего точек: " << data.size() << std::endl;
    
    return 0;
}
