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

// Lê o csv de authors e os adicionam em uma hash table
void read_authors(hash_table<author> &authors);

// Lê N livros aleatórios do CSV e os adicionam a uma tabela hash, ao mesmo
// tempo que incrementa a frequência dos autores encontrados nos livros
void read_books_hash(hash_table<book> &books, hash_table<author> &authors,
                     int n);

void test_hashing() {
    // Lê a entrada do usuário
    int n = 0;
    do {
        std::cout << "Insira quantos livros devem ser lidos: ";
        std::cin >> n;
    } while (n <= 0);

    int m = 0;
    do {
        std::cout << "Insira quantos autores devem ser impressos: ";
        std::cin >> m;
    } while (m > n || m <= 0);

    std::cout << std::endl;

    // o número utilizado para definir o tamanho da tabela é o número total de
    // autores no arquivo somado a uma quantia de espaço extra. A quantia de
    // espaço estra foi inclusa por questões de desempenho, para diminuir o
    // número de colisões e tornar mais rápido o tempo médio de busca.
    hash_table<author> authors(243699 * 1.25);
    read_authors(authors);

    // Inicializa e lê uma tabela hash de livros. A tabela é iniciada com N
    // somado a uma quantia de espaço estra para otimizar a performance
    hash_table<book> books(n * 1.15);
    read_books_hash(books, authors, n);

    // Extrai os autores da tabela hash para um vector para que os autores
    // possam ser ordenados de acordo com sua frequência
    std::vector<author> author_vec;
    authors.to_vector(author_vec);
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

void read_books_hash(hash_table<book> &books, hash_table<author> &authors,
                     int n) {
    csv_parser parser("./res/books.csv");

    if (!parser.is_open()) {
        std::cerr << "Failed to open `books.csv`!" << std::endl;
        return;
    }

    std::vector<int> indices(n);
    // o número utilizado equivale à quantidade de linhas no arquivo CSV
    generate_indices(indices, 1086955);
    // os índices devem estar ordenados, pois a leitura do arquivo é sequencial
    std::sort(indices.begin(), indices.end());

    // O loop lê e ignora cada linha do CSV até encontrar a próxima linha que
    // coincida com o próximo índice a ser lido. Quando isso ocorre, os dados
    // lidos são usados para inserir uma struct book numa tabela hash de livros
    // e para incrementar a frequência dos autores
    int i = 0;
    for (int index : indices) {
        while (i != index) {
            parser.read_line();
            i++;
        }

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

        books.insert(b.id, std::move(b));

        for (int id : b.authors) {
            author *a = authors.search(id);
            if (a != nullptr) {
                a->occurrences += 1;
            } else {
                author aux;
                aux.name = "Desconhecido";
                aux.occurrences = 1;
                authors.insert(id, aux);
            }
        }
    }
}
