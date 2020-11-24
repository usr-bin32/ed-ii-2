#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "../hashing/hash_table.h"
#include "../parsing/csv_parser.h"
#include "../sorting/quick_sort.h"
#include "../structures/author.h"
#include "../structures/book.h"

// Aqui, ao invés de ler para um vector, ler direto para uma hash_table de
// autores
void read_books(std::vector<book> &books) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `books.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        book b;

        parser.get(0, b.authors);

        // já incrementar na tabela de autores logo aqui
        // cuidado com os autores inexistentes. Você deve lidar com eles aqui

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

void read_authors(std::vector<std::pair<int, std::string>> &authors) {
    csv_parser parser("./res/authors.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `authors.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {

        int id;
        std::string name;
        parser.get(0, id);
        parser.get(1, name);

        authors.push_back(std::make_pair(id, name));
    }
}

void shuffle(std::vector<book> &books) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(books.begin(), books.end(), std::default_random_engine(seed));
}

void test_hashing() {
    int n;
    std::cout << "Insira o quantos livros devem ser lidos: ";
    std::cin >> n;

    int m;
    do {
        std::cout << "Insira quantos autores deseja imprimir: ";
        std::cin >> m;
    } while (m > n);

    int tam_books = (n + n * 0.20), tam_authors = (n + n * 0.35),
        tam_all_authors;

    // ler todos os autores do CSV para uma hash_table<author>
    // a struct, que está na master, tem o seguinte formato:
    // { int id; std::string name; int occurrences = 0; }

    std::vector<std::pair<int, std::string>> all_authors;
    read_authors(all_authors);
    tam_all_authors = all_authors.size() + all_authors.size() * 0.20;

    hash_table<std::string> author_names(tam_all_authors);

    hash_table<book> hash_books(tam_books);
    hash_table<author> hash_authors(tam_authors);
    std::vector<book> books;
    read_books(books);
    shuffle(books);
    books.resize(n);

    // armazenando os livros lidos em uma hash table
    // criando uma nova hash_table com key = id e value = author { id,  contagem
    // }
    for (auto &b : books) {

        hash_books.insert(b.id, b);

        for (int id : b.authors) {

            author a;
            a.id = id;
            a.occurrences = 1;

            if (hash_authors.search(id)) {
                hash_authors.search(id)->occurrences += 1;
            } else {
                hash_authors.insert(id, a);
            }
        }
    }
    // armazena os autores em hash table
    for (auto &a : all_authors) {
        author_names.insert(a.first, a.second);
    }

    std::vector<author> author_vec;
    hash_authors.to_vector(author_vec);
    quick_sort(author_vec.data(), author_vec.size());

    for (int i = 0; i < m && i < author_vec.size(); i++) {
        // aqui, bastará pegar o autor:
        // author_vec[i]
        // std::cout << author_vec[i].name << ... << author_vec[i].occurrences
        if (author_names.search(author_vec[i].id) == nullptr) {
            m++;
            continue;
        }

        std::cout << *author_names.search(author_vec[i].id) << ": "
                  << author_vec[i].occurrences << std::endl;
    }
}
