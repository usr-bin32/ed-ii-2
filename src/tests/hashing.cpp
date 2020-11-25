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
#include "common.h"
#include "hashing.h"

// le o csv de authors e os adicionam em uma hash table
void read_authors(hash_table<author> &authors);

void test_hashing() {
    int n = 0;
    do {
        std::cout << "Insira quantos livros devem ser lidos: ";
        std::cin >> n;
    } while (n <= 0);

    int m = 0;
    do {
        std::cout << "Insira quantos autores devem ser impresoss: ";
        std::cin >> m;
    } while (m > n || m <= 0);

    hash_table<author> hash_authors(244000);
    read_authors(hash_authors);

    std::vector<book> books;
    read_books(books);
    shuffle(books);
    books.resize(n);

    // armazenando os livros lidos em uma hash table
    // coloca as ocorrencias dos autores na hash authors e verifica se tem algum
    // autor nao cadastrado no csv
    hash_table<book> hash_books(n * 1.15);
    for (auto &b : books) {
        hash_books.insert(b.id, b);

        for (int id : b.authors) {
            author *a = hash_authors.search(id);
            if (a != nullptr) {
                a->occurrences += 1;
            } else {
                author aux;
                aux.name = "Desconhecido";
                aux.occurrences = 1;
                hash_authors.insert(id, aux);
            }
        }
    }

    std::vector<author> author_vec;
    hash_authors.to_vector(author_vec);
    quick_sort(author_vec.data(), author_vec.size());

    // imprimindo as ocorrencias de autores em ordem decrescente
    for (int i = 0; i < m && i < author_vec.size(); i++) {
        // caso um autor com zero ocorrências seja encontrado antes de i chegar
        // a m, significa que todos os autores referenciados nos N livros já
        // foram impressos
        if (author_vec[i].occurrences == 0) {
            break;
        }

        std::cout << author_vec[i].name << ": " << author_vec[i].occurrences
                  << std::endl;
    }
}

void read_authors(hash_table<author> &authors) {
    csv_parser parser("./res/authors.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `authors.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        int id;
        author a;
        parser.get(0, id);
        parser.get(1, a.name);

        authors.insert(id, std::move(a));
    }
}
