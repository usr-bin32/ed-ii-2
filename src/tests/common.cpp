#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include "../parsing/csv_parser.h"
#include "../structures/book.h"
#include "common.h"

void generate_indices(std::vector<int> &values, size_t size) {
    size_t length = values.size();

    // redimensiona o vector para que ele possa armazenar uma sequência de
    // `size` índices
    values.resize(size);

    // preenche o vector com todos os inteiros entre 0 até o tamano do vector
    std::iota(values.begin(), values.end(), 0);

    // embaralha os elementos do vector
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(values.begin(), values.end(),
                 std::default_random_engine(seed));

    // retorna o vector de entrada para seu tamanho original, de forma que o
    // vector tenha `length` índices aleatórios sem repetição
    values.resize(length);
}
