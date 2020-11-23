#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

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

    // hash<author> authors;
    // read_authors(authors);

    std::vector<book> books;
    read_books(books);
    shuffle(books);
    books.resize(n);

    for (auto &b : books) {
        for (int id : b.author_ids) {
            // authors[id].occurrences += 1; ou authors.find(id).occurrences += 1
        }
    }

    std::vector<author> author_vec;
    // authors.to_vec(author_vec);
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

    // hash<book> books_hash;

    while (parser.read_line()) {
        book b;
        parser.get(0, b.author_ids);

        // int id;
        // parser.get(4, id);
        // books_hash[id] = b; ou books_hash.insert(id, b);

        books.push_back(std::move(b));
    }

    // books_hash.to_vec(books);
}

void shuffle(std::vector<book> &books) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(books.begin(), books.end(), std::default_random_engine(seed));
}