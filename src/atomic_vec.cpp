#include "atomic_vec.hpp"
#include <transaction.hpp>

void VersionedVector::write_row(std::uint64_t transaction_id, std::vector<float> new_vector) {
    std::lock_guard<std::mutex> lock(row_lock);
    versions[transaction_id] = new_vector;
}

std::vector<float> VersionedVector::read_row(TransactionManager& tm, std::uint64_t transaction_id) {
    // MVCC: Read the row from the versioned vector without locking the row
    for (auto it = versions.rbegin(); it != versions.rend(); ++it) {
        if (tm.is_writer_visible_to_reader(it->first, transaction_id)) {
            return it->second;
        }
    }

    // If no version is visible, return an empty vector
    static const std::vector<float> empty_vec;
    return empty_vec;
}

std::shared_ptr<VersionedVector> VectorStore::get_row(std::uint64_t vector_id) {
    std::lock_guard<std::mutex> lock(store_lock);
    if (store.find(vector_id) == store.end()) {
        store[vector_id] = std::make_shared<VersionedVector>();
    }
    return store.at(vector_id);
}