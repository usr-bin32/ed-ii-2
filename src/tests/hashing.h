#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "../hashing/hash_table.h"
#include "../parsing/csv_parser.h"
#include "../sorting/quick_sort.h"
#include "../structures/author.h"
#include "../structures/book.h"

void test_hashing() {
    int n;
    std::cout << "Insira o valor de N: ";
    std::cin >> n;

    int m;
    std::cout << "Insira o valor de M: ";
    std::cin >> m;

    // hash_table<author> all_authors;
    // read_authors(authors);

    std::vector<book> books;
    read_books(books);
    shuffle(books);
    books.resize(n);

    // armazenar os livros lidos em uma hash table
    // criar uma nova hash_table de autores

    for (auto &b : books) {
        for (int id : b.author_ids) {
            // author default;
            // author *a = authors.insert(id, default);
            // if (a.name.empty()) a.name = all_books.search(id)->name;
            // a->occurrences += 1;
        }
    }

    std::vector<author> author_vec;
    // authors.to_vector(author_vec);
    quick_sort(author_vec.data(), author_vec.size());

    for (int i = 0; i < m; i++) {
        std::cout << author_vec[i].name << ": " << author_vec[i].occurrences
                  << std::endl;
    }
}

void read_books(std::vector<book> &books) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `books.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        book b;
        parser.get(0, b.author_ids);

        books.push_back(std::move(b));
    }
}

void shuffle(std::vector<book> &books) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(books.begin(), books.end(), std::default_random_engine(seed));
}
