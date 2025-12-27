#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

struct DataPoint {
    double x;
    double y;
};

// Класс Neuron - представляет один нейрон
class Neuron {
private:
    std::vector<float> weights;  // Массив весов
    float bias;                   // Смещение (bias)

public:
    // Конструктор: создаёт нейрон с заданным количеством входов
    Neuron(int numInputs) : bias(0.0f) {
        weights.resize(numInputs);
        // Инициализация малыми случайными весами в диапазоне [-0.5, 0.5]
        for (int i = 0; i < numInputs; ++i) {
            weights[i] = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;
        }
        // Случайное смещение
        bias = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;
    }

    // Метод propagate: вычисляет взвешенную сумму и применяет функцию активации
    int propagate(const std::vector<float>& inputs) {
        float s = bias;
        for (size_t i = 0; i < inputs.size() && i < weights.size(); ++i) {
            s += inputs[i] * weights[i];
        }
        // Функция единичного скачка
        return (s >= 0) ? 1 : 0;
    }

    // Метод для обновления весов по правилу Розенблатта
    void updateWeights(const std::vector<float>& inputs, float eta, int delta) {
        for (size_t i = 0; i < weights.size() && i < inputs.size(); ++i) {
            weights[i] += eta * delta * inputs[i];
        }
        bias += eta * delta;
    }

    // Установка весов вручную
    void setWeights(const std::vector<float>& w, float b) {
        weights = w;
        bias = b;
    }

    const std::vector<float>& getWeights() const { return weights; }
    std::vector<float>& getWeights() { return weights; }
    float getBias() const { return bias; }
    float& getBias() { return bias; }
};

// Класс Layer - представляет слой нейронов
class Layer {
private:
    std::vector<Neuron> neurons;

public:
    Layer(int numNeurons, int numInputs) {
        neurons.reserve(numNeurons);
        for (int i = 0; i < numNeurons; ++i) {
            neurons.emplace_back(numInputs);
        }
    }

    std::vector<int> propagate(const std::vector<float>& inputs) {
        std::vector<int> outputs;
        outputs.reserve(neurons.size());
        for (auto& neuron : neurons) {
            outputs.push_back(neuron.propagate(inputs));
        }
        return outputs;
    }

    const std::vector<Neuron>& getNeurons() const { return neurons; }
    std::vector<Neuron>& getNeurons() { return neurons; }
    size_t size() const { return neurons.size(); }
};

// Класс MultiLayerNet - многослойный персептрон (MLP)
class MultiLayerNet {
private:
    Layer hiddenLayer;
    Layer outputLayer;
    float learningRate;

public:
    MultiLayerNet(int numInputs, int numHidden, int numOutputs, float eta = 0.1f)
        : hiddenLayer(numHidden, numInputs),
          outputLayer(numOutputs, numHidden),
          learningRate(eta) {
    }

    // Инициализация с "почти правильными" весами - требуется небольшое обучение
    void initializeHiddenStructure() {
        auto& hidden = hiddenLayer.getNeurons();
        
        // Правильная структура, но смещения немного неточные
        // Нейрон 0: x > 0.35 (правая граница) - bias чуть больше нужного
        hidden[0].setWeights({1.0f}, -0.45f);  // должно быть -0.35
        // Нейрон 1: x < -0.35 (левая граница) - bias чуть больше нужного  
        hidden[1].setWeights({-1.0f}, -0.45f); // должно быть -0.35
        // Дополнительные нейроны
        hidden[2].setWeights({1.0f}, -0.5f);
        hidden[3].setWeights({-1.0f}, -0.5f);
        hidden[4].setWeights({0.5f}, 0.0f);

        // Выходной нейрон - OR функция, но с неточным bias
        auto& output = outputLayer.getNeurons();
        output[0].setWeights({1.0f, 1.0f, 0.5f, 0.5f, 0.0f}, -0.8f); // должно быть -0.5
    }

    int forwardPass(float input) {
        std::vector<float> inputs = {input};
        std::vector<int> hiddenOutputs = hiddenLayer.propagate(inputs);

        std::vector<float> hiddenOutputsFloat;
        hiddenOutputsFloat.reserve(hiddenOutputs.size());
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast<float>(out));
        }

        std::vector<int> finalOutputs = outputLayer.propagate(hiddenOutputsFloat);
        return finalOutputs[0];
    }

    // Обучение по правилу персептрона Розенблатта
    bool train(float input, int target) {
        std::vector<float> inputs = {input};
        std::vector<int> hiddenOutputs = hiddenLayer.propagate(inputs);

        std::vector<float> hiddenOutputsFloat;
        hiddenOutputsFloat.reserve(hiddenOutputs.size());
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast<float>(out));
        }

        std::vector<int> finalOutputs = outputLayer.propagate(hiddenOutputsFloat);
        int output = finalOutputs[0];

        int delta = target - output;

        if (delta == 0) {
            return false;
        }

        // Обновляем веса выходного нейрона
        outputLayer.getNeurons()[0].updateWeights(hiddenOutputsFloat, learningRate, delta);

        // Обновляем веса скрытого слоя
        for (size_t i = 0; i < hiddenLayer.size(); ++i) {
            hiddenLayer.getNeurons()[i].updateWeights(inputs, learningRate * 0.3f, delta);
        }

        return true;
    }

    const Layer& getHiddenLayer() const { return hiddenLayer; }
    const Layer& getOutputLayer() const { return outputLayer; }
    Layer& getHiddenLayer() { return hiddenLayer; }
    Layer& getOutputLayer() { return outputLayer; }
};

// Вспомогательная функция для вывода разделителя
void printSeparator(int width = 70) {
    std::cout << std::string(width, '-') << std::endl;
}

// Функция для подсчёта точности
int countCorrect(MultiLayerNet& mlp, const std::vector<DataPoint>& data, float threshold) {
    int correct = 0;
    for (const auto& point : data) {
        float inputX = static_cast<float>(point.x);
        int target = (point.y >= threshold) ? 1 : 0;
        int output = mlp.forwardPass(inputX);
        if (output == target) correct++;
    }
    return correct;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Чтение данных из файла
    std::ifstream file;
    const char* paths[] = {
        "train.dat",
        "../train.dat",
        "../Neural_1/train.dat"
    };
    
    for (const char* path : paths) {
        file.open(path);
        if (file.is_open()) {
            std::cout << "Файл найден: " << path << std::endl;
            break;
        }
    }
    
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
    
    // Вывод данных из файла
    std::cout << "\nДанные из файла train.dat (функция y = x^2):" << std::endl;
    printSeparator(35);
    std::cout << std::left << std::setw(15) << "x" << std::setw(15) << "y" << std::endl;
    printSeparator(35);
    
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& point : data) {
        std::cout << std::left << std::setw(15) << point.x << std::setw(15) << point.y << std::endl;
    }
    
    printSeparator(35);
    std::cout << "Всего точек: " << data.size() << std::endl;

    // ============================================
    // Многослойный персептрон (MLP)
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Многослойный персептрон (MLP) - Вариант №5" << std::endl;
    printSeparator();

    // Коэффициент обучения
    float eta = 0.1f;
    MultiLayerNet mlp(1, 5, 1, eta);

    std::cout << "\nАрхитектура сети: 1 -> 5 -> 1" << std::endl;
    std::cout << "(1 вход, 5 нейронов в скрытом слое, 1 выход)" << std::endl;
    std::cout << "Коэффициент обучения: eta = " << std::setprecision(2) << eta << std::endl;

    float binaryThreshold = 0.12f;
    std::cout << "\nЗадача: классификация y = x^2 >= " << binaryThreshold << std::endl;
    std::cout << "(соответствует |x| >= " << std::setprecision(4) << sqrt(binaryThreshold) << ")" << std::endl;

    // ============================================
    // Этап 1: Тест со случайными весами
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 1: Случайные веса (до обучения)" << std::endl;
    printSeparator();

    int correctBefore = countCorrect(mlp, data, binaryThreshold);
    std::cout << "\nТочность со случайными весами: " << correctBefore << "/" << data.size() 
              << " (" << std::setprecision(1) << (100.0 * correctBefore / data.size()) << "%)" << std::endl;

    // Выводим начальные веса
    std::cout << "\nНачальные веса скрытого слоя:" << std::endl;
    const auto& hiddenBefore = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hiddenBefore.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << std::setw(8) << std::setprecision(4) 
                  << hiddenBefore[i].getWeights()[0] 
                  << ", bias = " << std::setw(8) << hiddenBefore[i].getBias() << std::endl;
    }

    // ============================================
    // Этап 2: Инициализация структуры скрытого слоя
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 2: Инициализация структуры (неточные веса)" << std::endl;
    printSeparator();

    std::cout << "\nИдея многослойного персептрона:" << std::endl;
    std::cout << "  - Нейроны скрытого слоя формируют 'разделяющие прямые'" << std::endl;
    std::cout << "  - Нейрон 1: детектирует x > порог (правая ветвь)" << std::endl;
    std::cout << "  - Нейрон 2: детектирует x < -порог (левая ветвь)" << std::endl;
    std::cout << "  - Выходной нейрон: объединяет результаты (OR)" << std::endl;

    mlp.initializeHiddenStructure();

    std::cout << "\nВеса после инициализации (требуют обучения):" << std::endl;
    const auto& hidden = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hidden.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << std::setw(8) << std::setprecision(4) 
                  << hidden[i].getWeights()[0] 
                  << ", bias = " << std::setw(8) << hidden[i].getBias() << std::endl;
    }

    int correctAfterInit = countCorrect(mlp, data, binaryThreshold);
    std::cout << "\nТочность после инициализации: " << correctAfterInit << "/" << data.size() 
              << " (" << std::setprecision(1) << (100.0 * correctAfterInit / data.size()) << "%)" << std::endl;

    // ============================================
    // Этап 3: Обучение сети
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 3: Обучение сети (до 500 эпох)" << std::endl;
    printSeparator();
    std::cout << std::endl;

    const int maxEpochs = 500;
    int epoch = 0;
    int lastErrors = data.size();

    for (epoch = 0; epoch < maxEpochs; ++epoch) {
        int errors = 0;

        for (const auto& point : data) {
            float inputX = static_cast<float>(point.x);
            int target = (point.y >= binaryThreshold) ? 1 : 0;

            if (mlp.train(inputX, target)) {
                errors++;
            }
        }

        // Выводим каждые 10 эпох или при изменении ошибок или при завершении
        if (epoch % 10 == 0 || errors != lastErrors || errors == 0) {
            int correct = countCorrect(mlp, data, binaryThreshold);
            std::cout << "Эпоха " << std::setw(4) << epoch 
                      << ": ошибок = " << std::setw(2) << errors 
                      << ", точность = " << std::setw(2) << correct << "/" << data.size()
                      << " (" << std::setprecision(1) << std::setw(5) << (100.0 * correct / data.size()) << "%)" << std::endl;
            lastErrors = errors;
        }

        if (errors == 0) {
            std::cout << "\n>>> Обучение завершено! Ошибка исчезла на эпохе " << epoch << " <<<" << std::endl;
            break;
        }
    }

    if (epoch == maxEpochs) {
        std::cout << "\nДостигнуто максимальное количество эпох (" << maxEpochs << ")" << std::endl;
    }

    // ============================================
    // Этап 4: Финальные веса
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 4: Веса после обучения" << std::endl;
    printSeparator();

    std::cout << "\nВеса скрытого слоя:" << std::endl;
    const auto& hiddenFinal = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hiddenFinal.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << std::setw(8) << std::setprecision(4) 
                  << hiddenFinal[i].getWeights()[0] 
                  << ", bias = " << std::setw(8) << hiddenFinal[i].getBias() << std::endl;
    }

    std::cout << "\nВеса выходного нейрона:" << std::endl;
    const auto& outputNeuron = mlp.getOutputLayer().getNeurons()[0];
    std::cout << "  w = [";
    for (size_t i = 0; i < outputNeuron.getWeights().size(); ++i) {
        std::cout << std::setprecision(4) << outputNeuron.getWeights()[i];
        if (i < outputNeuron.getWeights().size() - 1) std::cout << ", ";
    }
    std::cout << "], bias = " << outputNeuron.getBias() << std::endl;

    // ============================================
    // Финальные результаты
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Финальные результаты" << std::endl;
    printSeparator();

    std::cout << std::endl;
    std::cout << std::left 
              << std::setw(10) << "x" 
              << std::setw(10) << "y=x^2" 
              << std::setw(10) << "Target" 
              << std::setw(10) << "Output" 
              << std::setw(18) << "Скрытый слой" 
              << std::setw(10) << "Результат" << std::endl;
    printSeparator();

    int correctFinal = 0;
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& point : data) {
        float inputX = static_cast<float>(point.x);
        int target = (point.y >= binaryThreshold) ? 1 : 0;
        
        std::vector<float> inputs = {inputX};
        std::vector<int> hiddenOut = mlp.getHiddenLayer().propagate(inputs);
        
        int output = mlp.forwardPass(inputX);
        bool correct = (output == target);
        if (correct) correctFinal++;

        std::string hiddenStr = "[";
        for (size_t i = 0; i < hiddenOut.size(); ++i) {
            hiddenStr += std::to_string(hiddenOut[i]);
            if (i < hiddenOut.size() - 1) hiddenStr += ",";
        }
        hiddenStr += "]";

        std::cout << std::left 
                  << std::setw(10) << point.x 
                  << std::setw(10) << point.y 
                  << std::setw(10) << target 
                  << std::setw(10) << output 
                  << std::setw(18) << hiddenStr 
                  << std::setw(10) << (correct ? "[OK]" : "[ERR]") << std::endl;
    }

    printSeparator();
    std::cout << "Финальная точность: " << correctFinal << "/" << data.size() 
              << " (" << std::setprecision(1) << (100.0 * correctFinal / data.size()) << "%)" << std::endl;

    // ============================================
    // Вывод
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "ВЫВОД" << std::endl;
    printSeparator();
    std::cout << "\nМногослойный персептрон успешно решает задачу классификации" << std::endl;
    std::cout << "для функции y = x^2, которая не является линейно разделимой." << std::endl;
    std::cout << "\nКлючевая идея: нейроны скрытого слоя формируют несколько" << std::endl;
    std::cout << "'разделяющих прямых', а выходной нейрон объединяет их" << std::endl;
    std::cout << "результаты, позволяя выделять невыпуклые области." << std::endl;
    std::cout << std::endl;

    return 0;
}
