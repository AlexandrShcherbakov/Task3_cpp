#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>


int main() {
    size_t count(0);
    std::atomic_bool done(false);
    std::queue<int> items;
    std::mutex m;
    std::thread producer([&]() {
        for (int i = 0; i < 10000; ++i) {
            // ... some code may be here ...
            std::lock_guard<std::mutex> l(m);
            items.push(i);
            count++;
        }
        done = true;
    });
    std::thread consumer([&]() {
        while (!done) {
            while (true) {
                m.lock();
                if (!items.empty()) {
                    items.pop();
                    count--;
                    m.unlock();
                    // ...
                } else {
                    m.unlock();
                    break;
                }
            }
        }
        while (!items.empty()) {
            items.pop();
            count--;
            // ...
        }
    });
    producer.join();
    consumer.join();
    std::cout << count << std::endl;
}