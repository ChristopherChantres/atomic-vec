#pragma once

#include <atomic>
#include <unordered_set>
#include <unordered_map>
#include <mutex>

/*
Methods:
    - `begin()`: Creates a new transaction ID and adds it to the active transactions set.
    - `commit(uint64_t transaction_id)`: Adds the transaction ID to the committed transactions set.
    - `rollback(uint64_t transaction_id)`: Adds the transaction ID to the aborted transactions set.
    - `is_writer_visible_to_reader(uint64_t writer_transaction_id, uint64_t reader_transaction_id)`: Determines if a version written by writer_transaction_id should be visible to reader_transaction_id, supporting MVCC (Multi-Version Concurrency Control) semantics for consistent non-blocking reads.
*/
class TransactionManager {
private:
    std::mutex mutex;

    std::atomic<uint64_t> logical_clock{0};
    std::unordered_map<std::uint64_t, std::uint64_t> begin_times; // {transaction_id, begin_time}
    std::unordered_map<std::uint64_t, std::uint64_t> commit_times; // {transaction_id, begin_time}

    std::atomic<std::uint64_t> transaction_id{0};
    std::unordered_set<std::uint64_t> active_transactions;
    std::unordered_set<std::uint64_t> committed_transactions;
    std::unordered_set<std::uint64_t> aborted_transactions;

public:
    std::uint64_t begin();
    void commit(std::uint64_t transaction_id);
    void rollback(std::uint64_t transaction_id);
    bool is_visible(std::uint64_t target_id, std::uint64_t viewer_id);
};