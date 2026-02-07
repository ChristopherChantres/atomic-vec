# Atomic-Vec

A C++20 vector store with built-in MVCC (Multi-Version Concurrency Control) transaction support. Atomic-Vec enables concurrent read and write operations on vector data through thread-safe transaction management, ensuring data consistency and isolation between transactions.

## Project Structure

```
atomic-vec/
├── include/
│   ├── atomic_vec.hpp        # AtomicVec class declaration
│   └── transaction.hpp       # TransactionManager class declaration
├── src/
│   ├── atomic_vec.cpp        # AtomicVec method implementations
│   ├── main.cpp              # Entry point
│   └── transaction.cpp       # TransactionManager method implementations
├── .gitignore
├── CMakeLists.txt
├── README.md
└── run.sh                    # Build and run script
```

## TransactionManager

A thread-safe transaction manager that tracks transaction lifecycle (active, committed, aborted) and supports visibility checks between transactions.

### Methods

| Method | Description |
|--------|-------------|
| `begin()` | Creates a new transaction and returns its ID |
| `commit(int transaction_id)` | Moves a transaction from active to committed |
| `rollback(int transaction_id)` | Moves a transaction from active to aborted |
| `is_visible(int reader_id, int writer_id)` | Checks if a writer's changes are visible to a reader |

## Getting Started

### Prerequisites

- CMake 3.16+
- A C++20 compatible compiler (Clang, GCC, or MSVC)

On macOS, install CMake via Homebrew if you don't have it:

```bash
brew install cmake
```

### Quick Start

```bash
git clone https://github.com/<your-username>/atomic-vec.git
cd atomic-vec
chmod +x run.sh
./run.sh
```

### Manual Build & Run

```bash
git clone https://github.com/<your-username>/atomic-vec.git
cd atomic-vec
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
./build/atomic-vec
```