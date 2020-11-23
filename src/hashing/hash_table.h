#include <cmath>
#include <functional>
#include <vector>

constexpr int EMPTY_RECORD_KEY = -1;

template <typename T>
struct record {
    record() {
        this->key = EMPTY_RECORD_KEY;
    }

    record(int key, T const &data) {
        this->key = key;
        this->data = data;
    }

    bool empty() {
        return this->key == EMPTY_RECORD_KEY;
    }

    int key;
    T data;
};

template <typename T>
class hash_table {
  public:
    hash_table(unsigned int m) {
        this->table.resize(m);
    }

    T *insert(int key, T const &data) {
        record<T> *record = this->probe(key);
        if (record == nullptr) {
            return nullptr;
        }

        record->key = key;
        record->data = data;

        return &record->data;
    }

    T *lookup(int key) {
        record<T> *record = this->probe(key);
        if (record == nullptr || record->empty()) {
            return nullptr;
        }
        return &record->data;
    }

    unsigned int collisions() {
        unsigned int count = 0;
        for (auto &record : this->table) {
            if (!record.empty()) {
                count += this->probe_collisions(record.key);
            }
        }
        return count;
    }
    std::vector<record<T>> table;

  private:
    unsigned int probe(int key, record<T> *&result) {
        unsigned int i;
        for (i = 0; i < this->table.size(); i++) {
            record<T> *record = &this->table[hash(key, i)];

            if (record->empty() || record->key == key) {
                result = record;
                break;
            }
        }
        return i;
    }

    record<T> *probe(int key) {
        record<T> *record = nullptr;
        this->probe(key, record);

        return record;
    }

    unsigned int probe_collisions(int key) {
        record<T> *_ = nullptr;
        return this->probe(key, _);
    }

    int hash(int key, int index) {
        unsigned int m = this->table.size();
        return (h1(key, m) + index * h2(key, m)) % m;
    }

    int h1(int k, unsigned int m) {
        return k % m;
    }

    int h2(int k, int m) {
        double A = 0.6180339887;
        return static_cast<int>(m * (k * A - static_cast<int>(k * A)));
    }
};
