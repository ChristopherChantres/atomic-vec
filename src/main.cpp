#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include "transaction.hpp"
#include "atomic_vec.hpp"

// Global instances
TransactionManager tm;
VectorStore store;

void chris_thread() {
    // Start Transaction
    std::uint64_t t1 = tm.begin();
    std::cout << "[Chris] Started Tx: " << t1 << " (Snapshot Time: " << tm.get_start_time(t1) << ")" << std::endl;

    // Get Row
    auto row = store.get_row(1);

    // Simulate "Thinking" (Processing data)
    // While Chris thinks, Kate will sneak in and update the row
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Try to Write
    std::cout << "[Chris] Attempting to write..." << std::endl;
    std::vector<float> data = {1.0f, 1.0f};
    
    bool success = row->write_row(tm, t1, data);

    if (success) {
        std::cout << "[Chris] Write SUCCESS! Committing..." << std::endl;
        tm.commit(t1);
    } else {
        std::cout << "[Chris] Write FAILED! Conflict detected. Rolling back." << std::endl;
        tm.rollback(t1);
    }
}

void kate_thread() {
    // Let Chris start first
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Start Transaction
    std::uint64_t t2 = tm.begin();
    std::cout << "[Kate] Started Tx: " << t2 << " (Snapshot Time: " << tm.get_start_time(t2) << ")" << std::endl;

    // Get Row
    auto row = store.get_row(1);

    // Write Immediately
    std::vector<float> data = {2.0f, 2.0f};
    if (row->write_row(tm, t2, data)) {
        std::cout << "[Kate]   Write SUCCESS! Committing..." << std::endl;
        tm.commit(t2);
        std::cout << "[Kate]   Committed." << std::endl;
    } else {
        std::cout << "[Kate]   Write FAILED!" << std::endl;
        tm.rollback(t2);
    }
}

void test_01() {
    std::thread t1(chris_thread);
    std::thread t2(kate_thread);

    t1.join();
    t2.join();
}

int main() {
    test_01();
    return 0;
}