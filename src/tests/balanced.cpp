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
#include "common.h"

constexpr int NUM_TESTS = 5;

void read_input(std::vector<int> &input_sizes);
void read_books(std::vector<book> &books);
void generate_keys(std::vector<long> &keys);

void test_red_black(std::vector<book> &books, int n, std::ofstream &insert_out,
                    std::ofstream &search_out, float random_frac);
void test_btree(std::vector<book> &books, int n, std::ofstream &insert_out,
                std::ofstream &search_out, float random_frac, int degree);

void test_balanced() {
    std::vector<int> input_sizes;
    read_input(input_sizes);

    // abandona a execução caso não haja Ns de entrada
    if (input_sizes.empty()) {
        return;
    }

    float random_frac = 0.0f;
    do {
        std::cout << "Insira a fração das chaves buscadas que será aleatória "
                     "(0 a 1): ";
        std::cin >> random_frac;
    } while (random_frac < 0 || random_frac > 1);

    std::cout << std::endl;

    std::vector<book> books;
    read_books(books);

    std::ofstream insert_out("./saidaInsercao.txt", std::ios_base::trunc);
    std::ofstream search_out("./saidaBusca.txt", std::ios_base::trunc);

    for (int n : input_sizes) {
        std::cout << "N = " << n << std::endl;
        insert_out << "N = " << n << std::endl;
        search_out << "N = " << n << std::endl;

        test_red_black(books, n, insert_out, search_out, random_frac);
        test_btree(books, n, insert_out, search_out, random_frac, 2);
        test_btree(books, n, insert_out, search_out, random_frac, 20);

        std::cout << std::endl;
        insert_out << std::endl;
        search_out << std::endl;
    }

    insert_out.close();
    search_out.close();
}

void read_input(std::vector<int> &input_sizes) {
    std::ifstream input_file("./entrada.txt");

    if (!input_file.is_open()) {
        std::cerr << "Falha ao tentar abrir `entrada.txt`!" << std::endl;
        return;
    }

    int len = 0;
    input_file >> len;

    input_sizes.reserve(len);
    for (int i = 0; i < len; i++) {
        int size = 0;
        input_file >> size;

        input_sizes.push_back(size);
    }
}

void test_red_black(std::vector<book> &books, int n, std::ofstream &insert_out,
                    std::ofstream &search_out, float random_frac) {
    int insertion_cmp = 0;
    double insertion_time = 0;

    int search_cmp = 0;
    double search_time = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        double t0;
        double t1;

        red_black_tree<book *> tree;

        std::vector<int> indices(n);
        generate_indices(indices, books.size());

        // inserção
        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i : indices) {
            tree.insert(books[i].id, &books[i], insertion_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        insertion_time += t1 - t0;

        // busca com chaves aleatórias
        std::vector<long> random_keys(n * random_frac);
        generate_keys(random_keys);

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (auto key : random_keys) {
            tree.search(key, search_cmp);
        }

        // busca com chaves presentes
        for (int i = 0; i < n * (1 - random_frac); i++) {
            tree.search(books[indices[i]].id, search_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        search_time += t1 - t0;
    }

    std::cout << "Árvore Vermelho-Preto:" << std::endl;
    insert_out << "Árvore Vermelho-Preto:" << std::endl;
    search_out << "Árvore Vermelho-Preto:" << std::endl;

    std::cout << "    Comparações nas Inserções: " << insertion_cmp / NUM_TESTS
              << std::endl;
    std::cout << "    Comparações nas Buscas:    " << search_cmp / NUM_TESTS
              << std::endl;
    std::cout << "    Tempo nas Inserções (s):   " << insertion_time / NUM_TESTS
              << std::endl;
    std::cout << "    Tempo nas Buscas (s):      " << search_time / NUM_TESTS
              << std::endl;

    insert_out << "    Comparações: " << insertion_cmp / NUM_TESTS << std::endl;
    insert_out << "    Tempo (s):   " << insertion_time / NUM_TESTS
               << std::endl;
    search_out << "    Comparações: " << search_cmp / NUM_TESTS << std::endl;
    search_out << "    Tempo (s):   " << search_time / NUM_TESTS << std::endl;
}

void test_btree(std::vector<book> &books, int n, std::ofstream &insert_out,
                std::ofstream &search_out, float random_frac, int degree) {
    int insertion_cmp = 0;
    double insertion_time = 0;

    int search_cmp = 0;
    double search_time = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        double t0;
        double t1;

        btree<book *> tree(degree);

        std::vector<int> indices(n);
        generate_indices(indices, books.size());

        // inserção
        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (int i : indices) {
            tree.insert(books[i].id, &books[i], insertion_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        insertion_time += t1 - t0;

        // busca com chaves aleatórias
        std::vector<long> random_keys(n * random_frac);
        generate_keys(random_keys);

        t0 = double(clock()) / CLOCKS_PER_SEC;
        for (auto key : random_keys) {
            tree.search(key, search_cmp);
        }

        // busca com chaves presentes
        for (int i = 0; i < n * (1 - random_frac); i++) {
            tree.search(books[indices[i]].id, search_cmp);
        }
        t1 = double(clock()) / CLOCKS_PER_SEC;
        search_time += t1 - t0;
    }

    std::cout << "Árvore B (d = " << degree << "):" << std::endl;
    insert_out << "Árvore B (d = " << degree << "):" << std::endl;
    search_out << "Árvore B (d = " << degree << "):" << std::endl;

    std::cout << "    Comparações nas Inserções: " << insertion_cmp / NUM_TESTS
              << std::endl;
    std::cout << "    Comparações nas Buscas:    " << search_cmp / NUM_TESTS
              << std::endl;
    std::cout << "    Tempo nas Inserções (s):   " << insertion_time / NUM_TESTS
              << std::endl;
    std::cout << "    Tempo nas Buscas (s):      " << search_time / NUM_TESTS
              << std::endl;

    insert_out << "    Comparações: " << insertion_cmp / NUM_TESTS << std::endl;
    insert_out << "    Tempo (s):   " << insertion_time / NUM_TESTS
               << std::endl;
    search_out << "    Comparações: " << search_cmp / NUM_TESTS << std::endl;
    search_out << "    Tempo (s):   " << search_time / NUM_TESTS << std::endl;
}

void generate_keys(std::vector<long> &keys) {
    // os valores utilizados abaixo são os valores mínimo e máximo que um id
    // pode ter dentro do arquivo de livros. Em uma distribuição uniforme, é
    // estimado que apenas 0.0040% dos números gerados sejam um id existente,
    // praticamente zero.
    std::uniform_int_distribution<long> distribution(9771130767002,
                                                     9798484760114);
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

    std::generate(keys.begin(), keys.end(), [&distribution, &generator]() {
        return distribution(generator);
    });
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
