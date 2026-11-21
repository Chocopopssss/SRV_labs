#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

// Пункт 1.1: Разминка
void warmup() {
    std::string s = "01234";
    // Этот цикл бесконечный и приведет к ошибке
    for (unsigned int i = s.size() - 1; i >= 0; i--) {
        std::cout << s[i] << std::endl;
    }
}

// Вспомогательная функция вычисления 10!
unsigned long long fact10() {
    unsigned long long res = 1;
    for (int i = 2; i <= 10; ++i) {
        res *= i;
    }
    return res;
}

// Пункт 1.2: 10 млн раз в Debug и Release 
void exercise1() {
    const int N = 10000000;
    volatile unsigned long long sink = 0; // volatile не дает компилятору вырезать цикл в Release

    clock_t start = clock();
    for (int i = 0; i < N; ++i) {
        sink += fact10();
    }
    clock_t end = clock();

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "Time (clock): " << seconds << " sec" << std::endl;
}

// Пункт 1.3: Потоки vs Последовательный запуск 
void Func(std::string name) {
    const int N = 10000000;
    volatile unsigned long long sink = 0;
    for (int i = 0; i < N; ++i) {
        sink += fact10();
    }
    // Если раскомментировать вывод ниже, без мьютекса строки в консоли могут перемешаться
    // std::cout << name << " finished\n";
}

void exercise2() {
    // 1. Параллельный запуск в двух потоках
    auto start_par = std::chrono::high_resolution_clock::now();

    std::thread thread1(Func, "t1");
    std::thread thread2(Func, "t2");
    thread1.join();
    thread2.join();

    auto end_par = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_par = end_par - start_par;
    std::cout << "Parallel threads time: " << diff_par.count() << " sec" << std::endl;

    // 2. Последовательный запуск
    auto start_seq = std::chrono::high_resolution_clock::now();

    Func("seq1");
    Func("seq2");

    auto end_seq = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_seq = end_seq - start_seq;
    std::cout << "Sequential time: " << diff_seq.count() << " sec" << std::endl;
}

int main() {
    //warmup();
    //exercise1();
    exercise2();

    system("pause");
    return 0;
}