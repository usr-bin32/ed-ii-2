#include <cmath>
#include <list>
#include <vector>

template <typename T>
struct hash_node {
    hash_node(unsigned int key, T const &data) {
        this->key = key;
        this->data = data;
    }

    unsigned int key;
    T data;
};

template <typename T>
class hash_table {
  public:
    hash_table(unsigned int m) {
        this->table.resize(m);
    }

    T *insert(unsigned int key, T const &data) {
        unsigned int index = this->hash_function(key, table.size());

        T *data_ptr = lookup_index(key, index);
        if (data_ptr != nullptr) {
            // caso a chave já esteja ocupada, não fazer nada
        } else {
            this->table[index].emplace_front(key, data);
            data_ptr = &this->table[index].front().data;
        }

        return data_ptr;
    }

    T *search(unsigned int key) {
        unsigned int index = this->hash_function(key, table.size());
        return lookup_index(key, index);
    }

    unsigned int collisions() {
        unsigned int result = 0;
        for (auto const &list : this->table) {
            if (list.empty()) {
                continue;
            }
            result += list.size() - 1;
        }
        return result;
    }

    void to_vector(std::vector<T> &vector) {
        for (auto const &list : this->table) {
            for (auto const &node : list) {
                vector.push_back(node.data);
            }
        }
    }

  private:
    std::vector<std::list<hash_node<T>>> table;

    T *lookup_index(unsigned int key, unsigned int index) {
        T *data_ptr = nullptr;
        for (auto &node : this->table[index]) {
            if (node.key == key) {
                data_ptr = &node.data;
                break;
            }
        }

        return data_ptr;
    }

    unsigned int hash_function(unsigned int key, unsigned int m) {
        double A = (sqrt(5) - 1) / 2;
        return floor(m * (fmod(key * A, 1)));
    }
};
