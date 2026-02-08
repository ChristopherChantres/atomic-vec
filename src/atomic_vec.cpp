#include "atomic_vec.hpp"
#include <transaction.hpp>

bool VersionedVector::write_row(TransactionManager& tm, std::uint64_t transaction_id, std::vector<float> new_vector) {
    std::lock_guard<std::mutex> lock(row_lock);
    // Snapshot logic
    if (!versions.empty()) {
        std::uint64_t last_writer_transaction_id = versions.rbegin()->first;

        if (last_writer_transaction_id != transaction_id) {
            if (!tm.is_visible(last_writer_transaction_id, transaction_id)) {
                return false;
            }
        }
    }

    // We are allowed to write to the row
    versions[transaction_id] = new_vector;
    return true;
}

std::vector<float> VersionedVector::read_row(TransactionManager& tm, std::uint64_t transaction_id) {
    // MVCC: Read the row from the versioned vector without locking the row
    for (auto it = versions.rbegin(); it != versions.rend(); ++it) {
        if (tm.is_visible(it->first, transaction_id)) {
            return it->second;
        }
    }

    // If no version is visible, return an empty vector
    static const std::vector<float> empty_vec;
    return empty_vec;
}

std::shared_ptr<VersionedVector> VectorStore::get_row(std::uint64_t vector_id) {
    std::lock_guard<std::mutex> lock(vector_store_lock);
    if (vector_store.find(vector_id) == vector_store.end()) {
        vector_store[vector_id] = std::make_shared<VersionedVector>();
    }
    return vector_store.at(vector_id);
}