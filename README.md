# paralgo_qsort
Реализация параллельной версии алгоритма Quick Sort и сравнение с последовательной версией

Seq sort test 100kk bench.\
Run 1: 19.5113s\
Run 2: 19.3562s\
Run 3: 19.3268s\
Run 4: 19.6403s\
Run 5: 19.5744s\
AVG: 19.4818s

Par sort test 100kk bench (4 threads)\
Run 1: 9.2074s\
Run 2: 7.6613s\
Run 3: 4.3294s\
Run 4: 4.3602s\
Run 5: 4.3127s\
AVG: 5.9742s

Speedup: 3.2610x

Для запуска\
`mkdir build`\
`cd build`\
`cmake ..`\
`cmake --build .`\
`PARLAY_NUM_THREADS=4 ./paralgo`
