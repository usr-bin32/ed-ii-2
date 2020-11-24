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

void read_books(std::vector<book> &books) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `books.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        book b;

        parser.get(0, b.authors);

        books.push_back(std::move(b));
    }
}

int count_authors() {
    csv_parser parser("./res/authors.csv");
    int count = 0;

    if (!parser.is_open()) {
        std::cerr << "Failed to open `authors.csv`!" << std::endl;
        return 0;
    }

    while (parser.read_line()) {
        count += 1;
    }

    return count;
}

void read_authors(hash_table<std::string> &authors) {
    csv_parser parser("./res/authors.csv");
    int id;
    std::string name;

    if (!parser.is_open()) {
        std::cerr << "Failed to open `authors.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        parser.get(0, id);
        parser.get(1, name);
        authors.insert(id, name);
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
    std::cout << "Insira quantos autores deseja imprimir: ";
    std::cin >> m;

    int tam_books = (n + n*0.20), tam_authors = (n + n*0.35), tam_all_authors = count_authors(); 
    tam_all_authors += tam_all_authors*0.20; 

    hash_table<std::string> author_names(tam_all_authors);
    read_authors(author_names);

    hash_table<book> hash_books(tam_books);
    hash_table<author> hash_authors(tam_authors);
    std::vector<book> books;
    read_books(books); 
 //   shuffle(books);
    books.resize(n);

    // armazenando os livros lidos em uma hash table
    // criando uma nova hash_table com key = id e value = author { id,  contagem }

    for (auto &b : books) {

        hash_books.insert(b.id, b);
       
        for (int id : b.authors) {

            author a;
            a.id = id;
            a.occurrences = 1;

            if(hash_authors.search(id)){
                hash_authors.search(id)->occurrences += 1; 
            }else{
                hash_authors.insert(id, a);
            }
        }
    }

    std::vector<author> author_vec;
    hash_authors.to_vector(author_vec);
    quick_sort(author_vec.data(), author_vec.size());

    for (int i = 0; i < m; i++) {

        std::cout << *author_names.search(author_vec[i].id) << ": " <<
        author_vec[i].occurrences
                  << std::endl;
    }
}


