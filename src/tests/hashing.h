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

// le o csv de authors e os adicionam em uma hash table
void read_authors(hash_table<author> &authors) {
    csv_parser parser("./res/authors.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `authors.csv`!" << std::endl;
        return;
    }

    while (parser.read_line()) {
        int id;
        std::string name;
        author aux;
        parser.get(0, id);
        parser.get(1, name);

        aux.name = name;
        aux.occurrences = 0;
        authors.insert(id, aux);
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
    do{
        std::cout << "Insira quantos autores deseja imprimir: ";
        std::cin >> m;
    }while(m>n);

    int tam_books = (n + n*0.20);
    hash_table<author> hash_authors(244000);
    read_authors(hash_authors);
   
    hash_table<book> hash_books(tam_books);
    std::vector<book> books;
    read_books(books); 
    shuffle(books);
    books.resize(n);

    // armazenando os livros lidos em uma hash table
    // coloca as ocorrencias dos autores na hash authors e verifica se tem algum autor nao cadastrado no csv
    for (auto &b : books) {
        hash_books.insert(b.id, b);

        for (int id : b.authors) {
        
            if(hash_authors.search(id)){
                hash_authors.search(id)->occurrences += 1; 
            }else{
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

    //imprimindo as ocorrencias de autores em ordem decrescente
    for (int i = 0; i < m && i < author_vec.size(); i++) {
        std::cout << author_vec[i].name << ": "
                  << author_vec[i].occurrences << std::endl;
    }
}
