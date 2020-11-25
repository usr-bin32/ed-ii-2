#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "../balanced/btree.h"
#include "../balanced/red_black.h"
#include "../parsing/csv_parser.h"
#include "../structures/book.h"
#include "balanced.h"

void read_books(std::vector<book> &books);
void shuffle(std::vector<book> &books);

void test_balanced() {
    int n;
    std::cout << "Insira o valor de N: ";
    std::cin >> n;

    std::vector<book> books;

    int insertion_comparisons = 0;
    int search_comparisons = 0;
    for (int i = 0; i < 5; i++) {
        shuffle(books);
        std::cout << "oops\n";
        red_black_tree<int> tree;

        for (int i = 0; i < n; i++) {
            tree.insert(100, books[i].id, insertion_comparisons);
        }

        for (int i = 0; i < n; i++) {
            tree.search(books[i].id, search_comparisons);
        }
    }

    std::cout << "Média Inserções: " << insertion_comparisons / 5 << std::endl;
    std::cout << "Média Busca: " << search_comparisons / 5 << std::endl;
}

void read_books(std::vector<book> &books) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `books.csv`!" << std::endl;
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
