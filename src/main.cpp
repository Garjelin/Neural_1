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

// Класс MultiLayerNet - многослойная нейронная сеть (MLP)
class MultiLayerNet {
private:
    Layer hiddenLayer;  // Скрытый слой
    Layer outputLayer;  // Выходной слой

public:
    // Конструктор: создаёт сеть с заданной архитектурой
    // numInputs - количество входов (1 для нашего x)
    // numHidden - количество нейронов в скрытом слое
    // numOutputs - количество выходных нейронов (1 для нашей задачи)
    MultiLayerNet(int numInputs, int numHidden, int numOutputs, float threshold = 0.0f)
        : hiddenLayer(numHidden, numInputs, threshold),
          outputLayer(numOutputs, numHidden, threshold) {
    }

    // Метод forwardPass: прямое распространение сигнала через всю сеть
    // Вход -> Скрытый слой -> Выходной слой -> Результат
    int forwardPass(float input) {
        // Шаг 1: Входной слой просто передаёт x (без преобразований)
        std::vector<float> inputs = {input};

        // Шаг 2: Пропускаем через скрытый слой
        std::vector<int> hiddenOutputs = hiddenLayer.propagate(inputs);

        // Шаг 3: Преобразуем выходы скрытого слоя в float для входа в выходной слой
        std::vector<float> hiddenOutputsFloat;
        hiddenOutputsFloat.reserve(hiddenOutputs.size());
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast<float>(out));
        }

        // Шаг 4: Пропускаем через выходной слой
        std::vector<int> finalOutputs = outputLayer.propagate(hiddenOutputsFloat);

        // Возвращаем единственный выход сети
        return finalOutputs[0];
    }

    // Метод для детального вывода прохода через сеть
    void forwardPassVerbose(float input) {
        std::vector<float> inputs = {input};

        std::cout << "  Вход: x = " << input << std::endl;

        // Скрытый слой
        std::vector<int> hiddenOutputs = hiddenLayer.propagate(inputs);
        std::cout << "  Скрытый слой (" << hiddenLayer.size() << " нейронов): [";
        for (size_t i = 0; i < hiddenOutputs.size(); ++i) {
            std::cout << hiddenOutputs[i];
            if (i < hiddenOutputs.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        // Выходной слой
        std::vector<float> hiddenOutputsFloat;
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast<float>(out));
        }
        std::vector<int> finalOutputs = outputLayer.propagate(hiddenOutputsFloat);
        std::cout << "  Выход сети: y = " << finalOutputs[0] << std::endl;
    }

    // Геттеры для доступа к слоям
    const Layer& getHiddenLayer() const { return hiddenLayer; }
    const Layer& getOutputLayer() const { return outputLayer; }
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
    // Демонстрация работы MultiLayerNet (MLP)
    // ============================================
    std::cout << "\n============================================" << std::endl;
    std::cout << "Многослойная нейронная сеть (MLP)" << std::endl;
    std::cout << "============================================\n" << std::endl;

    // Создаём сеть: 1 вход -> 5 нейронов в скрытом слое -> 1 выход
    MultiLayerNet mlp(1, 5, 1, 0.0f);

    std::cout << "Архитектура сети: 1 -> 5 -> 1" << std::endl;
    std::cout << "(1 вход, 5 нейронов в скрытом слое, 1 выход)\n" << std::endl;

    // Выводим веса скрытого слоя
    std::cout << "Веса скрытого слоя:" << std::endl;
    const auto& hiddenNeurons = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hiddenNeurons.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << hiddenNeurons[i].getWeights()[0] << std::endl;
    }

    // Выводим веса выходного слоя
    std::cout << "\nВеса выходного нейрона:" << std::endl;
    const auto& outputNeurons = mlp.getOutputLayer().getNeurons();
    std::cout << "  Выходной нейрон: w = [";
    const auto& outputWeights = outputNeurons[0].getWeights();
    for (size_t i = 0; i < outputWeights.size(); ++i) {
        std::cout << outputWeights[i];
        if (i < outputWeights.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    // Тестируем сеть на всех данных из файла
    std::cout << "\n============================================" << std::endl;
    std::cout << "Прямой проход (forwardPass) для всех точек:" << std::endl;
    std::cout << "============================================\n" << std::endl;

    for (const auto& point : data) {
        float inputX = static_cast<float>(point.x);
        std::cout << "Точка x = " << inputX << " (ожидаемое y = " << point.y << "):" << std::endl;
        mlp.forwardPassVerbose(inputX);
        std::cout << std::endl;
    }

    return 0;
}
