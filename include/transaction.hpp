#pragma once

#include <atomic>
#include <unordered_set>
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
    std::atomic<std::uint64_t> transaction_id{0};
    std::unordered_set<std::uint64_t> active_transactions;
    std::unordered_set<std::uint64_t> committed_transactions;
    std::unordered_set<std::uint64_t> aborted_transactions;

public:
    std::uint64_t begin();
    void commit(std::uint64_t transaction_id);
    void rollback(std::uint64_t transaction_id);
    bool is_writer_visible_to_reader(std::uint64_t writer_transaction_id, std::uint64_t reader_transaction_id);
};