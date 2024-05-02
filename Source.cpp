#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

#define THREADS 6

std::mutex mut;

void threadSum(int& sum, int bound) {
    for (int i = 0; i < bound; ++i) {
        sum += 2;
    }
}

void threadSumMutex(int& sum, int bound) {
    for (int i = 0; i < bound; ++i) {
        std::lock_guard<std::mutex> lock(mut);
        sum += 2;
    }
}

int main() {
    int sum = 0;
    int bound = 1000000;

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    for (size_t i = 0; i < THREADS; ++i)
        threads.emplace_back(threadSum, std::ref(sum), bound);
    
    for (size_t i = 0; i < THREADS; ++i)
        threads[i].join();

    std::cout << "Sum is: " << sum << std::endl;
    return 0;
}
