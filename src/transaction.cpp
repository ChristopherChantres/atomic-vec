#include "transaction.hpp"

std::uint64_t TransactionManager::begin() {
    std::lock_guard<std::mutex> lock(mutex); // Automatically releases the lock when the scope ends
    transaction_id++;
    active_transactions.emplace(transaction_id);
    return transaction_id;
}

void TransactionManager::commit(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(mutex);
    if (active_transactions.count(transaction_id)){
        active_transactions.erase(transaction_id);
        committed_transactions.emplace(transaction_id);
        // Here we would write to disk (WAL)
    }
}

void TransactionManager::rollback(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(mutex);
    if (active_transactions.count(transaction_id)){
        active_transactions.erase(transaction_id);
        aborted_transactions.emplace(transaction_id);
    }
}

// Checks if the writer transaction is visible to the reader transaction
bool TransactionManager::is_visible(std::uint64_t reader_transaction_id, std::uint64_t writer_transaction_id) {
    if (reader_transaction_id == writer_transaction_id) {
        return true;
    } else if (committed_transactions.count(writer_transaction_id)) {
        return true;
    } else {
        return false;
    }
}