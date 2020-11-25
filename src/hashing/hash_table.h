#include <cmath>
#include <vector>

constexpr int EMPTY_RECORD_KEY = -1;

template <typename T>
struct record {
    record() { this->key = EMPTY_RECORD_KEY; }

    record(long key, T const &data) {
        this->key = key;
        this->data = data;
    }

    bool empty() { return this->key == EMPTY_RECORD_KEY; }

    long key;
    T data;
};

template <typename T>
class hash_table {
  public:
    hash_table(unsigned int m) { this->table.resize(m); }

    std::vector<record<T>> table;

    T *insert(long key, T const &data) {
        record<T> *record = this->probe(key);
        if (record == nullptr) {
            return nullptr;
        }

        record->key = key;
        record->data = data;

        return &record->data;
    }

    T *search(long key) {
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

    void to_vector(std::vector<T> &vetor) {
        for (auto &record : this->table) {
            if (!record.empty()) {
                vetor.push_back(record.data);
            }
        }
    }

  private:
    unsigned int probe(long key, record<T> *&result) {
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

    record<T> *probe(long key) {
        record<T> *record = nullptr;
        this->probe(key, record);

        return record;
    }

    unsigned int probe_collisions(long key) {
        record<T> *_ = nullptr;
        return this->probe(key, _);
    }

    int hash(long key, int index) {
        unsigned int m = this->table.size();
        return (h1(key, m) + index * h2(key, m)) % m;
    }

    int h1(long k, unsigned int m) { return k % m; }

    int h2(long k, unsigned int m) {
        double A = (sqrt(5) - 1) / 2;
        return floor(m * (fmod(k * A, 1)));
    }
};
