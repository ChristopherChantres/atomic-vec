#pragma once

#include <atomic>
#include <unordered_set>
#include <mutex>

/*
Methods:
    - begin(): Creates a new transaction ID and adds it to the active transactions set.
    - commit(int transaction_id): Adds the transaction ID to the committed transactions set.
    - abort(int transaction_id): Adds the transaction ID to the aborted transactions set.
    - is_visible(int reader_transaction_id, int writer_transaction_id): Checks if the writer transaction is visible to the reader transaction.
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
    bool is_visible(std::uint64_t reader_transaction_id, std::uint64_t writer_transaction_id);
};