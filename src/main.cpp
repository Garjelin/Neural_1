#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct DataPoint {
    double x;
    double y;
};

// Класс Neuron - представляет один нейрон
class Neuron {
private:
    std::vector<float> weights;  // Массив весов
    float threshold;              // Порог активации

public:
    // Конструктор: создаёт нейрон с заданным количеством входов
    Neuron(int numInputs, float thresh = 0.0f) : threshold(thresh) {
        weights.resize(numInputs);
        // Инициализация случайными весами в диапазоне [-1, 1]
        for (int i = 0; i < numInputs; ++i) {
            weights[i] = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        }
    }

    // Метод propagate: вычисляет взвешенную сумму и применяет функцию активации
    int propagate(const std::vector<float>& inputs) {
        float s = 0.0f;
        // Вычисляем взвешенную сумму s = Σ(xi * wi)
        for (size_t i = 0; i < inputs.size() && i < weights.size(); ++i) {
            s += inputs[i] * weights[i];
        }
        // Функция единичного скачка (ступенчатая функция)
        return (s >= threshold) ? 1 : 0;
    }

    // Геттеры для отладки
    const std::vector<float>& getWeights() const { return weights; }
    float getThreshold() const { return threshold; }
};

// Класс Layer - представляет слой нейронов
class Layer {
private:
    std::vector<Neuron> neurons;  // Массив нейронов в слое

public:
    // Конструктор: создаёт слой из numNeurons нейронов, каждый с numInputs входами
    Layer(int numNeurons, int numInputs, float threshold = 0.0f) {
        neurons.reserve(numNeurons);
        for (int i = 0; i < numNeurons; ++i) {
            neurons.emplace_back(numInputs, threshold);
        }
    }

    // Метод propagate: пропускает входы через все нейроны слоя
    std::vector<int> propagate(const std::vector<float>& inputs) {
        std::vector<int> outputs;
        outputs.reserve(neurons.size());
        for (auto& neuron : neurons) {
            outputs.push_back(neuron.propagate(inputs));
        }
        return outputs;
    }

    // Геттер для доступа к нейронам
    const std::vector<Neuron>& getNeurons() const { return neurons; }
    size_t size() const { return neurons.size(); }
};

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    // Чтение данных из файла
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

    // ============================================
    // Демонстрация работы классов Neuron и Layer
    // ============================================
    std::cout << "\n============================================" << std::endl;
    std::cout << "Демонстрация работы нейронной сети" << std::endl;
    std::cout << "============================================\n" << std::endl;

    // Создаём слой из 5 нейронов с 1 входом каждый (для нашего x)
    // Порог установлен в 0.0
    Layer layer(5, 1, 0.0f);

    // Выводим веса каждого нейрона
    std::cout << "Инициализированные веса нейронов:" << std::endl;
    for (size_t i = 0; i < layer.size(); ++i) {
        const auto& neuron = layer.getNeurons()[i];
        std::cout << "  Нейрон " << i + 1 << ": вес = " << neuron.getWeights()[0]
                  << ", порог = " << neuron.getThreshold() << std::endl;
    }

    // Берём первое значение x из файла
    if (!data.empty()) {
        float firstX = static_cast<float>(data[0].x);
        std::vector<float> inputs = {firstX};

        std::cout << "\nВходное значение x = " << firstX << std::endl;
        std::cout << "Выходы нейронов слоя:" << std::endl;

        std::vector<int> outputs = layer.propagate(inputs);
        for (size_t i = 0; i < outputs.size(); ++i) {
            const auto& neuron = layer.getNeurons()[i];
            float weightedSum = firstX * neuron.getWeights()[0];
            std::cout << "  Нейрон " << i + 1 << ": s = " << weightedSum
                      << " -> y = " << outputs[i] << std::endl;
        }
    }

    return 0;
}
