#pragma once

#include <mutex>
#include <map>
#include <unordered_map>
#include <vector>
#include "transaction.hpp"

class VersionedVector {
private:
    std::mutex row_lock; // It will prevent multiple writers from writing to the same row at the same time
    std::map<std::uint64_t, std::vector<float>> versions; // {transaction_id, vector/row}
public:
    void write_row(std::uint64_t transaction_id, std::vector<float> new_vector);
    std::vector<float> read_row(TransactionManager& tm, std::uint64_t transaction_id);
};

class VectorStore {
private:

public:
    void get_row(std::uint64_t row_id);
};

/*
Documentation
*/
class VectorStore {
private:
    std::mutex store_lock;
    std::unordered_map<std::uint64_t, std::shared_ptr<VersionedVector>> store;

public:
    std::shared_ptr<VersionedVector> get_row(std::uint64_t vector_id);
};