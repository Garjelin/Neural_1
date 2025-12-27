# Скрипт gnuplot для построения графика результатов
# Запуск: gnuplot plot_result.gp

set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'result.png'

set title "Аппроксимация функции y = x² многослойным персептроном" font 'Arial,14'
set xlabel "x"
set ylabel "y"

set grid
set key top center

set xrange [-0.6:0.6]
set yrange [-0.1:0.35]

# Эталонная функция y = x²
f(x) = x**2

# Порог классификации
threshold = 0.12

# Границы классификации |x| = 0.346
x_boundary = 0.346

# Построение графика
plot f(x) with lines lw 2 lc rgb "#3498db" title "Эталон y = x²", \
     threshold with lines lw 2 lc rgb "#e74c3c" dt 2 title "Порог классификации (y = 0.12)", \
     '-' using 1:2 with points pt 7 ps 1.5 lc rgb "#27ae60" title "Выход сети = 1", \
     '-' using 1:2 with points pt 7 ps 1.5 lc rgb "#95a5a6" title "Выход сети = 0"
-0.50 0.25
-0.45 0.2025
-0.40 0.16
-0.35 0.1225
0.35 0.1225
0.40 0.16
0.45 0.2025
0.50 0.25
e
-0.30 0.09
-0.25 0.0625
-0.20 0.04
-0.15 0.0225
-0.10 0.01
-0.05 0.0025
0.00 0.00
0.05 0.0025
0.10 0.01
0.15 0.0225
0.20 0.04
0.25 0.0625
0.30 0.09
e

print "График сохранён в result.png"

