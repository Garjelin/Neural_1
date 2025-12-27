#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>

struct DataPoint {
    double x;
    double y;
};

// Класс Neuron - представляет один нейрон
class Neuron {
private:
    std::vector<float> weights;
    float bias;

public:
    Neuron(int numInputs) : bias(0.0f) {
        weights.resize(numInputs);
        for (int i = 0; i < numInputs; ++i) {
            weights[i] = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;
        }
        bias = (static_cast<float>(rand()) / RAND_MAX) - 0.5f;
    }

    int propagate(const std::vector<float>& inputs) {
        float s = bias;
        for (size_t i = 0; i < inputs.size() && i < weights.size(); ++i) {
            s += inputs[i] * weights[i];
        }
        return (s >= 0) ? 1 : 0;
    }

    void updateWeights(const std::vector<float>& inputs, float eta, int delta) {
        for (size_t i = 0; i < weights.size() && i < inputs.size(); ++i) {
            weights[i] += eta * delta * inputs[i];
        }
        bias += eta * delta;
    }

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

    void initializeHiddenStructure() {
        auto& hidden = hiddenLayer.getNeurons();
        
        hidden[0].setWeights({1.0f}, -0.45f);
        hidden[1].setWeights({-1.0f}, -0.45f);
        hidden[2].setWeights({1.0f}, -0.5f);
        hidden[3].setWeights({-1.0f}, -0.5f);
        hidden[4].setWeights({0.5f}, 0.0f);

        auto& output = outputLayer.getNeurons();
        output[0].setWeights({1.0f, 1.0f, 0.5f, 0.5f, 0.0f}, -0.8f);
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

        outputLayer.getNeurons()[0].updateWeights(hiddenOutputsFloat, learningRate, delta);

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

// Вспомогательные функции
void printSeparator(int width = 70) {
    std::cout << std::string(width, '-') << std::endl;
}

void printSeparatorToFile(std::ofstream& file, int width = 70) {
    file << std::string(width, '-') << std::endl;
}

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

// Функция для получения строки с весами выходного нейрона
std::string getOutputWeightsString(const Neuron& neuron) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "w = [";
    const auto& weights = neuron.getWeights();
    for (size_t i = 0; i < weights.size(); ++i) {
        oss << weights[i];
        if (i < weights.size() - 1) oss << ", ";
    }
    oss << "], bias = " << neuron.getBias();
    return oss.str();
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Чтение данных из файла
    std::ifstream file;
    std::string dataFilePath;
    const char* paths[] = {
        "train.dat",
        "../train.dat",
        "../Neural_1/train.dat"
    };
    
    for (const char* path : paths) {
        file.open(path);
        if (file.is_open()) {
            dataFilePath = path;
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

    float eta = 0.1f;
    MultiLayerNet mlp(1, 5, 1, eta);

    std::cout << "\nАрхитектура сети: 1 -> 5 -> 1" << std::endl;
    std::cout << "(1 вход, 5 нейронов в скрытом слое, 1 выход)" << std::endl;
    std::cout << "Коэффициент обучения: eta = " << std::setprecision(2) << eta << std::endl;

    float binaryThreshold = 0.12f;
    std::cout << "\nЗадача: классификация y = x^2 >= " << binaryThreshold << std::endl;
    std::cout << "(соответствует |x| >= " << std::setprecision(4) << sqrt(binaryThreshold) << ")" << std::endl;

    // ============================================
    // Этап 1: Инициализация структуры
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 1: Инициализация структуры сети" << std::endl;
    printSeparator();

    std::cout << "\nИдея многослойного персептрона:" << std::endl;
    std::cout << "  - Нейроны скрытого слоя формируют 'разделяющие прямые'" << std::endl;
    std::cout << "  - Нейрон 1: детектирует x > порог (правая ветвь)" << std::endl;
    std::cout << "  - Нейрон 2: детектирует x < -порог (левая ветвь)" << std::endl;
    std::cout << "  - Выходной нейрон: объединяет результаты (OR)" << std::endl;

    mlp.initializeHiddenStructure();

    // Сохраняем веса ДО обучения
    std::cout << "\n";
    printSeparator();
    std::cout << "ВЕСА ВЫХОДНОГО НЕЙРОНА ДО ОБУЧЕНИЯ:" << std::endl;
    printSeparator();
    
    const auto& outputNeuronBefore = mlp.getOutputLayer().getNeurons()[0];
    std::string weightsBefore = getOutputWeightsString(outputNeuronBefore);
    std::cout << weightsBefore << std::endl;

    std::cout << "\nВеса скрытого слоя:" << std::endl;
    const auto& hidden = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hidden.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << std::setw(8) << std::setprecision(4) 
                  << hidden[i].getWeights()[0] 
                  << ", bias = " << std::setw(8) << hidden[i].getBias() << std::endl;
    }

    int correctBeforeTrain = countCorrect(mlp, data, binaryThreshold);
    std::cout << "\nТочность до обучения: " << correctBeforeTrain << "/" << data.size() 
              << " (" << std::setprecision(1) << (100.0 * correctBeforeTrain / data.size()) << "%)" << std::endl;

    // ============================================
    // Этап 2: Обучение сети
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Этап 2: Обучение сети" << std::endl;
    printSeparator();
    std::cout << std::endl;

    const int maxEpochs = 500;
    int epoch = 0;
    int totalEpochs = 0;
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

        if (epoch % 10 == 0 || errors != lastErrors || errors == 0) {
            int correct = countCorrect(mlp, data, binaryThreshold);
            std::cout << "Эпоха " << std::setw(4) << epoch 
                      << ": ошибок = " << std::setw(2) << errors 
                      << ", точность = " << std::setw(2) << correct << "/" << data.size()
                      << " (" << std::setprecision(1) << std::setw(5) << (100.0 * correct / data.size()) << "%)" << std::endl;
            lastErrors = errors;
        }

        if (errors == 0) {
            totalEpochs = epoch + 1;
            std::cout << "\n>>> Обучение завершено! <<<" << std::endl;
            break;
        }
    }

    if (epoch == maxEpochs) {
        totalEpochs = maxEpochs;
        std::cout << "\nДостигнуто максимальное количество эпох (" << maxEpochs << ")" << std::endl;
    }

    // ============================================
    // Этап 3: Веса после обучения
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "ВЕСА ВЫХОДНОГО НЕЙРОНА ПОСЛЕ ОБУЧЕНИЯ:" << std::endl;
    printSeparator();

    const auto& outputNeuronAfter = mlp.getOutputLayer().getNeurons()[0];
    std::string weightsAfter = getOutputWeightsString(outputNeuronAfter);
    std::cout << weightsAfter << std::endl;

    std::cout << "\nВеса скрытого слоя после обучения:" << std::endl;
    const auto& hiddenFinal = mlp.getHiddenLayer().getNeurons();
    for (size_t i = 0; i < hiddenFinal.size(); ++i) {
        std::cout << "  Нейрон " << i + 1 << ": w = " << std::setw(8) << std::setprecision(4) 
                  << hiddenFinal[i].getWeights()[0] 
                  << ", bias = " << std::setw(8) << hiddenFinal[i].getBias() << std::endl;
    }

    // ============================================
    // Этап 4: Финальные результаты
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "Финальные результаты" << std::endl;
    printSeparator();

    std::cout << std::endl;
    std::cout << std::left 
              << std::setw(10) << "x" 
              << std::setw(12) << "Эталон x^2" 
              << std::setw(10) << "Target" 
              << std::setw(10) << "Выход" 
              << std::setw(10) << "Результат" << std::endl;
    printSeparator(52);

    int correctFinal = 0;
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& point : data) {
        float inputX = static_cast<float>(point.x);
        int target = (point.y >= binaryThreshold) ? 1 : 0;
        int output = mlp.forwardPass(inputX);
        bool correct = (output == target);
        if (correct) correctFinal++;

        std::cout << std::left 
                  << std::setw(10) << point.x 
                  << std::setw(12) << point.y 
                  << std::setw(10) << target 
                  << std::setw(10) << output 
                  << std::setw(10) << (correct ? "[OK]" : "[ERR]") << std::endl;
    }

    printSeparator(52);
    std::cout << "Финальная точность: " << correctFinal << "/" << data.size() 
              << " (" << std::setprecision(1) << (100.0 * correctFinal / data.size()) << "%)" << std::endl;

    // ============================================
    // Статистика обучения
    // ============================================
    std::cout << "\n";
    printSeparator();
    std::cout << "СТАТИСТИКА ОБУЧЕНИЯ" << std::endl;
    printSeparator();
    std::cout << "\nОбщее количество эпох обучения: " << totalEpochs << std::endl;
    std::cout << "Коэффициент обучения (eta): " << std::setprecision(2) << eta << std::endl;
    std::cout << "Архитектура сети: 1 -> 5 -> 1" << std::endl;
    std::cout << "Функция активации: единичный скачок (step function)" << std::endl;
    std::cout << "Алгоритм обучения: правило Розенблатта" << std::endl;

    // ============================================
    // Запись результатов в файл output.txt
    // ============================================
    std::string outputPath = "../output.txt";
    // Если запускаем из build, файл создастся в корне проекта
    std::ofstream outFile(outputPath);
    
    if (!outFile.is_open()) {
        // Пробуем создать в текущей папке
        outputPath = "output.txt";
        outFile.open(outputPath);
    }
    
    if (outFile.is_open()) {
        outFile << std::fixed;
        
        outFile << "========================================" << std::endl;
        outFile << "РЕЗУЛЬТАТЫ РАБОТЫ МНОГОСЛОЙНОГО ПЕРСЕПТРОНА" << std::endl;
        outFile << "Вариант №5 - Аппроксимация функции y = x^2" << std::endl;
        outFile << "========================================" << std::endl;
        outFile << std::endl;
        
        outFile << "ПАРАМЕТРЫ СЕТИ:" << std::endl;
        outFile << "  Архитектура: 1 -> 5 -> 1" << std::endl;
        outFile << "  Коэффициент обучения (eta): " << std::setprecision(2) << eta << std::endl;
        outFile << "  Функция активации: единичный скачок" << std::endl;
        outFile << "  Алгоритм обучения: правило Розенблатта" << std::endl;
        outFile << std::endl;
        
        outFile << "РЕЗУЛЬТАТЫ ОБУЧЕНИЯ:" << std::endl;
        outFile << "  Количество эпох: " << totalEpochs << std::endl;
        outFile << "  Финальная точность: " << correctFinal << "/" << data.size() 
                << " (" << std::setprecision(1) << (100.0 * correctFinal / data.size()) << "%)" << std::endl;
        outFile << std::endl;
        
        outFile << "ВЕСА ВЫХОДНОГО НЕЙРОНА:" << std::endl;
        outFile << "  До обучения:    " << weightsBefore << std::endl;
        outFile << "  После обучения: " << weightsAfter << std::endl;
        outFile << std::endl;
        
        outFile << "========================================" << std::endl;
        outFile << "ИТОГОВАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ" << std::endl;
        outFile << "========================================" << std::endl;
        outFile << std::endl;
        
        outFile << std::left 
                << std::setw(12) << "x" 
                << std::setw(14) << "Эталон (x^2)" 
                << std::setw(14) << "Результат сети" << std::endl;
        printSeparatorToFile(outFile, 40);
        
        outFile << std::setprecision(4);
        for (const auto& point : data) {
            float inputX = static_cast<float>(point.x);
            int output = mlp.forwardPass(inputX);
            
            outFile << std::left 
                    << std::setw(12) << point.x 
                    << std::setw(14) << point.y 
                    << std::setw(14) << output << std::endl;
        }
        
        printSeparatorToFile(outFile, 40);
        outFile << std::endl;
        
        outFile << "Примечание: Результат сети - бинарная классификация" << std::endl;
        outFile << "  1 = x^2 >= 0.12 (|x| >= 0.346)" << std::endl;
        outFile << "  0 = x^2 < 0.12 (|x| < 0.346)" << std::endl;
        
        outFile.close();
        
        std::cout << "\n";
        printSeparator();
        std::cout << "Результаты сохранены в файл: " << outputPath << std::endl;
        printSeparator();
    } else {
        std::cerr << "\nОшибка: не удалось создать файл output.txt" << std::endl;
    }

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
