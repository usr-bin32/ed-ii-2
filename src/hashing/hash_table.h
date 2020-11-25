#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cmath>
#include <vector>

constexpr int EMPTY_RECORD_KEY = -1;

// Representa um registro dentro de uma tabela hash. Somente valores positivos
// devem ser utilizados como chave. O valor -1 é reservado para indicar que uma
// posição está vazia
template <typename T>
struct record {
    record() { this->key = EMPTY_RECORD_KEY; }

    record(long key, T const &value) {
        this->key = key;
        this->value = value;
    }

    bool empty() { return this->key == EMPTY_RECORD_KEY; }

    long key;
    T value;
};

// Implementa uma tabela hash de valores genéricos que utiliza sondagem dupla
template <typename T>
class hash_table {
  public:
    hash_table(unsigned int m) { this->table.resize(m); }

    std::vector<record<T>> table;

    // Insere um par (chave, valor) na tabela hash
    T *insert(long key, T const &value) {
        record<T> *record = this->probe(key);
        if (record == nullptr) {
            // caso não haja espaço disponível na tabela
            return nullptr;
        }

        record->key = key;
        record->value = value;

        return &record->value;
    }

    // Realiza uma busca de chave na tabela hash e retorna o valor associado, se
    // houver
    T *search(long key) {
        record<T> *record = this->probe(key);
        if (record == nullptr || record->empty()) {
            return nullptr;
        }
        return &record->value;
    }

    // Computa o número de colisões na tabela hash
    unsigned int collisions() {
        unsigned int count = 0;
        for (auto &record : this->table) {
            if (!record.empty()) {
                count += this->probe_collisions(record.key);
            }
        }
        return count;
    }

    // Preenche um vector com os valores armazenados na tabela
    void to_vector(std::vector<T> &vec) {
        for (auto &record : this->table) {
            if (!record.empty()) {
                vec.push_back(record.value);
            }
        }
    }

  private:
    // Realiza a sondagem dupla de uma chave e determina o endereço de memória
    // da posição da tabela onde o elemento com essa chave está ou deve ser
    // inserido, bem como retorna o número de colisões no processo
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

    // Método para tornar a sondagem com o objetivo de obter uma posição da
    // memória mais conveniente
    record<T> *probe(long key) {
        record<T> *record = nullptr;
        this->probe(key, record);

        return record;
    }

    // Método para tornar a sondagem com o objetivo de obter o número de
    // colisões mais conveniente
    unsigned int probe_collisions(long key) {
        record<T> *_ = nullptr;
        return this->probe(key, _);
    }

    // Faz o cálculo de uma hash dupla
    int hash(long key, int index) {
        unsigned int m = this->table.size();
        return (h1(key, m) + index * h2(key, m)) % m;
    }

    // h1 usa o método da divisão
    int h1(long k, unsigned int m) { return k % m; }

    // h2 usa o método da multiplicação
    int h2(long k, unsigned int m) {
        // valor sugerido por Donald Knuth
        double A = (sqrt(5) - 1) / 2;
        return floor(m * (fmod(k * A, 1)));
    }
};

#endif
