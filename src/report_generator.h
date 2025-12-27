#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <string>
#include <vector>

// Структура для хранения данных отчёта
struct ReportData {
    // Параметры сети
    int numInputs = 1;
    int numHidden = 5;
    int numOutputs = 1;
    float learningRate = 0.1f;
    float binaryThreshold = 0.12f;
    
    // Результаты обучения
    int totalEpochs = 0;
    int correctCount = 0;
    int totalCount = 0;
    
    // Веса до и после обучения
    std::string weightsBefore;
    std::string weightsAfter;
    
    // Веса скрытого слоя (после обучения)
    struct HiddenNeuronWeights {
        float weight;
        float bias;
    };
    std::vector<HiddenNeuronWeights> hiddenWeights;
    
    // Результаты классификации
    struct ResultRow {
        double x;
        double y;
        int target;
        int output;
    };
    std::vector<ResultRow> results;
};

// Генерирует HTML-отчёт и сохраняет в файл
// Возвращает true при успехе
bool generateHtmlReport(const ReportData& data, const std::string& outputPath);

#endif // REPORT_GENERATOR_H

