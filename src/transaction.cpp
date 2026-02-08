#include "transaction.hpp"

std::uint64_t TransactionManager::begin() {
    std::lock_guard<std::mutex> lock(transaction_manager_mutex); // Automatically releases the lock when the scope ends
    transaction_id++;
    begin_times[transaction_id] = logical_clock.load();
    active_transactions.emplace(transaction_id);
    return transaction_id;
}

void TransactionManager::commit(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(transaction_manager_mutex);
    if (active_transactions.count(transaction_id)){
        active_transactions.erase(transaction_id);
        commit_times[transaction_id] = logical_clock.load();
        logical_clock++;
        committed_transactions.emplace(transaction_id);
        // Here we would write to disk "Redo log" (WAL)
    }
}

void TransactionManager::rollback(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(transaction_manager_mutex);
    if (active_transactions.count(transaction_id)){
        active_transactions.erase(transaction_id);
        aborted_transactions.emplace(transaction_id);
    }
}

bool TransactionManager::is_visible(std::uint64_t target_id, std::uint64_t viewer_id) {
    if (target_id == viewer_id) return true;
    if (commit_times.count(target_id) == 0) return false;
    return commit_times.at(target_id) < begin_times.at(viewer_id);
}

std::uint64_t TransactionManager::get_start_time(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(transaction_manager_mutex);
    auto it = begin_times.find(transaction_id);
    if (it == begin_times.end()) {
        return INVALID_TIME;
    }
    return it->second;
}

std::uint64_t TransactionManager::get_commit_time(std::uint64_t transaction_id) {
    std::lock_guard<std::mutex> lock(transaction_manager_mutex);
    auto it = commit_times.find(transaction_id);
    if (it == commit_times.end()) {
        return INVALID_TIME;
    }
    return it->second;
}