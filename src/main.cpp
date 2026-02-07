#include "transaction.hpp"
#include <iostream>

int main() {
    TransactionManager tm;
    
    // Create two transactions
    std::uint64_t T1_id = tm.begin();
    std::uint64_t T2_id = tm.begin();

    bool is_visible = tm.is_visible(T1_id, T2_id);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "[T1 ≠ T2]\n";
    if (is_visible) {
        std::cout << "T2 is visible to T1" << std::endl;
    } else {
        std::cout << "T2 is not visible to T1" << std::endl;
    }
}