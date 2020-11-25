#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include "../parsing/csv_parser.h"
#include "../structures/book.h"
#include "common.h"

void read_books(std::vector<book> &books) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Falha ao tentar abrir `books.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        book b;

        parser.get(0, b.authors);
        parser.get(1, b.bestsellers_rank);
        parser.get(2, b.categories);
        parser.get(3, b.edition);
        parser.get(4, b.id);
        parser.get(5, b.isbn10);
        parser.get(6, b.isbn13);
        parser.get(7, b.rating);
        parser.get(8, b.rating_count);
        parser.get(9, b.title);

        books.push_back(std::move(b));
    }
}

void shuffle(std::vector<book> &books) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(books.begin(), books.end(), std::default_random_engine(seed));
}
