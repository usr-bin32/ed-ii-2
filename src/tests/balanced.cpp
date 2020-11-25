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

constexpr int NUM_TESTS = 5;

void read_input(std::vector<int> &sizes);
void read_books(std::vector<book> &books);
void test_red_black(std::vector<book> &books, int n);
void test_btree(std::vector<book> &books, int n, int degree);
void shuffle(std::vector<book> &books);

void test_balanced() {
    std::vector<int> sizes;
    read_input(sizes);

    // abandona a execução caso não haja Ns de entrada
    if (sizes.empty()) {
        return;
    }

    std::vector<book> books;
    read_books(books);

    for (int n : sizes) {
        std::cout << "N = " << n << std::endl;

        test_red_black(books, n);
        test_btree(books, n, 2);
        test_btree(books, n, 20);

        std::cout << std::endl;
    }
}

void read_input(std::vector<int> &sizes) {
    std::ifstream input_file("./entrada.txt");

    if (!input_file.is_open()) {
        std::cerr << "Falha ao tentar abrir `entrada.txt`!" << std::endl;
        return;
    }

    int len = 0;
    input_file >> len;

    sizes.reserve(len);
    for (int i = 0; i < len; i++) {
        int size = 0;
        input_file >> size;

        sizes.push_back(size);
    }
}

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

void test_red_black(std::vector<book> &books, int n) {
    int insertion_cmp = 0;
    double insertion_time = 0;

    int search_cmp = 0;
    double search_time = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        double t0;
        double t1;

        shuffle(books);

        red_black_tree<book> tree;

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i = 0; i < n; i++) {
            tree.insert(books[i], books[i].id, insertion_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        insertion_time += t1 - t0;

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i = 0; i < n; i++) {
            tree.search(books[i].id, search_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        search_time += t1 - t0;
    }

    std::cout << "Árvore Vermelho-Preto:" << std::endl;

    std::cout << "    Média de Comparações nas Inserções: "
              << insertion_cmp / NUM_TESTS << std::endl;
    std::cout << "    Média de Comparações nas Buscas:    "
              << search_cmp / NUM_TESTS << std::endl;
    std::cout << "    Média de Tempo nas Inserções (s):   "
              << insertion_time / NUM_TESTS << std::endl;
    std::cout << "    Média de Tempo nas Buscas (s):      "
              << search_time / NUM_TESTS << std::endl;
}

void test_btree(std::vector<book> &books, int n, int degree) {
    int insertion_cmp = 0;
    double insertion_time = 0;

    int search_cmp = 0;
    double search_time = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        double t0;
        double t1;

        shuffle(books);

        btree<book> tree(degree);

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i = 0; i < n; i++) {
            tree.insert(books[i].id, books[i], insertion_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        insertion_time += t1 - t0;

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i = 0; i < n; i++) {
            tree.search(books[i].id, search_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        search_time += t1 - t0;
    }

    std::cout << "Árvore B (d = " << degree << "):" << std::endl;

    std::cout << "    Média de Comparações nas Inserções: "
              << insertion_cmp / NUM_TESTS << std::endl;
    std::cout << "    Média de Comparações nas Buscas:    "
              << search_cmp / NUM_TESTS << std::endl;
    std::cout << "    Média de Tempo nas Inserções (s):   "
              << insertion_time / NUM_TESTS << std::endl;
    std::cout << "    Média de Tempo nas Buscas (s):      "
              << search_time / NUM_TESTS << std::endl;
}

void shuffle(std::vector<book> &books) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(books.begin(), books.end(), std::default_random_engine(seed));
}
