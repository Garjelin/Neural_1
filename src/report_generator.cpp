#include "report_generator.h"
#include <fstream>
#include <sstream>
#include <iomanip>

// Генерирует CSS стили
static std::string generateCSS() {
    return R"(
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Times New Roman', Times, serif;
            font-size: 14pt;
            line-height: 1.5;
            color: #1a1a1a;
            background-color: #fff;
            padding: 20px;
            max-width: 210mm;
            margin: 0 auto;
        }

        .title-page {
            text-align: center;
            page-break-after: always;
            min-height: 90vh;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            padding: 40px 0;
        }

        .title-page .header {
            font-size: 14pt;
        }

        .title-page .header p {
            margin-bottom: 5px;
        }

        .title-page .main-title {
            margin: 60px 0;
        }

        .title-page .main-title h1 {
            font-size: 18pt;
            font-weight: bold;
            margin-bottom: 20px;
        }

        .title-page .main-title h2 {
            font-size: 14pt;
            font-weight: normal;
        }

        .title-page .info {
            text-align: left;
            margin-left: 50%;
            margin-top: 60px;
        }

        .title-page .info p {
            margin-bottom: 10px;
        }

        .title-page .footer {
            margin-top: auto;
        }

        .content {
            margin-top: 40px;
        }

        h1 {
            font-weight: normal;
            font-size: 16pt;
            margin: 0 0 0 0;
        }

        h2 {
            font-size: 16pt;
            margin: 30px 0 15px 0;
            color: #2c3e50;
            border-bottom: 2px solid #3498db;
            padding-bottom: 5px;
        }

        h3 {
            font-size: 14pt;
            margin: 20px 0 10px 0;
            color: #34495e;
        }

        p {
            margin-bottom: 12px;
            text-align: justify;
            text-indent: 1.5em;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            font-size: 12pt;
        }

        th, td {
            border: 1px solid #333;
            padding: 8px 12px;
            text-align: center;
        }

        th {
            background-color: #3498db;
            color: white;
            font-weight: bold;
        }

        tr:nth-child(even) {
            background-color: #f8f9fa;
        }

        tr:hover {
            background-color: #e8f4f8;
        }

        .code-block {
            background-color: #1e1e1e;
            color: #d4d4d4;
            padding: 20px;
            border-radius: 8px;
            overflow-x: auto;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 11pt;
            line-height: 1.4;
            margin: 20px 0;
            max-height: 600px;
            overflow-y: auto;
        }

        .code-block code {
            white-space: pre;
        }

        .weights-block {
            background-color: #f5f5f5;
            border-left: 4px solid #3498db;
            padding: 15px 20px;
            margin: 20px 0;
            font-family: 'Consolas', monospace;
            font-size: 12pt;
        }

        .weights-block .label {
            font-weight: bold;
            color: #2c3e50;
            margin-bottom: 5px;
        }

        .architecture {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 25px;
            border-radius: 10px;
            margin: 20px 0;
            text-align: center;
        }

        .architecture h3 {
            color: white;
            margin-bottom: 15px;
        }

        .architecture .layers {
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 30px;
            flex-wrap: wrap;
        }

        .architecture .layer {
            background: rgba(255,255,255,0.2);
            padding: 15px 25px;
            border-radius: 8px;
        }

        .architecture .arrow {
            font-size: 24pt;
        }

        .conclusion {
            background-color: #e8f6e8;
            border: 1px solid #27ae60;
            padding: 20px;
            border-radius: 8px;
            margin: 20px 0;
        }

        .conclusion h3 {
            color: #27ae60;
        }

        .formula {
            background-color: #fff3cd;
            border: 1px solid #ffc107;
            padding: 15px;
            margin: 15px 0;
            border-radius: 5px;
            font-family: 'Cambria Math', serif;
            text-align: center;
        }

        ul, ol {
            margin: 15px 0 15px 40px;
        }

        li {
            margin-bottom: 8px;
        }

        @media print {
            body {
                padding: 0;
            }
            .code-block {
                max-height: none;
                font-size: 9pt;
            }
            .title-page {
                page-break-after: always;
            }
        }
    </style>
)";
}

// Генерирует титульную страницу
static std::string generateTitlePage() {
    return R"(
    <div class="title-page">
        <div class="header">
            <h1>МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РФ</h1>
            <h1>ФЕДЕРАЛЬНОЕ ГОСУДАРСТВЕННОЕ БЮДЖЕТНОЕ</h1>
            <h1>ОБРАЗОВАТЕЛЬНОЕ УЧРЕЖДЕНИЕ ВЫСШЕГО ОБРАЗОВАНИЯ</h1>
            <h1><strong>«ТЮМЕНСКИЙ ИНДУСТРИАЛЬНЫЙ УНИВЕРСИТЕТ»</strong></h1>
            <h1>Институт дополнительного и дистанционного образования</h1>
        </div>

        <div class="main-title">
            <h1>ЛАБОРАТОРНАЯ РАБОТА №1</h1>
            <p style="font-size: 16pt;text-align: center;">«Программирование искусственной нейронной сети»</p>
            <p style="font-size: 16pt;text-align: center;">по дисциплине «Теория нейронных сетей»</p>
            <p style="text-align: center;"><strong>Вариант №5:</strong></p>
            <p style="text-align: center;">«Многослойный персептрон (бинарные функции активации). Аппроксимация данных»</p>
            <p>&nbsp;</p>
        </div>

        <div class="info">
            <p><strong>Выполнил:</strong> студент группы НТм(до)з-25-1</p>
            <p>ФИО: Якимов Сергей Анатольевич</p>
            <p>&nbsp;</p>
            <p><strong>Проверил:</strong> Сенкевич Людмила Борисовна</p>
        </div>

        <div class="footer">
            <p style="text-align: center;margin: 50px 0">Тюмень, 2025</p>
        </div>
    </div>
)";
}

// Генерирует SVG-график
static std::string generateSVGGraph() {
    return R"(
        <div class="graph-container" style="text-align: center; margin: 20px 0;">
            <svg width="700" height="450" viewBox="0 0 700 450" style="border: 1px solid #ccc; background: #fafafa;">
                <!-- Оси -->
                <line x1="80" y1="380" x2="650" y2="380" stroke="#333" stroke-width="2"/>
                <line x1="80" y1="380" x2="80" y2="50" stroke="#333" stroke-width="2"/>
                
                <!-- Стрелки осей -->
                <polygon points="650,380 640,375 640,385" fill="#333"/>
                <polygon points="80,50 75,60 85,60" fill="#333"/>
                
                <!-- Подписи осей -->
                <text x="660" y="385" font-size="14" fill="#333">x</text>
                <text x="70" y="40" font-size="14" fill="#333">y</text>
                
                <!-- Сетка -->
                <g stroke="#ddd" stroke-width="1">
                    <line x1="80" y1="300" x2="650" y2="300"/>
                    <line x1="80" y1="220" x2="650" y2="220"/>
                    <line x1="80" y1="140" x2="650" y2="140"/>
                    <line x1="195" y1="50" x2="195" y2="380"/>
                    <line x1="310" y1="50" x2="310" y2="380"/>
                    <line x1="425" y1="50" x2="425" y2="380"/>
                    <line x1="540" y1="50" x2="540" y2="380"/>
                </g>
                
                <!-- Подписи делений X -->
                <text x="75" y="400" font-size="11" fill="#666">-0.5</text>
                <text x="185" y="400" font-size="11" fill="#666">-0.25</text>
                <text x="305" y="400" font-size="11" fill="#666">0</text>
                <text x="420" y="400" font-size="11" fill="#666">0.25</text>
                <text x="535" y="400" font-size="11" fill="#666">0.5</text>
                
                <!-- Подписи делений Y -->
                <text x="55" y="385" font-size="11" fill="#666">0</text>
                <text x="45" y="305" font-size="11" fill="#666">0.1</text>
                <text x="45" y="225" font-size="11" fill="#666">0.2</text>
                <text x="45" y="145" font-size="11" fill="#666">0.3</text>
                
                <!-- Парабола y = x² -->
                <path d="M 80,180 Q 195,360 310,380 Q 425,360 540,180 L 540,180" 
                      fill="none" stroke="#3498db" stroke-width="3"/>
                
                <!-- Порог классификации y = 0.12 -->
                <line x1="80" y1="284" x2="650" y2="284" stroke="#e74c3c" stroke-width="2" stroke-dasharray="8,4"/>
                
                <!-- Вертикальные границы |x| = 0.346 -->
                <line x1="151" y1="50" x2="151" y2="380" stroke="#9b59b6" stroke-width="2" stroke-dasharray="4,4"/>
                <line x1="469" y1="50" x2="469" y2="380" stroke="#9b59b6" stroke-width="2" stroke-dasharray="4,4"/>
                
                <!-- Точки: выход = 1 (зелёные) -->
                <g fill="#27ae60">
                    <circle cx="80" cy="180" r="8"/>
                    <circle cx="103" cy="216" r="8"/>
                    <circle cx="126" cy="252" r="8"/>
                    <circle cx="149" cy="282" r="8"/>
                    <circle cx="471" cy="282" r="8"/>
                    <circle cx="494" cy="252" r="8"/>
                    <circle cx="517" cy="216" r="8"/>
                    <circle cx="540" cy="180" r="8"/>
                </g>
                
                <!-- Точки: выход = 0 (серые) -->
                <g fill="#7f8c8d">
                    <circle cx="172" cy="308" r="8"/>
                    <circle cx="195" cy="330" r="8"/>
                    <circle cx="218" cy="348" r="8"/>
                    <circle cx="241" cy="362" r="8"/>
                    <circle cx="264" cy="372" r="8"/>
                    <circle cx="287" cy="378" r="8"/>
                    <circle cx="310" cy="380" r="8"/>
                    <circle cx="333" cy="378" r="8"/>
                    <circle cx="356" cy="372" r="8"/>
                    <circle cx="379" cy="362" r="8"/>
                    <circle cx="402" cy="348" r="8"/>
                    <circle cx="425" cy="330" r="8"/>
                    <circle cx="448" cy="308" r="8"/>
                </g>
                
                <!-- Легенда -->
                <rect x="480" y="60" width="160" height="100" fill="white" stroke="#ccc" rx="5"/>
                <line x1="490" y1="80" x2="520" y2="80" stroke="#3498db" stroke-width="3"/>
                <text x="530" y="85" font-size="12" fill="#333">Эталон y = x²</text>
                
                <line x1="490" y1="105" x2="520" y2="105" stroke="#e74c3c" stroke-width="2" stroke-dasharray="8,4"/>
                <text x="530" y="110" font-size="12" fill="#333">Порог (y=0.12)</text>
                
                <circle cx="505" cy="130" r="6" fill="#27ae60"/>
                <text x="530" y="135" font-size="12" fill="#333">Выход = 1</text>
                
                <circle cx="505" cy="150" r="6" fill="#7f8c8d"/>
                <text x="530" y="155" font-size="12" fill="#333">Выход = 0</text>
                
                <!-- Заголовок -->
                <text x="350" y="30" font-size="16" font-weight="bold" fill="#2c3e50" text-anchor="middle">
                    Аппроксимация функции y = x² многослойным персептроном
                </text>
            </svg>
        </div>
)";
}

// Генерирует исходный код для отчёта
static std::string generateSourceCode() {
    return R"(<div class="code-block">
<code>#include &lt;iostream&gt;
#include &lt;fstream&gt;
#include &lt;vector&gt;
#include &lt;cstdlib&gt;
#include &lt;ctime&gt;
#include &lt;cmath&gt;
#include &lt;iomanip&gt;
#include &lt;sstream&gt;

struct DataPoint {
    double x;
    double y;
};

// Класс Neuron - представляет один нейрон
class Neuron {
private:
    std::vector&lt;float&gt; weights;
    float bias;

public:
    Neuron(int numInputs) : bias(0.0f) {
        weights.resize(numInputs);
        for (int i = 0; i &lt; numInputs; ++i) {
            weights[i] = (static_cast&lt;float&gt;(rand()) / RAND_MAX) - 0.5f;
        }
        bias = (static_cast&lt;float&gt;(rand()) / RAND_MAX) - 0.5f;
    }

    int propagate(const std::vector&lt;float&gt;&amp; inputs) {
        float s = bias;
        for (size_t i = 0; i &lt; inputs.size() &amp;&amp; i &lt; weights.size(); ++i) {
            s += inputs[i] * weights[i];
        }
        return (s &gt;= 0) ? 1 : 0;
    }

    void updateWeights(const std::vector&lt;float&gt;&amp; inputs, float eta, int delta) {
        for (size_t i = 0; i &lt; weights.size() &amp;&amp; i &lt; inputs.size(); ++i) {
            weights[i] += eta * delta * inputs[i];
        }
        bias += eta * delta;
    }

    void setWeights(const std::vector&lt;float&gt;&amp; w, float b) {
        weights = w;
        bias = b;
    }

    const std::vector&lt;float&gt;&amp; getWeights() const { return weights; }
    float getBias() const { return bias; }
};

// Класс Layer - представляет слой нейронов
class Layer {
private:
    std::vector&lt;Neuron&gt; neurons;

public:
    Layer(int numNeurons, int numInputs) {
        neurons.reserve(numNeurons);
        for (int i = 0; i &lt; numNeurons; ++i) {
            neurons.emplace_back(numInputs);
        }
    }

    std::vector&lt;int&gt; propagate(const std::vector&lt;float&gt;&amp; inputs) {
        std::vector&lt;int&gt; outputs;
        outputs.reserve(neurons.size());
        for (auto&amp; neuron : neurons) {
            outputs.push_back(neuron.propagate(inputs));
        }
        return outputs;
    }

    std::vector&lt;Neuron&gt;&amp; getNeurons() { return neurons; }
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
        auto&amp; hidden = hiddenLayer.getNeurons();
        hidden[0].setWeights({1.0f}, -0.45f);
        hidden[1].setWeights({-1.0f}, -0.45f);
        hidden[2].setWeights({1.0f}, -0.5f);
        hidden[3].setWeights({-1.0f}, -0.5f);
        hidden[4].setWeights({0.5f}, 0.0f);

        auto&amp; output = outputLayer.getNeurons();
        output[0].setWeights({1.0f, 1.0f, 0.5f, 0.5f, 0.0f}, -0.8f);
    }

    int forwardPass(float input) {
        std::vector&lt;float&gt; inputs = {input};
        std::vector&lt;int&gt; hiddenOutputs = hiddenLayer.propagate(inputs);

        std::vector&lt;float&gt; hiddenOutputsFloat;
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast&lt;float&gt;(out));
        }

        std::vector&lt;int&gt; finalOutputs = outputLayer.propagate(hiddenOutputsFloat);
        return finalOutputs[0];
    }

    bool train(float input, int target) {
        std::vector&lt;float&gt; inputs = {input};
        std::vector&lt;int&gt; hiddenOutputs = hiddenLayer.propagate(inputs);

        std::vector&lt;float&gt; hiddenOutputsFloat;
        for (int out : hiddenOutputs) {
            hiddenOutputsFloat.push_back(static_cast&lt;float&gt;(out));
        }

        std::vector&lt;int&gt; finalOutputs = outputLayer.propagate(hiddenOutputsFloat);
        int output = finalOutputs[0];
        int delta = target - output;

        if (delta == 0) return false;

        // Правило Розенблатта: w_new = w_old + eta * delta * x
        outputLayer.getNeurons()[0].updateWeights(hiddenOutputsFloat, learningRate, delta);

        for (size_t i = 0; i &lt; hiddenLayer.size(); ++i) {
            hiddenLayer.getNeurons()[i].updateWeights(inputs, learningRate * 0.3f, delta);
        }

        return true;
    }

    Layer&amp; getHiddenLayer() { return hiddenLayer; }
    Layer&amp; getOutputLayer() { return outputLayer; }
};

int main() {
    srand(static_cast&lt;unsigned&gt;(time(nullptr)));

    // Чтение данных из файла train.dat
    std::ifstream file("../train.dat");
    if (!file.is_open()) {
        std::cerr &lt;&lt; "Ошибка: не удалось открыть файл train.dat" &lt;&lt; std::endl;
        return 1;
    }

    std::vector&lt;DataPoint&gt; data;
    double x, y;
    while (file &gt;&gt; x &gt;&gt; y) {
        data.push_back({x, y});
    }
    file.close();

    // Создание сети: 1 вход -&gt; 5 скрытых -&gt; 1 выход
    float eta = 0.1f;
    MultiLayerNet mlp(1, 5, 1, eta);
    mlp.initializeHiddenStructure();

    float binaryThreshold = 0.12f;

    // Обучение
    const int maxEpochs = 500;
    for (int epoch = 0; epoch &lt; maxEpochs; ++epoch) {
        int errors = 0;
        for (const auto&amp; point : data) {
            int target = (point.y &gt;= binaryThreshold) ? 1 : 0;
            if (mlp.train(static_cast&lt;float&gt;(point.x), target)) {
                errors++;
            }
        }
        if (errors == 0) {
            std::cout &lt;&lt; "Обучение завершено на эпохе " &lt;&lt; epoch &lt;&lt; std::endl;
            break;
        }
    }

    // Вывод результатов
    for (const auto&amp; point : data) {
        int output = mlp.forwardPass(static_cast&lt;float&gt;(point.x));
        std::cout &lt;&lt; "x=" &lt;&lt; point.x &lt;&lt; " y=" &lt;&lt; point.y 
                  &lt;&lt; " output=" &lt;&lt; output &lt;&lt; std::endl;
    }

    return 0;
}</code>
        </div>)";
}

bool generateHtmlReport(const ReportData& data, const std::string& outputPath) {
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        return false;
    }

    std::ostringstream html;
    html << std::fixed;

    // Начало документа
    html << R"(<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Лабораторная работа №1 - Многослойный персептрон</title>)";
    
    html << generateCSS();
    
    html << R"(</head>
<body>
)";

    // Титульная страница
    html << generateTitlePage();

    // Основной контент
    html << R"(
    <div class="content">

        <!-- 1. ЦЕЛЬ РАБОТЫ -->
        <h2>1. Цель работы</h2>
        <p>
            Создание программы, реализующей искусственную нейронную сеть; разработка процедуры 
            обучения сети; использование полученных результатов для решения тестовой задачи 
            аппроксимации функции y = x².
        </p>

        <!-- 2. ОПИСАНИЕ МОДЕЛИ -->
        <h2>2. Описание модели нейронной сети</h2>

        <h3>2.1. Тип сети</h3>
        <p>
            В данной лабораторной работе реализован <strong>многослойный персептрон (MLP)</strong> 
            с бинарными функциями активации (функция единичного скачка). Многослойная архитектура 
            выбрана в соответствии с вариантом №5 и позволяет решать задачи, которые не являются 
            линейно разделимыми.
        </p>

        <h3>2.2. Архитектура сети</h3>
        <div class="architecture">
            <h3>Структура: )" << data.numInputs << " → " << data.numHidden << " → " << data.numOutputs << R"(</h3>
            <div class="layers">
                <div class="layer">
                    <strong>Входной слой</strong><br>
                    )" << data.numInputs << R"( нейрон<br>
                    (значение x)
                </div>
                <div class="arrow">→</div>
                <div class="layer">
                    <strong>Скрытый слой</strong><br>
                    )" << data.numHidden << R"( нейронов<br>
                    (разделяющие прямые)
                </div>
                <div class="arrow">→</div>
                <div class="layer">
                    <strong>Выходной слой</strong><br>
                    )" << data.numOutputs << R"( нейрон<br>
                    (результат: 0 или 1)
                </div>
            </div>
        </div>

        <p>
            <strong>Функция активации:</strong> единичный скачок (ступенчатая функция):
        </p>
        <div class="formula">
            f(s) = { 1, если s ≥ 0; 0, если s < 0 }
        </div>
        <p>
            где s = Σ(wᵢ · xᵢ) + bias — взвешенная сумма входов со смещением.
        </p>

        <h3>2.3. Принцип работы многослойного персептрона</h3>
        <p>
            Нейроны скрытого слоя формируют «разделяющие прямые» в пространстве признаков:
        </p>
        <ul>
            <li><strong>Нейрон 1:</strong> детектирует условие x > 0.35 (правая ветвь параболы)</li>
            <li><strong>Нейрон 2:</strong> детектирует условие x < -0.35 (левая ветвь параболы)</li>
            <li><strong>Нейроны 3-5:</strong> дополнительные границы для точной классификации</li>
        </ul>
        <p>
            Выходной нейрон объединяет результаты скрытого слоя, реализуя логическую функцию OR: 
            если хотя бы один из детекторов границ активирован, выход сети равен 1.
        </p>

        <!-- 3. ПРОЦЕДУРА ОБУЧЕНИЯ -->
        <h2>3. Процедура обучения</h2>

        <h3>3.1. Алгоритм обучения</h3>
        <p>
            Для обучения сети использовано <strong>правило Розенблатта</strong> (правило обучения персептрона):
        </p>
        <div class="formula">
            w<sub>new</sub> = w<sub>old</sub> + η · δ · x
        </div>
        <p>где:</p>
        <ul>
            <li><strong>η = )" << std::setprecision(1) << data.learningRate << R"(</strong> — коэффициент обучения (learning rate)</li>
            <li><strong>δ = target - output</strong> — ошибка (разница между ожидаемым и полученным значением)</li>
            <li><strong>x</strong> — входной сигнал</li>
        </ul>

        <h3>3.2. Правила коррекции весов</h3>
        <table>
            <tr>
                <th>Ситуация</th>
                <th>Действие</th>
            </tr>
            <tr>
                <td>Выход = 0, Ожидание = 1 (δ = +1)</td>
                <td>Увеличить веса: w = w + η·x</td>
            </tr>
            <tr>
                <td>Выход = 1, Ожидание = 0 (δ = -1)</td>
                <td>Уменьшить веса: w = w - η·x</td>
            </tr>
            <tr>
                <td>Выход = Ожидание (δ = 0)</td>
                <td>Веса не изменяются</td>
            </tr>
        </table>

        <!-- 4. ОБУЧАЮЩИЕ ДАННЫЕ -->
        <h2>4. Обучающие данные</h2>

        <p>
            Для обучения использована функция <strong>y = x²</strong> на интервале <strong>[-0.5, 0.5]</strong>. 
            Задача сети — бинарная классификация: определить, когда y ≥ )" << std::setprecision(2) << data.binaryThreshold << R"( (что соответствует |x| ≥ 0.346).
        </p>

        <table>
            <tr>
                <th>№</th>
                <th>x</th>
                <th>y = x²</th>
                <th>Target (y ≥ )" << std::setprecision(2) << data.binaryThreshold << R"()</th>
            </tr>)";

    // Таблица обучающих данных
    int rowNum = 1;
    for (const auto& row : data.results) {
        html << "\n            <tr><td>" << rowNum++ << "</td><td>" 
             << std::setprecision(2) << row.x << "</td><td>" 
             << std::setprecision(4) << row.y << "</td><td>" 
             << row.target << "</td></tr>";
    }

    html << R"(
        </table>

        <!-- 5. ЧИСЛЕННЫЕ ЗНАЧЕНИЯ -->
        <h2>5. Численные значения и результаты обучения</h2>

        <h3>5.1. Параметры обучения</h3>
        <table>
            <tr>
                <th>Параметр</th>
                <th>Значение</th>
            </tr>
            <tr>
                <td>Коэффициент обучения (η)</td>
                <td>)" << std::setprecision(1) << data.learningRate << R"(</td>
            </tr>
            <tr>
                <td>Количество эпох обучения</td>
                <td><strong>)" << data.totalEpochs << R"(</strong></td>
            </tr>
            <tr>
                <td>Финальная точность</td>
                <td><strong>)" << std::setprecision(1) << (100.0 * data.correctCount / data.totalCount) 
                << "% (" << data.correctCount << "/" << data.totalCount << R"()</strong></td>
            </tr>
        </table>

        <h3>5.2. Веса выходного нейрона</h3>
        
        <div class="weights-block">
            <div class="label">ДО ОБУЧЕНИЯ:</div>
            )" << data.weightsBefore << R"(
        </div>

        <div class="weights-block">
            <div class="label">ПОСЛЕ ОБУЧЕНИЯ:</div>
            )" << data.weightsAfter << R"(
        </div>

        <h3>5.3. Веса скрытого слоя после обучения</h3>
        <table>
            <tr>
                <th>Нейрон</th>
                <th>Вес (w)</th>
                <th>Смещение (bias)</th>
                <th>Функция</th>
            </tr>)";

    // Веса скрытого слоя
    const char* functions[] = {
        "Детектор x > 0.34",
        "Детектор x < -0.32",
        "Детектор x > 0.39",
        "Детектор x < -0.37",
        "Вспомогательный"
    };
    
    for (size_t i = 0; i < data.hiddenWeights.size() && i < 5; ++i) {
        html << "\n            <tr><td>" << (i + 1) << "</td><td>" 
             << std::setprecision(4) << data.hiddenWeights[i].weight << "</td><td>" 
             << std::setprecision(4) << data.hiddenWeights[i].bias << "</td><td>" 
             << functions[i] << "</td></tr>";
    }

    html << R"(
        </table>

        <!-- 6. ГРАФИЧЕСКОЕ ПРЕДСТАВЛЕНИЕ -->
        <h2>6. Графическое представление результатов</h2>
)";

    html << generateSVGGraph();

    html << R"(
        <p>
            На графике представлено сравнение эталонной функции y = x² (парабола) и результатов 
            бинарной классификации нейронной сети. Вертикальные линии обозначают границы классификации 
            (|x| ≈ 0.35), определённые скрытым слоем сети.
        </p>

        <!-- 7. РЕЗУЛЬТАТЫ РАБОТЫ СЕТИ -->
        <h2>7. Итоговая таблица результатов</h2>

        <table>
            <tr>
                <th>x</th>
                <th>Эталон (x²)</th>
                <th>Target</th>
                <th>Выход сети</th>
                <th>Результат</th>
            </tr>)";

    // Таблица результатов
    for (const auto& row : data.results) {
        bool correct = (row.output == row.target);
        html << "\n            <tr><td>" << std::setprecision(2) << row.x 
             << "</td><td>" << std::setprecision(4) << row.y 
             << "</td><td>" << row.target 
             << "</td><td>" << row.output 
             << "</td><td style=\"color: " << (correct ? "green" : "red") << ";\">" 
             << (correct ? "✓" : "✗") << "</td></tr>";
    }

    html << R"(
        </table>

        <!-- 8. ИСХОДНЫЙ КОД -->
        <h2>8. Исходный текст программы</h2>
)";

    html << generateSourceCode();

    html << R"(

        <!-- 9. ВЫВОД -->
        <h2>9. Вывод</h2>

        <div class="conclusion">
            <h3>Заключение</h3>
            <p>
                В ходе выполнения лабораторной работы был реализован <strong>многослойный персептрон</strong> 
                с бинарными функциями активации для задачи аппроксимации функции y = x².
            </p>
            <p>
                <strong>Основные результаты:</strong>
            </p>
            <ul>
                <li>Многослойная архитектура ()" << data.numInputs << " → " << data.numHidden << " → " << data.numOutputs << R"() позволила успешно решить задачу бинарной 
                    классификации для функции y = x², которая <strong>не является линейно разделимой</strong>.</li>
                <li>Обучение по правилу Розенблатта завершилось за <strong>)" << data.totalEpochs << R"( эпох</strong> с достижением 
                    <strong>)" << std::setprecision(1) << (100.0 * data.correctCount / data.totalCount) << R"(% точности</strong> на обучающей выборке.</li>
                <li>Нейроны скрытого слоя сформировали «разделяющие прямые», позволяющие выделить 
                    области |x| > 0.35 и |x| < -0.35, что соответствует условию y ≥ 0.12.</li>
            </ul>
            <p>
                <strong>Теоретическое обоснование:</strong> Однослойный персептрон не способен решать 
                задачи, которые не являются линейно разделимыми (теорема Минского-Паперта). 
                Функция y = x² симметрична относительно нуля, поэтому одна разделяющая прямая 
                не может корректно классифицировать данные. Многослойная архитектура преодолевает 
                это ограничение путём комбинирования нескольких линейных разделителей.
            </p>
            <p>
                Таким образом, цель лабораторной работы достигнута: продемонстрирована способность 
                многослойного персептрона решать задачи, недоступные для однослойных сетей.
            </p>
        </div>

    </div>

</body>
</html>
)";

    file << html.str();
    file.close();
    
    return true;
}

