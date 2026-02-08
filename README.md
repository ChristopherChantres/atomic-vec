# Atomic-Vec

A thread-safe, snapshot-isolated, MVCC vector database engine built from scratch in C++20.

Atomic-Vec enables concurrent read and write operations on vector data through multi-version concurrency control (MVCC). Each transaction operates on a consistent snapshot of the data, and write conflicts are detected and rejected automatically — no locks are held during reads.

## How It Works

Atomic-Vec implements snapshot isolation using a logical clock and per-transaction timestamps:

1. **Logical Clock** — A monotonically increasing counter that advances on each commit, establishing a global ordering of committed transactions.
2. **Snapshot on Begin** — When a transaction starts, it captures the current logical clock value as its `begin_time`. This defines the snapshot boundary: only versions committed *before* this time are visible.
3. **Commit Timestamp** — When a transaction commits, it records the current logical clock as its `commit_time`, then increments the clock.
4. **Visibility Rule** — A version written by transaction `T_target` is visible to transaction `T_viewer` if and only if:
   - `T_target == T_viewer` (a transaction always sees its own writes), **or**
   - `T_target` has committed **and** `commit_time(T_target) < begin_time(T_viewer)`
5. **Write Conflict Detection** — Before writing a new version to a row, the engine checks whether the most recent version was written by a visible transaction. If it was written by an uncommitted or not-yet-visible transaction, the write is rejected and the caller must roll back.

## Project Structure

```
atomic-vec/
├── include/
│   ├── atomic_vec.hpp        # VersionedVector and VectorStore declarations
│   └── transaction.hpp       # TransactionManager declaration
├── src/
│   ├── atomic_vec.cpp        # VersionedVector and VectorStore implementations
│   ├── main.cpp              # Entry point with concurrent write conflict demo
│   └── transaction.cpp       # TransactionManager implementation
├── .gitignore
├── CMakeLists.txt
├── README.md
└── run.sh                    # Build and run script
```

## API Reference

### TransactionManager

A thread-safe transaction manager that tracks the full transaction lifecycle (active, committed, aborted) and provides snapshot-based visibility checks using a logical clock.

| Method | Description |
|--------|-------------|
| `begin()` | Starts a new transaction, records its `begin_time`, and returns the transaction ID |
| `commit(uint64_t id)` | Moves a transaction from active to committed and records its `commit_time` |
| `rollback(uint64_t id)` | Moves a transaction from active to aborted |
| `is_visible(uint64_t target_id, uint64_t viewer_id)` | Returns `true` if the version written by `target_id` is visible to `viewer_id` |
| `get_start_time(uint64_t id)` | Returns the begin timestamp of a transaction, or `INVALID_TIME` if not found |
| `get_commit_time(uint64_t id)` | Returns the commit timestamp of a transaction, or `INVALID_TIME` if not found |

### VersionedVector

Represents a single row with MVCC support. Each version is keyed by the transaction ID that wrote it.

| Method | Description |
|--------|-------------|
| `write_row(TransactionManager& tm, uint64_t id, vector<float> data)` | Writes a new version if no conflict is detected; returns `true` on success, `false` on conflict |
| `read_row(TransactionManager& tm, uint64_t id)` | Returns the most recent visible version for the transaction, or an empty vector if none is visible |

### VectorStore

Manages a collection of `VersionedVector` rows, identified by unique IDs.

| Method | Description |
|--------|-------------|
| `get_row(uint64_t vector_id)` | Returns a shared pointer to the `VersionedVector` for the given ID; creates an empty row if it does not exist |

## Demo: Write Conflict Detection

The included `main.cpp` runs a concurrent scenario with two threads (**Chris** and **Kate**) both targeting the same row:

1. **Chris** begins a transaction and fetches row 1, then pauses to simulate processing.
2. **Kate** begins her transaction, writes to row 1, and commits — all while Chris is still thinking.
3. **Chris** resumes and attempts to write to row 1. The engine detects that the latest version was written by Kate's transaction, which committed *after* Chris's snapshot. The write is rejected and Chris rolls back.

```
[Chris] Started Tx: 1 (Snapshot Time: 0)
[Kate]  Started Tx: 2 (Snapshot Time: 0)
[Kate]  Write SUCCESS! Committing...
[Kate]  Committed.
[Chris] Attempting to write...
[Chris] Write FAILED! Conflict detected. Rolling back.
```

## Getting Started

### Prerequisites

- CMake 3.16+
- A C++20 compatible compiler (Clang, GCC, or MSVC)

On macOS, install CMake via Homebrew if needed:

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
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
./build/atomic-vec
```
