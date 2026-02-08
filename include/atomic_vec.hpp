#pragma once

#include <mutex>
#include <map>
#include <unordered_map>
#include <vector>
#include "transaction.hpp"

/*
VersionedVector represents a single row that supports multi-version concurrency control (MVCC).
Each version of the row is associated with a transaction ID and a vector of floats.

Methods:
- `write_row(TransactionManager& tm, std::uint64_t transaction_id, std::vector<float> new_vector)`:
    Writes a new version if no conflict; returns true on success, false on conflict.
- `read_row(TransactionManager& tm, std::uint64_t transaction_id)`:
    Reads the visible version for the transaction; returns data or an empty vector.
*/
class VersionedVector {
private:
    std::mutex row_lock; // It will prevent multiple writers from writing to the same row at the same time
    std::map<std::uint64_t, std::vector<float>> versions; // {transaction_id, vector/row}
public:
    bool write_row(TransactionManager& tm, std::uint64_t transaction_id, std::vector<float> new_vector);
    std::vector<float> read_row(TransactionManager& tm, std::uint64_t transaction_id);
};


/*
VectorStore manages a collection of VersionedVectors (rows), identified by unique IDs, supporting concurrent multi-versioned access for transactional operations.

Methods:
- `get_row(std::uint64_t vector_id)`: Returns a shared pointer to a VersionedVector associated with vector_id. If the row does not exist, an empty one is created.
*/
class VectorStore {
private:
    std::mutex vector_store_lock;
    std::unordered_map<std::uint64_t, std::shared_ptr<VersionedVector>> vector_store; // {vector_id/row_id, vector}

public:
    std::shared_ptr<VersionedVector> get_row(std::uint64_t vector_id);
};