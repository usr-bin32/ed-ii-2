#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include "../parsing/csv_parser.h"
#include "../structures/book.h"
#include "common.h"

void generate_indices(std::vector<int> &values, size_t size) {
    size_t length = values.size();
    values.resize(size);

    std::iota(values.begin(), values.end(), 0);

    // randomiza o container e elimina os elementos após length.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(values.begin(), values.end(),
                 std::default_random_engine(seed));

    values.resize(length);
}
